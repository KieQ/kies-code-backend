#ifndef UTILS_FILE_HPP
#define UTILS_FILE_HPP

#include <string>
#include <string_view>

namespace utils
{
       
std::string read_template(std::string_view file_path);

std::string read_binary(std::string_view file_path);



}


#endif