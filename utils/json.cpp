#include "json.hpp"
#include <nlohmann/json.hpp>

namespace utils
{
    std::string resp(constant::StatusCode status_code, std::string_view status_message, nlohmann::json data)
    {
        nlohmann::json j;
        j["status_code"] = status_code;
        j["status_message"] = status_message;
        j["data"] = data;
        return j.dump(4);
    }

    template <nlohmann::json::value_t... type>
    inline bool all_type(const nlohmann::json &data, std::initializer_list<std::string> names)
    {
        return std::all_of(names.begin(), names.end(), [&data](const std::string &name)
                           { return data.contains(name) && ((data[name].type() == type) || ...); });
    }

    bool all_string(const nlohmann::json &data, std::initializer_list<std::string> names)
    {
        return all_type<nlohmann::json::value_t::string>(data, names);
    }

    bool all_number(const nlohmann::json &data, std::initializer_list<std::string> names)
    {
        return all_type<nlohmann::json::value_t::number_float, nlohmann::json::value_t::number_integer, nlohmann::json::value_t::number_unsigned>(data, names);
    }

    bool all_ingeter(const nlohmann::json &data, std::initializer_list<std::string> names)
    {
        return all_type<nlohmann::json::value_t::number_integer, nlohmann::json::value_t::number_unsigned>(data, names);
    }

    bool all_float(const nlohmann::json &data, std::initializer_list<std::string> names)
    {
        return all_type<nlohmann::json::value_t::number_float>(data, names);
    }

    bool all_boolean(const nlohmann::json &data, std::initializer_list<std::string> names)
    {
        return all_type<nlohmann::json::value_t::boolean>(data, names);
    }

    std::string get_string(const nlohmann::json &data, std::string name, std::string default_value)
    {
        if (data.contains(name) && data[name].is_string())
        {
            return data[name].get<std::string>();
        }
        return default_value;
    }

    std::optional<std::string> get_string(const nlohmann::json &data, std::string name)
    {
        if (data.contains(name) && data[name].is_string())
        {
            return data[name].get<std::string>();
        }
        return std::nullopt;
    }

    long long get_number(const nlohmann::json &data, std::string name, long long default_value)
    {
        if (data.contains(name) && data[name].is_number())
        {
            return data[name].get<long long>();
        }
        return default_value;
    }

    std::optional<long long> get_number(const nlohmann::json &data, std::string name)
    {
        if (data.contains(name) && data[name].is_number())
        {
            return data[name].get<long long>();
        }
        return std::nullopt;
    }

    int get_integer(const nlohmann::json &data, std::string name, int default_value)
    {
        if (data.contains(name) && data[name].is_number_integer())
        {
            return data[name].get<int>();
        }
        return default_value;
    }

    std::optional<int> get_integer(const nlohmann::json &data, std::string name)
    {
        if (data.contains(name) && data[name].is_number_integer())
        {
            return data[name].get<int>();
        }
        return std::nullopt;
    }

    double get_float(const nlohmann::json &data, std::string name, double default_value)
    {
        if (data.contains(name) && data[name].is_number_float())
        {
            return data[name].get<double>();
        }
        return default_value;
    }

    std::optional<double> get_float(const nlohmann::json &data, std::string name)
    {
        if (data.contains(name) && data[name].is_number_float())
        {
            return data[name].get<double>();
        }
        return std::nullopt;
    }

    bool get_boolean(const nlohmann::json &data, std::string name, bool default_value)
    {
        if (data.contains(name) && data[name].is_boolean())
        {
            return data[name].get<bool>();
        }
        return default_value;
    }

    std::optional<bool> get_boolean(const nlohmann::json &data, std::string name)
    {
        if (data.contains(name) && data[name].is_boolean())
        {
            return data[name].get<bool>();
        }
        return std::nullopt;
    }

    std::vector<std::string> to_string_vector(const std::string_view data)
    {
        if (!nlohmann::json::accept(data))
        {
            return {};
        }
        auto j = nlohmann::json::parse(data);
        if (!j.is_array())
        {
            return {};
        }

        std::vector<std::string> content;
        content.reserve(j.size());
        for (const auto &item : j)
        {
            if (!item.is_string())
            {
                return {};
            }
            content.push_back(item.get<std::string>());
        }
        return content;
    }

    std::unordered_map<std::string_view, std::any> retrieve_if_exist(const nlohmann::json &data, std::initializer_list<std::string_view> names)
    {
        std::unordered_map<std::string_view, std::any> result;
        for (auto name_view : names)
        {
            if (!data.contains(name_view))
            {
                continue;
            }
            std::string name = std::string(name_view);
            if (data[name].is_string())
            {
                result.insert({name_view, data[name].get<std::string>()});
            }
            else if (data[name].is_number_float())
            {
                result.insert({name_view, data[name].get<double>()});
            }
            else if (data[name].is_number())
            {
                result.insert({name_view, data[name].get<long long>()});
            }
            else if (data[name].is_boolean())
            {
                result.insert({name_view, data[name].get<bool>()});
            }
        }
        return result;
    }

    std::tuple<std::string,bool> to_string(const nlohmann::json& data){
        if(data.is_null()){
            return {"null", true};
        }else if(data.is_number_integer()){
            return {std::to_string(data.get<std::int64_t>()), true};
        }else if(data.is_number_unsigned()){
            return {std::to_string(data.get<std::uint64_t>()), true};
        }else if(data.is_boolean()){
            return {data.get<bool>()?"true":"false", true};
        }else if(data.is_number_float()){
            return {std::to_string(data.get<double>()), true};
        }else if(data.is_string()){
            return {data.get<std::string>(), true};
        }else{
            return {"", false};
        }
    }

} // namespace utils
