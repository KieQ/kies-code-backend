#include "download.h"

#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/hex.hpp>
#include <spdlog/spdlog.h>
#include <algorithm>
#include <filesystem>

#include "../db/t_video.hpp"
#include "../utils/time.hpp"

using libtorrent::aux::to_hex;

namespace service
{
    Downloader Downloader::d;

    Downloader &Downloader::get()
    {
        return d;
    }

    bool Downloader::init()
    {
        return false;
    }

    std::string_view Downloader::get_save_position()
    {
        return save_position;
    }

    bool Downloader::add_torrent(std::string_view log_id, std::string_view profile_link, const std::string &torrent_file)
    {
        lt::add_torrent_params apt;
        apt.save_path = save_position;
        apt.ti = std::make_shared<lt::torrent_info>(torrent_file);
        auto hash = to_hex(apt.ti->info_hash().to_string());
        if (downloading.find(hash) != downloading.end())
        {
            SPDLOG_INFO("log_id={}, torrrent file {} has been added to downloading", log_id, hash);
            return true;
        }

        auto h = session.add_torrent(apt);
        if (auto [_, exist] = db::t_video::fetch_first(log_id, {{"video_hash", hash}}); exist)
        {
            auto result = db::t_video::update(log_id, {{"video_hash", hash}}, {{"added_time", h.status().added_time}, {"profile_link", profile_link}, {"state", 0}});
            if (result.affected_rows() == 0)
            {
                SPDLOG_WARN("log_id={}, failed to update database", log_id);
                return false;
            }
        }
        else
        {
            auto video_link = lt::make_magnet_uri(lt::torrent_info{torrent_file});
            auto result = db::t_video::insert(log_id, {{"video_hash", hash},
                                                       {"video_name", h.status().name},
                                                       {"added_time", h.status().added_time},
                                                       {"video_link", video_link},
                                                       {"profile_link", profile_link}});

            if (result.affected_rows() == 0)
            {
                SPDLOG_WARN("log_id={}, failed to insert database", log_id);
                return false;
            }
        }

        downloading.insert({hash, LastMeta{.last_time = utils::now_ms(), .last_size = h.status().total_payload_download, .handle = h}});
        return true;
    }

    bool Downloader::add_magnet(std::string_view log_id, std::string_view profile_link, std::string_view video_link)
    {
        lt::add_torrent_params apt = lt::parse_magnet_uri(lt::string_view{video_link.data(), video_link.length()});
        auto hash = to_hex(apt.info_hash.to_string());
        if (downloading.find(hash) != downloading.end())
        {
            SPDLOG_INFO("log_id={}, torrrent file {} has been added to downloading", log_id, hash);
            return true;
        }
        apt.save_path = save_position;


        auto h = session.add_torrent(apt);

        if (auto [_, exist] = db::t_video::fetch_first(log_id, {{"video_hash", hash}}); exist)
        {
            auto result = db::t_video::update(log_id, {{"video_hash", hash}}, {{"added_time", h.status().added_time}, {"profile_link", profile_link}, {"state", 0}});
            if (result.affected_rows() == 0)
            {
                SPDLOG_WARN("log_id={}, failed to update database", log_id);
                return false;
            }
        }
        else
        {
            auto result = db::t_video::insert(log_id, {{"video_hash", hash},
                                                       {"video_name", h.status().name},
                                                       {"added_time", h.status().added_time},
                                                       {"video_link", video_link},
                                                       {"profile_link", profile_link}});
            if (result.affected_rows() == 0)
            {
                return false;
            }
        }

        downloading.insert({hash, LastMeta{.last_time = utils::now_ms(), .last_size = h.status().total_payload_download, .handle = h}});
        return true;
    }

    bool Downloader::add_hash(std::string_view log_id, std::string_view hash){
        auto [video_info, exist] = db::t_video::fetch_first(log_id, {{"video_hash", hash}});
        if(!exist){
            SPDLOG_WARN("log_id={}, video {} doesn't exist", log_id, hash);
            return false;
        }

        lt::add_torrent_params apt = lt::parse_magnet_uri(video_info.video_link);
        if (downloading.find(std::string{hash}) != downloading.end())
        {
            SPDLOG_INFO("log_id={}, torrrent file {} has been added to downloading", log_id, hash);
            return true;
        }
        apt.save_path = save_position;
        auto h = session.add_torrent(apt);

        downloading.insert({std::string{hash}, LastMeta{.last_time = utils::now_ms(), .last_size = h.status().total_payload_download, .handle = h}});

        return true;
    }

    std::tuple<Downloader::DownloadProgress, bool> Downloader::get_and_update(std::string_view log_id, const std::string &key)
    {
        if (downloading.find(key) == downloading.end())
        {
            if (auto [video_info, exist] = db::t_video::fetch_first(log_id, {{"video_hash", key}}); exist)
            {
                DownloadProgress progress{
                    .state = 5,
                    .speed = 0,
                    .paused = true,
                    .file_size = video_info.video_size,
                    .downloaded_size = video_info.state == 1 ? video_info.video_size : 0,
                    .progress = video_info.state == 1 ? 1.0f : 0.0f,
                    .file_name = video_info.video_name};
                return {progress, true};
            }
            else
            {
                SPDLOG_INFO("log_id={}, video {} does not exist", log_id, key);
                return {{}, false};
            }
        }
        SPDLOG_INFO("STATUS: {}", static_cast<int>(downloading[key].handle.status().state));

        if (downloading[key].handle.status().state == 5) // is seeding
        {
            auto result = db::t_video::update(log_id, {{"video_hash", key}}, {{"state", 1}, {"completed_time", downloading[key].handle.status().completed_time}, {"video_size", downloading[key].handle.status().total_wanted}});
            if (result.affected_rows() == 0)
            {
                SPDLOG_INFO("log_id={}, failed to update {}", log_id, key);
                return {{}, false};
            }
            DownloadProgress progress{
                .state = 5,
                .speed = 0,
                .paused = true,
                .file_size = downloading[key].handle.status().total_wanted,
                .downloaded_size = downloading[key].handle.status().total_wanted,
                .progress = 1.0,
                .file_name = downloading[key].handle.status().name};
            downloading[key].handle.unset_flags(lt::torrent_flags::seed_mode);
            downloading[key].handle.unset_flags(lt::torrent_flags::upload_mode);
            downloading[key].handle.unset_flags(lt::torrent_flags::share_mode);
            SPDLOG_INFO("IS SEEDING");
            // session.remove_torrent(downloading[key].handle);
            // downloading.erase(key);
            return {progress, true};
        }
        else
        {
            auto now = utils::now_ms();
            auto size = downloading[key].handle.status().total_payload_download;
            auto time_diff = std::max(now - downloading[key].last_time, static_cast<std::int64_t>(1));
            auto size_diff = std::max(size - downloading[key].last_size, static_cast<std::int64_t>(0));

            downloading[key].last_size = size;
            downloading[key].last_time = now;
            DownloadProgress progress{
                .state = downloading[key].handle.status().state,
                .speed = size_diff * 1000 / time_diff,
                .paused = false,
                .file_size = downloading[key].handle.status().total_wanted,
                .downloaded_size = downloading[key].handle.status().total_wanted_done,
                .progress = downloading[key].handle.status().progress,
                .file_name = downloading[key].handle.status().name};
            return {progress, true};
        }
    }

    void Downloader::remove(std::string_view log_id, const std::string &key)
    {
        if(downloading.find(key) != downloading.end()){
            auto delete_files = lt::session_handle::delete_files; //this is needed because libtorrent is compiled with C++14 or C++11 by conan and this project is in C++17. This might be a feature or bug of GCC which is related to unique symbol, so the linker will complain multi definition of delete_files. So I use this trick to walk around this bug. The bug might be fixed later even GCC 11 has this one as well, according to the internet, on which someone found this as well.
            session.remove_torrent(downloading[key].handle, delete_files);
            downloading.erase(key);
        }

        auto [video, exist] = db::t_video::fetch_first(log_id, {{"video_hash", key}});
        if (exist)
        {
            auto result = db::t_video::remove(log_id, {{"video_hash", key}});
            if (result.affected_rows() == 0)
            {
                SPDLOG_INFO("log_id={}, failed to delete video {}", log_id, key);
            }
            std::filesystem::remove(save_position+"/"+video.video_name);
        }
    }
} // namespace service
