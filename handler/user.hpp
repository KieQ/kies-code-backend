#ifndef HANDLER_USER_HPP
#define HANDLER_USER_HPP

#include <cinatra.hpp>

namespace handler
{
    void log_in(cinatra::request &req, cinatra::response &res);
    void log_out(cinatra::request &req, cinatra::response &res);
    void remove_user(cinatra::request &req, cinatra::response &res);
    void create_user(cinatra::request &req, cinatra::response &res);
    void update_user_info(cinatra::request &req, cinatra::response &res);
    void get_user_info(cinatra::request &req, cinatra::response &res);

} // namespace handler


#endif