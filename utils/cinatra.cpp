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

} // namespace utils
