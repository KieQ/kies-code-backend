#ifndef DB_T_BLOG_HPP
#define DB_T_BLOG_HPP

#include <string>
#include <string_view>
#include <pqxx/pqxx>
#include <vector>
#include <map>
#include <unordered_map>
#include <any>

namespace db
{
        class t_blog{
        //Data
        public:
        std::int64_t id;
        std::int64_t user_id;
        std::string content;
        std::int64_t create_time;
        std::int64_t update_time;
        std::int64_t modified_count;
        std::string title;
        std::string sub_title;
        std::vector<std::string> tags;
        std::vector<std::string> images;

        private:
        static t_blog row_to_blog(pqxx::row res);


        public:
        static inline std::string table_name();

        static std::vector<t_blog> fetch(std::string_view log_id,std::map<std::string_view, std::any> condition);

        static std::tuple<t_blog,bool> fetch_first(std::string_view log_id,std::map<std::string_view, std::any> condition);

        static pqxx::result remove(std::string_view log_id,std::map<std::string_view, std::any> condition);

        static pqxx::result update(std::string_view log_id,std::map<std::string_view, std::any> condition, std::unordered_map<std::string_view, std::any> value, int limit=0);

        static pqxx::result insert(std::string_view log_id,std::unordered_map<std::string_view, std::any> value);

        static pqxx::result insert(std::string_view log_id,std::vector<std::unordered_map<std::string_view, std::any>> value);
        
    };
} // namespace db


#endif