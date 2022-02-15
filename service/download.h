#ifndef SERVICE_DOWNLOADS_H
#define SERVICE_DOWNLOADS_H

#include <unordered_map>
#include <string>
#include <string_view>
#include <tuple>
#include <shared_mutex>

#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/session.hpp>
#include <nlohmann/json.hpp>

#include "../db/t_video.hpp"

namespace service
{
    class Downloader
    {
        struct DownloadProgress
        {
            std::int64_t state;
            std::int64_t speed;
            bool paused;
            std::int64_t file_size;
            std::int64_t downloaded_size;
            float progress;
            std::string file_name;

            NLOHMANN_DEFINE_TYPE_INTRUSIVE(DownloadProgress, state, speed, paused, file_size, downloaded_size, progress, file_name)
        };

        struct LastMeta
        {
            std::int64_t last_time;
            std::int64_t last_size;
            lt::torrent_handle handle;
        };

        std::unordered_map<std::string, LastMeta> downloading;
        lt::session session;
        std::string save_position = "./media";
        static Downloader d;

        Downloader() = default;

    public:
        static Downloader &get();

        bool init();
        std::string_view get_save_position();

        bool add_torrent(std::string_view log_id, std::string_view profile_link, const std::string &torrent_file);
        bool add_magnet(std::string_view log_id, std::string_view profile_link, std::string_view video_link);
        std::tuple<DownloadProgress, bool> get_and_update(std::string_view log_id, const std::string &key);
        void remove(std::string_view log_id, const std::string &key);
    };
} // namespace service

#endif