#include "json.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace utils;

TEST(JSON_TEST, JSON_TEST_1)
{
    std::string_view s = "[1,2,3]";
    auto j = nlohmann::json::parse(s, nullptr, false);
    if(j.is_discarded()){
        std::cout<<"Fail"<<std::endl;
    }else{
        std::cout<<"Success"<<std::endl;
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}