#include "user.hpp"
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#include "../utils/log.hpp"
#include "../utils/json.hpp"
#include "../utils/cinatra.hpp"
#include "../dto/user.hpp"
#include "../utils/time.hpp"
#include "../db/t_user_info.hpp"
#include "../constant/error_code.hpp"

using namespace cinatra;
using namespace constant;

namespace handler
{
    void log_in(cinatra::request &req, cinatra::response &res)
    {        
        auto [param, log_id, success] = utils::parse_request<dto::UserLogInRequest>(req);
        if (!success)
        {
            SPDLOG_ERROR("log_id={}, failed to parse the parameters", log_id);
            res.set_status_and_content(status_type::ok, utils::resp(StatusCode::FailedToParseRequest, "failed to parse the parameters"), req_content_type::json);
            return;
        }

        if (auto session = utils::check_and_refresh_session(req); session != nullptr)
        {
            SPDLOG_INFO("log_id={}, user {} has logged in, value in session is {}", log_id, param.user_name, session->get_data<std::string>("nick_name"));
            res.set_status_and_content(status_type::ok, utils::resp(StatusCode::Success, "", fmt::format("welcome {}", session->get_data<std::string>("nick_name"))), req_content_type::json);
            return;
        }

        if (auto [user, exist] = db::t_user_info::fetch_first(log_id, {{"user_name", param.user_name}, {"password", param.password}}); exist)
        {
            std::unordered_map<std::string_view, std::any> map = {
                {"last_login_time", utils::now()},
                {"last_login_ip", utils::get_ip(req)},
                {"login_count", user.login_count + 1}};
            auto result = db::t_user_info::update(log_id, {{"user_name", param.user_name}, {"password", param.password}}, map, 1);
            if (result.affected_rows() == 1)
            {
                if (param.remember_me)
                {
                    auto session = res.start_session();
                    session->set_data("nick_name", user.nick_name);
                    session->set_max_age(1800);
                }
                SPDLOG_INFO("log_id={}, user {} has logged in", log_id, param.user_name);
                res.set_status_and_content(status_type::ok, utils::resp(StatusCode::Success, "", fmt::format("welcome {}", user.nick_name)), req_content_type::json);
                return;
            }
        }

        res.set_status_and_content(status_type::ok, utils::resp(StatusCode::FailedToLogIn, "failed to login"), req_content_type::json);
    }

    void log_out(cinatra::request &req, cinatra::response &res)
    {
        auto session = utils::check_and_refresh_session(req);
        if (session == nullptr)
        {
            res.set_status_and_content(status_type::ok, utils::resp(StatusCode::UserNotLogIn, "User has not logged in"), req_content_type::json);
            return;
        }

        auto [param, log_id, success] = utils::parse_request<dto::UserLogoutRequest>(req);
        if (!success)
        {
            SPDLOG_ERROR("log_id={}, failed to parse the parameters", log_id);
            res.set_status_and_content(status_type::ok, utils::resp(StatusCode::FailedToParseRequest, "failed to parse the parameters"), req_content_type::json);
            return;
        }
        session->set_max_age(0);
        res.set_status_and_content(status_type::ok, utils::resp(), req_content_type::json);
    }

    void remove_user(cinatra::request &req, cinatra::response &res)
    {
        auto session = utils::check_and_refresh_session(req);
        if (session == nullptr)
        {
            res.set_status_and_content(status_type::ok, utils::resp(StatusCode::UserNotLogIn, "User has not logged in"), req_content_type::json);
            return;
        }

        auto [param, log_id, success] = utils::parse_request<dto::RemoveUserRequest>(req);
        if (!success)
        {
            SPDLOG_ERROR("log_id={}, failed to parse the parameters", log_id);
            res.set_status_and_content(status_type::ok, utils::resp(StatusCode::FailedToParseRequest, "failed to parse the parameters"), req_content_type::json);
            return;
        }

        if (auto login_user_name = session->get_data<std::string>("user_name"); login_user_name != param.user_name)
        {
            SPDLOG_ERROR("log_id={}, user has no authority to do this operation", log_id);
            res.set_status_and_content(status_type::ok, utils::resp(StatusCode::OperationIsNotPermitted, "user can only delete his own account"), req_content_type::json);
            return;
        }

        if (auto result = db::t_user_info::remove(log_id, {{"user_name", param.user_name}, {"password", param.password}}); result.affected_rows() == 0)
        {
            SPDLOG_WARN("log_id={}, failed to remove the user {}", param.user_name);
            res.set_status_and_content(status_type::ok, utils::resp(StatusCode::OperationFailed, "Failed to remove the user"), req_content_type::json);
            return;
        }

        res.set_status_and_content(status_type::ok, utils::resp(), req_content_type::json);
    }

    void create_user(cinatra::request &req, cinatra::response &res)
    {
        auto [param, log_id, success] = utils::parse_request<dto::CreateUserRequest>(req);
        if (!success)
        {
            SPDLOG_ERROR("log_id={}, failed to parse the parameters", log_id);
            res.set_status_and_content(status_type::ok, utils::resp(StatusCode::FailedToParseRequest, "failed to parse the parameters"), req_content_type::json);
            return;
        }

        std::unordered_map<std::string_view, std::any> value = {
            {"user_name", param.user_name},
            {"password", param.password},
            {"true_name", param.true_name},
            {"nick_name", param.nick_name},
            {"gender", param.gender},
            {"age", param.age},
            {"email", param.email},
            {"address", param.address},
            {"phone", param.phone},
            {"relationship", param.relationship},
            {"profile_image", param.profile_image},
            {"work", param.work},
            {"interest", param.interest},
            {"create_time", utils::now()},
            {"last_login_time", utils::now()},
            {"create_ip", utils::get_ip(req)},
            {"last_login_ip", utils::get_ip(req)},
            {"login_count", 1}};

        if (auto result = db::t_user_info::insert(log_id, value); result.affected_rows() == 0)
        {
            SPDLOG_ERROR("log_id={}, failed to create the user", log_id);
            res.set_status_and_content(status_type::ok, utils::resp(StatusCode::OperationFailed, "failed to create the user"), req_content_type::json);
            return;
        }

        res.set_status_and_content(status_type::ok, utils::resp(), req_content_type::json);
    }

    void update_user_info(cinatra::request &req, cinatra::response &res)
    {
        auto session = utils::check_and_refresh_session(req);
        if (session == nullptr)
        {
            res.set_status_and_content(status_type::ok, utils::resp(StatusCode::UserNotLogIn, "User has not logged in"), req_content_type::json);
            return;
        }

        auto [param, log_id, success] = utils::parse_request<dto::UpdateUserRequest>(req);
        if (!success)
        {
            SPDLOG_ERROR("log_id={}, failed to parse the parameters", log_id);
            res.set_status_and_content(status_type::ok, utils::resp(StatusCode::FailedToParseRequest, "failed to parse the parameters"), req_content_type::json);
            return;
        }

        std::map<std::string_view, std::any> condition = {{"user_name", param.user_name}};

        std::unordered_map<std::string_view, std::any> value;
        ADD_TO_MAP_OPT(value, param, password);
        ADD_TO_MAP_OPT(value, param, true_name);
        ADD_TO_MAP_OPT(value, param, nick_name);
        ADD_TO_MAP_OPT(value, param, gender);
        ADD_TO_MAP_OPT(value, param, age);
        ADD_TO_MAP_OPT(value, param, email);
        ADD_TO_MAP_OPT(value, param, address);
        ADD_TO_MAP_OPT(value, param, phone);
        ADD_TO_MAP_OPT(value, param, relationship);
        ADD_TO_MAP_OPT(value, param, profile_image);
        ADD_TO_MAP_OPT(value, param, work);
        ADD_TO_MAP_OPT(value, param, interest);

        if (auto result = db::t_user_info::update(log_id, condition, value, 1); result.affected_rows() == 0)
        {
            SPDLOG_ERROR("log_id={}, failed to update the user", log_id);
            res.set_status_and_content(status_type::ok, utils::resp(StatusCode::OperationFailed, "failed to update the user"), req_content_type::json);
            return;
        }

        res.set_status_and_content(status_type::ok, utils::resp(), req_content_type::json);
    }

    void get_user_info(cinatra::request &req, cinatra::response &res)
    {
        auto session = utils::check_and_refresh_session(req);
        if (session == nullptr)
        {
            res.set_status_and_content(status_type::ok, utils::resp(StatusCode::UserNotLogIn, "User has not logged in"), req_content_type::json);
            return;
        }

        auto [param, log_id, success] = utils::parse_request<dto::GetUserInfoRequest>(req);
        if (!success)
        {
            SPDLOG_ERROR("log_id={}, failed to parse the parameters", log_id);
            res.set_status_and_content(status_type::ok, utils::resp(StatusCode::FailedToParseRequest, "failed to parse the parameters"), req_content_type::json);
            return;
        }

        auto [user, exist] = db::t_user_info::fetch_first(log_id, {{"user_name", param.user_name}});
        if (exist)
        {
            dto::GetUserInfoResponse resp{
                .user_name = user.user_name,
                .true_name = user.true_name,
                .nick_name = user.nick_name,
                .gender = user.gender,
                .age = user.age,
                .email = user.email,
                .address = user.address,
                .phone = user.phone,
                .relationship = user.relationship,
                .profile_image = user.profile_image,
                .work = user.work,
                .interest = user.interest};
            res.set_status_and_content(status_type::ok, utils::resp(StatusCode::Success, "", resp), req_content_type::json);
        }
        res.set_status_and_content(status_type::ok, utils::resp(StatusCode::UserNotFound, "No found the user"), req_content_type::json);
    }
} // namespace handler
