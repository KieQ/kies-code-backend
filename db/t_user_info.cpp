#include "t_user_info.hpp"
#include "db_manager.hpp"
#include <spdlog/spdlog.h>
#include "../utils/any.hpp"

#include <vector>

namespace db
{
        std::string t_user_info::table_name()
        {
                return "t_user_info";
        }

        t_user_info t_user_info::row_to_user_info(pqxx::row res)
        {
                return t_user_info{
                    .id = res["id"].as<std::int64_t>(),
                    .user_name = res["user_name"].as<std::string>(),
                    .password = res["password"].as<std::string>(),
                    .true_name = res["true_name"].as<std::string>(),
                    .nick_name = res["nick_name"].as<std::string>(),
                    .gender = res["gender"].as<std::string>(),
                    .age = res["age"].as<std::uint16_t>(),
                    .email = res["email"].as<std::string>(),
                    .address = res["address"].as<std::string>(),
                    .phone = res["phone"].as<std::string>(),
                    .relationship = res["relationship"].as<std::string>(""),
                    .profile_image = res["profile_image"].as<std::string>(""),
                    .work = res["work"].as<std::string>(""),
                    .interest = res["interest"].as<std::string>(""),
                    .create_time = res["create_time"].as<std::int64_t>(),
                    .last_login_time = res["last_login_time"].as<std::int64_t>(),
                    .create_ip = res["create_ip"].as<std::string>(),
                    .last_login_ip = res["last_login_ip"].as<std::string>(),
                    .login_count = res["login_count"].as<std::uint64_t>(),
                };
        }

        std::vector<t_user_info> t_user_info::fetch(std::string_view log_id, std::map<std::string_view, std::any> condition)
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

                std::vector<t_user_info> user;
                user.reserve(result.size() + 1);
                for (auto row : result)
                {
                        user.push_back(row_to_user_info(row));
                }
                return user;
        }

        std::tuple<t_user_info, bool> t_user_info::fetch_first(std::string_view log_id, std::map<std::string_view, std::any> condition)
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

                SPDLOG_INFO("log_id={}, sql={}", log_id, ss.str());
                auto result = t.exec(ss.str(), "[FETCH FIRST]");
                t.commit();
                if (result.size() == 1)
                {
                        return {row_to_user_info(result[0]), true};
                }
                return {t_user_info{}, false};
        }

        pqxx::result t_user_info::remove(std::string_view log_id, std::map<std::string_view, std::any> condition)
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

        pqxx::result t_user_info::update(std::string_view log_id, std::map<std::string_view, std::any> condition, std::unordered_map<std::string_view, std::any> value, int limit)
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

        pqxx::result t_user_info::insert(std::string_view log_id, std::unordered_map<std::string_view, std::any> value)
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

        pqxx::result t_user_info::insert(std::string_view log_id, std::vector<std::unordered_map<std::string_view, std::any>> value)
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
