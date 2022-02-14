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
            utils::on_json(log_id, res, utils::wrap(StatusCode::FailedToParseRequest, "failed to parse the parameters"));
            return;
        }

        if (auto session = utils::check_and_refresh_session(req); session != nullptr)
        {
            SPDLOG_INFO("log_id={}, user {} has logged in, value in session is {}", log_id, param.user_name, session->get_data<std::string>("nick_name"));
            utils::on_json(log_id, res, utils::wrap(StatusCode::Success, "", fmt::format("welcome {}", session->get_data<std::string>("nick_name"))));
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
                utils::on_json(log_id, res, utils::wrap(StatusCode::Success, "", fmt::format("welcome {}", user.nick_name)));
                return;
            }
        }

        utils::on_json(log_id, res, utils::wrap(StatusCode::FailedToLogIn, "failed to login"));
    }

    void log_out(cinatra::request &req, cinatra::response &res)
    {
        auto [param, log_id, success] = utils::parse_request<dto::UserLogoutRequest>(req);
        if (!success)
        {
            SPDLOG_ERROR("log_id={}, failed to parse the parameters", log_id);
            utils::on_json(log_id, res, utils::wrap(StatusCode::FailedToParseRequest, "failed to parse the parameters"));
            return;
        }
        
        auto session = utils::check_and_refresh_session(req);
        if (session == nullptr)
        {
            utils::on_json(log_id, res, utils::wrap(StatusCode::UserNotLogIn, "User has not logged in"));
            return;
        }

        session->set_max_age(0);
        utils::on_json(log_id, res, utils::wrap());
    }

    void remove_user(cinatra::request &req, cinatra::response &res)
    {

        auto [param, log_id, success] = utils::parse_request<dto::RemoveUserRequest>(req);
        if (!success)
        {
            SPDLOG_ERROR("log_id={}, failed to parse the parameters", log_id);
            utils::on_json(log_id, res, utils::wrap(StatusCode::FailedToParseRequest, "failed to parse the parameters"));
            return;
        }
    
        auto session = utils::check_and_refresh_session(req);
        if (session == nullptr)
        {
            utils::on_json(log_id, res, utils::wrap(StatusCode::UserNotLogIn, "User has not logged in"));
            return;
        }

        if (auto login_user_name = session->get_data<std::string>("user_name"); login_user_name != param.user_name)
        {
            SPDLOG_ERROR("log_id={}, user has no authority to do this operation", log_id);
            utils::on_json(log_id, res, utils::wrap(StatusCode::OperationIsNotPermitted, "user can only delete his own account"));
            return;
        }

        if (auto result = db::t_user_info::remove(log_id, {{"user_name", param.user_name}, {"password", param.password}}); result.affected_rows() == 0)
        {
            SPDLOG_WARN("log_id={}, failed to remove the user {}", param.user_name);
            utils::on_json(log_id, res, utils::wrap(StatusCode::OperationFailed, "Failed to remove the user"));
            return;
        }

        utils::on_json(log_id, res, utils::wrap());
    }

    void create_user(cinatra::request &req, cinatra::response &res)
    {
        auto [param, log_id, success] = utils::parse_request<dto::CreateUserRequest>(req);
        if (!success)
        {
            SPDLOG_ERROR("log_id={}, failed to parse the parameters", log_id);
            utils::on_json(log_id, res, utils::wrap(StatusCode::FailedToParseRequest, "failed to parse the parameters"));
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
            utils::on_json(log_id, res, utils::wrap(StatusCode::OperationFailed, "failed to create the user"));
            return;
        }

        utils::on_json(log_id, res, utils::wrap());
    }

    void update_user_info(cinatra::request &req, cinatra::response &res)
    {
        auto [param, log_id, success] = utils::parse_request<dto::UpdateUserRequest>(req);
        if (!success)
        {
            SPDLOG_ERROR("log_id={}, failed to parse the parameters", log_id);
            utils::on_json(log_id, res, utils::wrap(StatusCode::FailedToParseRequest, "failed to parse the parameters"));
            return;
        }

        auto session = utils::check_and_refresh_session(req);
        if (session == nullptr)
        {
            utils::on_json(log_id, res, utils::wrap(StatusCode::UserNotLogIn, "User has not logged in"));
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
            utils::on_json(log_id, res, utils::wrap(StatusCode::OperationFailed, "failed to update the user"));
            return;
        }

        utils::on_json(log_id, res, utils::wrap());
    }

    void get_user_info(cinatra::request &req, cinatra::response &res)
    {
        auto [param, log_id, success] = utils::parse_request<dto::GetUserInfoRequest>(req);
        if (!success)
        {
            SPDLOG_ERROR("log_id={}, failed to parse the parameters", log_id);
            utils::on_json(log_id, res, utils::wrap(StatusCode::FailedToParseRequest, "failed to parse the parameters"));
            return;
        }

        auto session = utils::check_and_refresh_session(req);
        if (session == nullptr)
        {
            utils::on_json(log_id, res, utils::wrap(StatusCode::UserNotLogIn, "User has not logged in"));
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
            utils::on_json(log_id, res, utils::wrap(StatusCode::Success, "", resp));
        }
        utils::on_json(log_id, res, utils::wrap(StatusCode::UserNotFound, "No found the user"));
    }
} // namespace handler
