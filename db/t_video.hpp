#ifndef DB_T_VIDEO_HPP
#define DB_T_VIDEO_HPP

#include <string>
#include <string_view>
#include <pqxx/pqxx>
#include <vector>
#include <map>
#include <unordered_map>
#include <any>

namespace db
{
        class t_video{
        //Data
        public:
        std::int64_t id;
        std::string video_hash;
        std::string video_name;
        std::int64_t video_size;
        std::int64_t state;
        std::int64_t added_time;
        std::int64_t completed_time;
        std::string video_link;
        std::string profile_link;
        std::int64_t score;
        std::vector<std::string> tags;

        private:
        static t_video row_to_video(pqxx::row res);


        public:
        static inline std::string table_name();

        static std::vector<t_video> fetch(std::string_view log_id,std::map<std::string_view, std::any> condition);

        static std::tuple<t_video,bool> fetch_first(std::string_view log_id,std::map<std::string_view, std::any> condition);

        static pqxx::result remove(std::string_view log_id,std::map<std::string_view, std::any> condition);

        static pqxx::result update(std::string_view log_id,std::map<std::string_view, std::any> condition, std::unordered_map<std::string_view, std::any> value, int limit=0);

        static pqxx::result insert(std::string_view log_id,std::unordered_map<std::string_view, std::any> value);

        static pqxx::result insert(std::string_view log_id,std::vector<std::unordered_map<std::string_view, std::any>> value);
        
    };
} // namespace db


#endif