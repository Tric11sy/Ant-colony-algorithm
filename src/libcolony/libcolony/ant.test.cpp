#include <gtest/gtest.h>
#include <libcolony/ant.hpp>

TEST(TestFunc, Func) {
    int test_number = 10;
    const int expected_number = 10;

    EXPECT_EQ(test_number, expected_number);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
