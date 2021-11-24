#include "log.hpp"

namespace utils
{
    std::string_view req_id(const cinatra::request& req){
        return req.get_header_value("X-Request-ID");
    }

} // namespace utils
