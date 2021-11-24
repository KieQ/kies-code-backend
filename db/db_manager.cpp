#include "db_manager.hpp"
#include <spdlog/spdlog.h>


#include <thread>

namespace db
{
    pqxx::connection &Database::get_conn()
    {
        return conn;
    }

    std::string Database::print_result(pqxx::result result, bool print_col_name)
    {
        if(result.size()==0){
            return "<result is empty>";
        }
        std::stringstream ss;
        if (print_col_name)
        {
            auto row = result[0];
            for(const auto& col: row){
                ss << col.name()<<"\t";
            }
            ss<<"\n";
        }

        for (auto row : result)
        {
            for(auto col : row){
                ss<<col.c_str()<<"\t";
            }
            ss<<"\n";
        }
        return ss.str();
    }

    // Initialize the connection, if failed, the program should crash
    thread_local pqxx::connection Database::conn = []() noexcept
    {
        auto psql_str = std::getenv("DATABASE_URL");
        if (psql_str == nullptr)
        {
            SPDLOG_ERROR("environment variable DATABASE_URL needed");
            std::terminate();
        }
        pqxx::connection conn{psql_str};
        if (!conn.is_open())
        {
            SPDLOG_ERROR("connection does not open");
            std::terminate();
        }
        SPDLOG_INFO("Success to connect to database");
        return conn;
    }();

} // namespace db
