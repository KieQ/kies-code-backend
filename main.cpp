#include <cstdlib>
#include <cinatra.hpp>
#include <spdlog/spdlog.h>

#include "handler/page.hpp"
#include "handler/user.hpp"

#include "utils/file.hpp"

#include "db/t_user_info.hpp"

using namespace cinatra;

void bind_page(http_server &server)
{
    server.set_http_handler<GET>("/", handler::home_page);              //The home page
}

void bind_api(http_server &server)
{
    //user management
    server.set_http_handler<POST>("/api/user/log_in", handler::log_in);
    server.set_http_handler<POST>("/api/user/create_user", handler::create_user);
    server.set_http_handler<POST>("/api/user/log_out", handler::log_out);
    server.set_http_handler<POST>("/api/user/remove_user", handler::remove_user);
    server.set_http_handler<POST>("/api/user/update_user_info", handler::update_user_info);
    server.set_http_handler<GET>("/api/user/get_user_info", handler::get_user_info);

}

int main()
{
    const char *port = std::getenv("PORT");
    if (port == nullptr)
    {
        port = "8080";
    }

    SPDLOG_INFO("PORT={}", port);

    std::size_t max_thread_num = std::thread::hardware_concurrency();
    http_server server(max_thread_num);
    server.listen("0.0.0.0", port);
    server.set_not_found_handler(handler::not_found_page);

    bind_page(server);
    bind_api(server);
    SPDLOG_INFO("Server starts with {} threads", max_thread_num);
    server.run();    

    return 0;
}