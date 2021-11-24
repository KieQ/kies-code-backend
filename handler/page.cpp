#include "page.hpp"

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#include "../utils/file.hpp"
#include "../utils/log.hpp"

using namespace cinatra;


namespace handler
{
    void home_page(cinatra::request &req, cinatra::response &res)
    {
        auto log_id = utils::req_id(req);
        SPDLOG_INFO("log_id={}, homepage, ip={}", log_id, req.get_header_value("X-Forwarded-For"));

        res.set_status_and_content(status_type::ok, utils::read_template("www/index.html"), req_content_type::html, content_encoding::gzip);
    }


    void not_found_page(cinatra::request &req, cinatra::response &res){
        auto log_id = utils::req_id(req);
        SPDLOG_INFO("log_id={}, 404 not found page, ip={}", log_id, req.get_header_value("X-Forwarded-For"));
        res.redirect("/",true);
    }


} // namespace handler
