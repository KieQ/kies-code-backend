#ifndef UTILS_LOG_HPP
#define UTILS_LOG_HPP

#include <string_view>
#include <cinatra.hpp>

namespace utils
{
    std::string_view req_id(const cinatra::request& req);

} // namespace utils


#endif