#ifndef DTO_USER_HPP
#define DTO_USER_HPP

#include <string>
#include <nlohmann/json.hpp>

#include "macro.hpp"

namespace dto
{

    // User login
    struct UserLogInRequest
    {
        std::string user_name;
        std::string password;
        bool remember_me;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(UserLogInRequest, user_name, password, remember_me)
    };

    // User logout
    struct UserLogoutRequest
    {
        std::string user_name;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(UserLogoutRequest, user_name)
    };

    // Remove user
    struct RemoveUserRequest
    {
        std::string user_name;
        std::string password;
        bool remove_immediately;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(RemoveUserRequest, user_name, password, remove_immediately)
    };

    // Creaet User
    struct CreateUserRequest
    {
        std::string user_name;
        std::string password;
        std::string true_name;
        std::string nick_name;
        std::string gender;
        std::uint8_t age;
        std::string email;
        std::string address;
        std::string phone;
        std::string relationship;
        std::string profile_image;
        std::string work;
        std::string interest;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CreateUserRequest,
                                       user_name,
                                       password,
                                       true_name,
                                       nick_name,
                                       gender,
                                       age,
                                       email,
                                       address,
                                       phone,
                                       relationship,
                                       profile_image,
                                       work,
                                       interest)
    };

    // Update user information
    struct UpdateUserRequest
    {
        std::string user_name;
        std::optional<std::string> password;
        std::optional<std::string> true_name;
        std::optional<std::string> nick_name;
        std::optional<std::string> gender;
        std::optional<std::uint8_t> age;
        std::optional<std::string> email;
        std::optional<std::string> address;
        std::optional<std::string> phone;
        std::optional<std::string> relationship;
        std::optional<std::string> profile_image;
        std::optional<std::string> work;
        std::optional<std::string> interest;

        friend void from_json(const nlohmann::json &j, UpdateUserRequest &u)
        {
            FROM_JSON_STR(j, u, user_name, "");
            FROM_JSON_STR_OPT(j, u, password);
            FROM_JSON_STR_OPT(j, u, true_name);
            FROM_JSON_STR_OPT(j, u, nick_name);
            FROM_JSON_STR_OPT(j, u, gender);
            FROM_JSON_INT_OPT(j, u, age);
            FROM_JSON_STR_OPT(j, u, email);
            FROM_JSON_STR_OPT(j, u, address);
            FROM_JSON_STR_OPT(j, u, phone);
            FROM_JSON_STR_OPT(j, u, relationship);
            FROM_JSON_STR_OPT(j, u, profile_image);
            FROM_JSON_STR_OPT(j, u, work);
            FROM_JSON_STR_OPT(j, u, interest);
        }

        friend void to_json(nlohmann::json &j, const UpdateUserRequest &u)
        {
            TO_JSON(j, u, user_name);
            TO_JSON_OPT_WITH_DEFAULT(j, u, password, "");
            TO_JSON_OPT_WITH_DEFAULT(j, u, true_name, "");
            TO_JSON_OPT_WITH_DEFAULT(j, u, nick_name, "");
            TO_JSON_OPT_WITH_DEFAULT(j, u, gender, "");
            TO_JSON_OPT_WITH_DEFAULT(j, u, age, 0);
            TO_JSON_OPT_WITH_DEFAULT(j, u, email, "");
            TO_JSON_OPT_WITH_DEFAULT(j, u, address, "");
            TO_JSON_OPT_WITH_DEFAULT(j, u, phone, "");
            TO_JSON_OPT_WITH_DEFAULT(j, u, relationship, "");
            TO_JSON_OPT_WITH_DEFAULT(j, u, profile_image, "");
            TO_JSON_OPT_WITH_DEFAULT(j, u, work, "");
            TO_JSON_OPT_WITH_DEFAULT(j, u, interest, "");
        }
    };

    // Get user information
    struct GetUserInfoRequest
    {
        std::string user_name;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(GetUserInfoRequest, user_name);
    };

    struct GetUserInfoResponse
    {
        std::string user_name;
        std::string true_name;
        std::string nick_name;
        std::string gender;
        std::uint16_t age;
        std::string email;
        std::string address;
        std::string phone;
        std::string relationship;
        std::string profile_image;
        std::string work;
        std::string interest;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(GetUserInfoResponse,
                                       user_name,
                                       true_name,
                                       nick_name,
                                       gender,
                                       age,
                                       email,
                                       address,
                                       phone,
                                       relationship,
                                       profile_image,
                                       work,
                                       interest);
    };

} // namespace dto

#endif