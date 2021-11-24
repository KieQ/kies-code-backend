#ifndef CONSTANT_ERROR_CODE_HPP
#define CONSTANT_ERROR_CODE_HPP

#include <cstdint>

namespace constant
{
    enum class StatusCode{
        Success = 0,
        FailedToParseRequest = 10000,
        ParameterIsWrong = 10001,

        FailedToLogIn = 10010,
        UserNotLogIn = 10011,

        OperationFailed = 10020,
        UserNotFound = 10021,
        OperationIsNotPermitted = 10023
    };
} // namespace constant



#endif