#include <gtest/gtest.h> //GoogleTest libraries
#include <iostream>
#include <vector>

/*
 * TODO:
 * make the tests
 */
TEST(TESTNAME, WHATTOTEST){ //Replace TESTNAME and WHATTOTEST with the correct equivalents
    EXPECT_EQ(2,2); // Replace this
    EXPECT_EQ("anything", "anything"); // Replace this
    EXPECT_TRUE(true); // Replace this
}

GTEST_API_ int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
