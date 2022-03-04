#include "video.hpp"
#include "../dto/video.hpp"
#include "../utils/cinatra.hpp"
#include "../utils/json.hpp"
#include "../constant/error_code.hpp"
#include "../service/download.h"

#include <filesystem>

using namespace cinatra;
using namespace constant;

namespace handler
{

    void video_list(cinatra::request &req, cinatra::response &res)
    {
        SPDLOG_INFO("【KIE DEBUG】 #1");
        auto log_id = utils::req_id(req);
        auto videos = db::t_video::fetch(log_id, {});
        std::vector<dto::VideoListResponse> all_videos;
        for (auto &video : videos)
        {
            bool can_display = std::filesystem::exists(std::string{service::Downloader::get().get_save_position()} + "/" + video.video_name) && video.state == 1;
            all_videos.push_back(dto::VideoListResponse{.name = video.video_name, .profile = video.profile_link, .file_size = video.video_size, .state = video.state, .can_display = can_display, .hash_key = video.video_hash});
        }
        utils::on_json(log_id, res, utils::wrap(StatusCode::Success, "", all_videos));
        return;
    }

    void video_add(cinatra::request &req, cinatra::response &res)
    {
        auto type = req.get_query_value("type");
        auto log_id = utils::req_id(req);

        if (type == "hash")
        {
            auto [param, _, success] = utils::parse_request<dto::VideoDownloadWithHashRequest>(req);
            if (!success)
            {
                SPDLOG_ERROR("log_id={}, failed to parse the parameters", log_id);
                utils::on_json(log_id, res, utils::wrap(StatusCode::FailedToParseRequest, "failed to parse the parameters"));
                return;
            }
            service::Downloader::get().add_hash(log_id, param.hash);
            utils::on_json(log_id, res, utils::wrap());
            return;
        }
        else if (type == "torrent")
        {
            auto file = req.get_file();
            if (file == nullptr)
            {
                SPDLOG_ERROR("log_id={}, couldn't get file", log_id);
                utils::on_json(log_id, res, utils::wrap(StatusCode::FailedToParseRequest, "couldn't get file"));
                return;
            }
            auto profile_link = req.get_query_value("profile_link");

            SPDLOG_ERROR("{}, {}", file== nullptr, profile_link);
            service::Downloader::get().add_torrent(log_id, profile_link, file->get_file_path());
            file->remove();
            utils::on_json(log_id, res, utils::wrap());
            return;
        }
        else if (type == "magnet")
        {
            auto [param, _, success] = utils::parse_request<dto::VideoDownloadWithMagnetRequest>(req);
            if (!success)
            {
                SPDLOG_ERROR("log_id={}, failed to parse the parameters", log_id);
                utils::on_json(log_id, res, utils::wrap(StatusCode::FailedToParseRequest, "failed to parse the parameters"));
                return;
            }
            service::Downloader::get().add_magnet(log_id, param.profile_link, param.magnet_link);
            utils::on_json(log_id, res, utils::wrap());
            return;
        }
        else
        {
            SPDLOG_ERROR("log_id={}, unknown type", log_id);
            utils::on_json(log_id, res, utils::wrap(StatusCode::FailedToParseRequest, "unknown type"));
            return;
        }
    }

    void video_progress(cinatra::request &req, cinatra::response &res)
    {
        auto log_id = utils::req_id(req);
        auto key = req.get_query_value("key");
        auto [progress, success] = service::Downloader::get().get_and_update(log_id, std::string{key});
        if (!success)
        {
            SPDLOG_ERROR("log_id={}, failed to fetch progress for {}", log_id, key);
            utils::on_json(log_id, res, utils::wrap(StatusCode::OperationFailed, "failed to fetch"));
            return;
        }
        utils::on_json(log_id, res, utils::wrap(StatusCode::Success, "", progress));
        return;
    }

    void video_remove(cinatra::request &req, cinatra::response &res)
    {
        auto [param, log_id, success] = utils::parse_request<dto::VideoRemoveRequest>(req);
        if (!success)
        {
            SPDLOG_ERROR("log_id={}, failed to parse the parameters", log_id);
            utils::on_json(log_id, res, utils::wrap(StatusCode::FailedToParseRequest, "failed to parse the parameters"));
            return;
        }
        service::Downloader::get().remove(log_id, param.key);
        utils::on_json(log_id, res, utils::wrap());
        return;
    }

} // namespace handler
