#include "json.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace utils;

TEST(JSON_TEST, JSON_TEST_1)
{

    nlohmann::json data = R"json(
        {
            "integer_a":123,
            "integer_b":-123,
            "float_a":1.2,
            "float_b":-1.2,
            "string_a":"hello",
            "string_b":"",
            "string_c":"yes",
            "boolean_a":true,
            "boolean_b":false
        }
    )json"_json;

    EXPECT_TRUE(all_boolean(data, {}));
    EXPECT_TRUE(all_boolean(data, {"boolean_a","boolean_b"}));
    
    EXPECT_FALSE(all_boolean(data, {"boolean_a","integer_a"}));
    EXPECT_FALSE(all_boolean(data, {"boolean_a","no_exist"}));
    EXPECT_FALSE(all_boolean(data, {"boolean_a","string_b"}));
    EXPECT_FALSE(all_boolean(data, {"boolean_b","integer_a"}));
    EXPECT_FALSE(all_boolean(data, {"boolean_b","no_exist"}));
    EXPECT_FALSE(all_boolean(data, {"boolean_b","string_b"}));

    EXPECT_TRUE(all_float(data, {}));
    EXPECT_TRUE(all_float(data, {"float_a", "float_b"}));

    EXPECT_FALSE(all_float(data, {"float_a", "integer_a"}));
    EXPECT_FALSE(all_float(data, {"float_a","no_exist"}));
    EXPECT_FALSE(all_float(data, {"float_a","string_b"}));
    EXPECT_FALSE(all_float(data, {"float_b", "integer_a"}));
    EXPECT_FALSE(all_float(data, {"float_b","no_exist"}));
    EXPECT_FALSE(all_float(data, {"float_b","string_b"}));


    EXPECT_TRUE(all_ingeter(data, {}));
    EXPECT_TRUE(all_ingeter(data, {"integer_a", "integer_b"}));

    EXPECT_FALSE(all_ingeter(data, {"integer_a", "float_a"}));
    EXPECT_FALSE(all_ingeter(data, {"integer_a","no_exist"}));
    EXPECT_FALSE(all_ingeter(data, {"integer_a","string_b"}));
    EXPECT_FALSE(all_ingeter(data, {"integer_a", "float_a"}));
    EXPECT_FALSE(all_ingeter(data, {"integer_a","no_exist"}));
    EXPECT_FALSE(all_ingeter(data, {"integer_a","string_b"}));


    EXPECT_TRUE(all_number(data, {}));
    EXPECT_TRUE(all_number(data, {"integer_a", "integer_b", "float_a", "float_b"}));
    
    EXPECT_FALSE(all_number(data, {"integer_a", "integer_b", "float_a", "float_b","boolean_a"}));

    EXPECT_TRUE(all_string(data, {}));
    EXPECT_TRUE(all_string(data, {"string_a", "string_b", "string_c"}));
    
    EXPECT_FALSE(all_string(data, {"string_a", "string_b", "float_a", "float_b","boolean_a"}));


}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}