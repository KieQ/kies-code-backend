#include "t_video.hpp"
#include "db_manager.hpp"
#include <spdlog/spdlog.h>
#include "../utils/any.hpp"
#include "../utils/json.hpp"

namespace db
{
    std::string t_video::table_name()
    {
        return "t_video";
    }

    t_video t_video::row_to_video(pqxx::row res)
    {
        return t_video{
            .id = res["id"].as<std::int64_t>(),
            .video_hash = res["video_hash"].as<std::string>(),
            .video_name = res["video_name"].as<std::string>(),
            .video_size = res["video_size"].as<std::int64_t>(),
            .state = res["state"].as<std::int64_t>(),
            .added_time = res["added_time"].as<std::int64_t>(),
            .completed_time = res["completed_time"].as<std::int64_t>(),
            .video_link = res["video_link"].as<std::string>(),
            .profile_link = res["profile_link"].as<std::string>(),
            .score = res["score"].as<std::int64_t>(),
            .tags = utils::to_string_vector(res["tags"].as<std::string>())};
    }

    std::vector<t_video> t_video::fetch(std::string_view log_id, std::map<std::string_view, std::any> condition)
    {
        pqxx::work t(Database::get_conn());
        std::stringstream ss;
        ss << fmt::format(R"sql(SELECT * FROM {} )sql", table_name());
        if (condition.size() != 0)
        {
            std::string sep = " ";
            ss << R"sql(WHERE)sql";
            for (auto &&item : condition)
            {
                ss << sep << item.first << "=" << t.quote(utils::any_to_string(item.second));
                sep = " AND ";
            }
        }
        SPDLOG_INFO("log_id={}, sql={}", log_id, ss.str());
        auto result = t.exec(ss.str(), "[FETCH]");
        t.commit();

        std::vector<t_video> user;
        user.reserve(result.size() + 1);
        for (auto row : result)
        {
            user.push_back(row_to_video(row));
        }
        return user;
    }

    std::tuple<t_video, bool> t_video::fetch_first(std::string_view log_id, std::map<std::string_view, std::any> condition)
    {
        pqxx::work t(Database::get_conn());
        std::stringstream ss;
        ss << fmt::format(R"sql(SELECT * FROM {})sql", table_name());
        if (!condition.empty())
        {
            std::string sep = " ";
            ss << R"sql( WHERE)sql";
            for (auto &&item : condition)
            {
                ss << sep << item.first << "=" << t.quote(utils::any_to_string(item.second));
                sep = " AND ";
            }
        }
        ss << " LIMIT 1";

        SPDLOG_INFO("log_id={},  sql={}", log_id, ss.str());
        auto result = t.exec(ss.str(), "[FETCH FIRST]");
        t.commit();
        if (result.size() == 1)
        {
            return {row_to_video(result[0]), true};
        }
        return {t_video{}, false};
    }

    pqxx::result t_video::remove(std::string_view log_id, std::map<std::string_view, std::any> condition)
    {
        pqxx::work t(Database::get_conn());
        std::stringstream ss;
        ss << fmt::format("DELETE FROM {}", table_name());
        if (!condition.empty())
        {
            ss << " WHERE";
            std::string sep = " ";
            for (const auto &item : condition)
            {
                ss << sep << item.first << "=" << t.quote(utils::any_to_string(item.second));
                sep = " AND ";
            }
        }

        SPDLOG_INFO("log_id={}, sql={}", log_id, ss.str());
        auto result = t.exec(ss.str(), "[REMOVE]");
        t.commit();
        return result;
    }

    pqxx::result t_video::update(std::string_view log_id, std::map<std::string_view, std::any> condition, std::unordered_map<std::string_view, std::any> value, int limit)
    {
        if (condition.empty() || value.empty())
        {
            return pqxx::result();
        }
        pqxx::work t(Database::get_conn());
        std::stringstream ss;
        ss << fmt::format("UPDATE {} SET", table_name());
        std::string sep = " ";
        for (auto &&item : value)
        {
            ss << sep << item.first << "=" << t.quote(utils::any_to_string(item.second));
            sep = ", ";
        }

        ss << " WHERE";
        sep = " ";
        for (auto &&item : condition)
        {
            ss << sep << item.first << "=" << t.quote(utils::any_to_string(item.second));
            sep = " AND ";
        }
        SPDLOG_INFO("log_id={}, sql={}", log_id, ss.str());
        auto result = t.exec(ss.str(), "[UPDATE]");
        t.commit();
        return result;
    }

    pqxx::result t_video::insert(std::string_view log_id, std::unordered_map<std::string_view, std::any> value)
    {
        if (value.empty())
        {
            return pqxx::result{};
        }
        pqxx::work t(Database::get_conn());
        std::stringstream ss;
        ss << fmt::format("INSERT INTO {}(", table_name());
        std::string sep = "";
        for (auto &&item : value)
        {
            ss << sep << item.first;
            sep = ", ";
        }
        ss << ") VALUES(";
        sep = "";
        for (auto &&item : value)
        {
            ss << sep << t.quote(utils::any_to_string(item.second));
            sep = ", ";
        }
        ss << ")";
        SPDLOG_INFO("log_id={}, sql={}", log_id, ss.str());
        auto result = t.exec(ss.str(), "[INSERT]");
        t.commit();
        return result;
    }

    pqxx::result t_video::insert(std::string_view log_id, std::vector<std::unordered_map<std::string_view, std::any>> value)
    {
        if (value.empty())
        {
            return pqxx::result{};
        }
        pqxx::work t(Database::get_conn());
        std::stringstream ss;
        ss << fmt::format("INSERT INTO {}(", table_name());
        std::string sep = "";
        for (auto &&item : value[0])
        {
            ss << sep << item.first;
            sep = ", ";
        }
        ss << ") VALUES";
        sep = "";
        for (auto &&item : value)
        {
            ss << sep << "(";
            std::string sep_ = "";
            for (auto &&v : item)
            {
                ss << sep_ << t.quote(utils::any_to_string(v.second));
                sep_ = ", ";
            }
            ss << ")";
            sep = ", ";
        }
        SPDLOG_INFO("log_id={}, sql={}", log_id, ss.str());
        auto result = t.exec(ss.str(), "[BATCH INSERT]");
        t.commit();
        return result;
    }

} // namespace db
