#ifndef DB_DB_MANAGER_HPP
#define DB_DB_MANAGER_HPP

#include <pqxx/pqxx>


namespace db
{
    class Database{
        static thread_local pqxx::connection conn;
        
        public:
            static pqxx::connection& get_conn();

            static std::string print_result(pqxx::result result, bool print_col_name=true);
    };
} // namespace db

#endif