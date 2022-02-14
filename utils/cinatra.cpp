#include "cinatra.hpp"

namespace utils
{
    std::string get_ip(cinatra::request &req)
    {
        return req.get_conn<cinatra::NonSSL>()->remote_address();
    }

    std::shared_ptr<cinatra::session> check_and_refresh_session(cinatra::request& req ,bool need_refresh){
        auto session = req.get_session().lock();
        if(session != nullptr && need_refresh){
            session->set_max_age(1800);
        }
        return session;
    }

    void on_json(std::string_view log_id, cinatra::response& res, std::string_view s){
        res.add_header("X-LOG-ID", std::string(log_id));
        res.set_status_and_content(cinatra::status_type::ok, std::string(s), cinatra::req_content_type::json);
    }

} // namespace utils
