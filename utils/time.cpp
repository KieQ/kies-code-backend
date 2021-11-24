#include "time.hpp"

#include <chrono>

namespace utils
{
    int now(){
        return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }
} // namespace utils
