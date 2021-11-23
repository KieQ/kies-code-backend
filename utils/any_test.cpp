#include "any.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(ANY_TEST, ANY_TEST_1)
{
    using ::testing::HasSubstr;
    using ::testing::MatchesRegex;
    using namespace std;

    EXPECT_EQ("123", utils::any_to_string(123));
    EXPECT_EQ("hello", utils::any_to_string("hello"));
    EXPECT_EQ("hello", utils::any_to_string("hello"s));
    EXPECT_EQ("true", utils::any_to_string(true));
    EXPECT_EQ("false", utils::any_to_string(false));
    EXPECT_THAT(utils::any_to_string(1.2), HasSubstr("unsupported type"));
    EXPECT_THAT(utils::any_to_string('a'), HasSubstr("unsupported type"));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}