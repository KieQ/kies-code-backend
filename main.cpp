#include <cstdlib>
#include <cinatra.hpp>
#include <spdlog/spdlog.h>

#include "handler/user.hpp"
#include "handler/video.hpp"

#include "db/t_user_info.hpp"

#include "utils/cinatra.hpp"

using namespace cinatra;


void bind_api(http_server &server)
{
    //user management
    server.set_http_handler<POST>("/user/log_in", handler::log_in);
    server.set_http_handler<POST>("/user/create_user", handler::create_user);
    server.set_http_handler<POST>("/user/log_out", handler::log_out);
    server.set_http_handler<POST>("/user/remove_user", handler::remove_user);
    server.set_http_handler<POST>("/user/update_user_info", handler::update_user_info);
    server.set_http_handler<GET>("/user/get_user_info", handler::get_user_info);

    //video management
    server.set_http_handler<POST>("/video/add", handler::video_add);
    server.set_http_handler<GET>("/video/progress", handler::video_progress);
    server.set_http_handler<POST>("/video/remove", handler::video_remove);
    server.set_http_handler<GET>("/video/list", handler::video_list);
    // server.set_not_found_handler([](auto& req, auto& res){
    //     res.add_header("Access-Control-Allow-Origin", "*");
    //     res.add_header("Access-Control-Allow-Headers","X-Requested-With,Content-Type,Accept");
    //     res.set_status_and_content(status_type::ok,"not found");
    // });
}   

int main()
{
    const char* port = std::getenv("BACKEND_PORT");
    if(port == nullptr){
        port = "8082";
    }

    SPDLOG_INFO("PORT={}", port);

    std::size_t max_thread_num = std::thread::hardware_concurrency();
    http_server server(max_thread_num);
    server.listen("0.0.0.0", port);

    bind_api(server);
    SPDLOG_INFO("Server starts with {} threads", max_thread_num);
    server.run();    

    return 0;
}