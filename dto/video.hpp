#ifndef DTO_VIDEO_HPP
#define DTO_VIDEO_HPP

#include "macro.hpp"
#include <string>
#include <nlohmann/json.hpp>

namespace dto
{
    struct VideoDownloadRequest{
        std::string magnet_link;
        std::string profile_link;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(VideoDownloadRequest, magnet_link, profile_link)
    };

    struct VideoPauseAndResumeRequest {
        bool pause;
        std::string key;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(VideoPauseAndResumeRequest, pause, key)
    };

    struct VideoRemoveRequest {
        std::string key;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(VideoRemoveRequest, key);
    };

    struct VideoListResponse {
        std::string name;
        std::string profile;
        std::int64_t file_size;
        std::int64_t state;
        bool can_display;
        std::string hash_key;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(VideoListResponse, name, profile, file_size, state, can_display, hash_key);
    };

} // namespace dto


#endif