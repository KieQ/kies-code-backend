#ifndef HANDLER_VIDEO_HPP
#define HANDLER_VIDEO_HPP

#include <cinatra.hpp>

namespace handler
{
    void video_list(cinatra::request& req, cinatra::response& res);
    void video_add(cinatra::request& req, cinatra::response& res);
    void video_progress(cinatra::request& req, cinatra::response& res);
    void video_remove(cinatra::request& req, cinatra::response& res);
} // namespace handler


#endif