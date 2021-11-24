#include "file.hpp"
#include <fstream>
#include <iostream>

namespace utils
{

    std::string read_template(std::string_view file_path)
    {
        std::fstream file(file_path.data(), std::ios::in);
        return std::string(std::istreambuf_iterator<char>{file}, {});
    }

    std::string read_binary(std::string_view file_path){
        std::fstream file(file_path.data(), std::ios::in|std::ios::binary);
        return std::string(std::istreambuf_iterator<char>{file}, {});
    }

}