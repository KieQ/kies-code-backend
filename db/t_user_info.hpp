#ifndef DB_T_USER_INFO_HPP
#define DB_T_USER_INFO_HPP

#include <string>
#include <pqxx/pqxx>
#include <unordered_map>
#include <map>
#include <any>
#include <string_view>

namespace db
{
    class t_user_info
    {
        //Data
    public:
        std::int64_t id;
        std::string user_name;
        std::string password;
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
        std::int64_t create_time;
        std::int64_t last_login_time;
        std::string create_ip;
        std::string last_login_ip;
        std::uint64_t login_count;


    private:
        static t_user_info row_to_user_info(pqxx::row res);

    public:
        static inline std::string table_name();

        static std::vector<t_user_info> fetch(std::string_view log_id, std::map<std::string_view, std::any> condition);

        static std::tuple<t_user_info, bool> fetch_first(std::string_view log_id, std::map<std::string_view, std::any> condition);

        static pqxx::result remove(std::string_view log_id, std::map<std::string_view, std::any> condition);

        static pqxx::result update(std::string_view log_id, std::map<std::string_view, std::any> condition, std::unordered_map<std::string_view, std::any> value, int limit = 0);

        static pqxx::result insert(std::string_view log_id, std::unordered_map<std::string_view, std::any> value);

        static pqxx::result insert(std::string_view log_id, std::vector<std::unordered_map<std::string_view, std::any>> value);
    };

} // namespace db

#endif