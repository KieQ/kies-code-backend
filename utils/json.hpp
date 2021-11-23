#ifndef UTILS_JSON_HPP
#define UTILS_JSON_HPP

#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <tuple>
#include <any>
#include <optional>
#include <nlohmann/json.hpp>


#include "../constant/error_code.hpp"


namespace utils
{
    std::string resp(constant::StatusCode status_code = constant::StatusCode::Success, std::string_view status_message = "", nlohmann::json data = nlohmann::json());

    bool all_string(const nlohmann::json &data, std::initializer_list<std::string> names);

    bool all_number(const nlohmann::json &data, std::initializer_list<std::string> names);

    bool all_ingeter(const nlohmann::json &data, std::initializer_list<std::string> names);

    bool all_float(const nlohmann::json &data, std::initializer_list<std::string> names);

    bool all_boolean(const nlohmann::json &data, std::initializer_list<std::string> names);

    std::string get_string(const nlohmann::json &data, std::string name, std::string default_value);

    std::optional<std::string> get_string(const nlohmann::json& data, std::string name);

    long long get_number(const nlohmann::json &data, std::string name, long long default_value);

    std::optional<long long> get_number(const nlohmann::json& data, std::string name);

    int get_integer(const nlohmann::json &data, std::string name, int default_value);

    std::optional<int> get_integer(const nlohmann::json &data, std::string name);

    double get_float(const nlohmann::json &data, std::string name, double default_value);

    std::optional<double> get_float(const nlohmann::json &data, std::string name);

    bool get_boolean(const nlohmann::json &data, std::string name, bool default_value);

    std::optional<bool> get_boolean(const nlohmann::json &data, std::string name);

    std::vector<std::string> to_string_vector(const std::string_view data);

    std::unordered_map<std::string_view, std::any> retrieve_if_exist(const nlohmann::json& data, std::initializer_list<std::string_view> names);

    std::tuple<std::string,bool> to_string(const nlohmann::json& data);
} // namespace utils

#endif