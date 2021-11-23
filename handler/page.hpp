#ifndef HANDLER_PAGE_HPP
#define HANDLER_PAGE_HPP

#include <cinatra.hpp>

namespace handler
{

    void home_page(cinatra::request &req, cinatra::response &res);

    void not_found_page(cinatra::request &req, cinatra::response &res);

} // namespace handler

#endif