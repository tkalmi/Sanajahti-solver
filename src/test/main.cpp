#include <gtest/gtest.h> //GoogleTest libraries

/*
 * Main function that calls the tests
 * Returns 0 if all tests are passed and 1 if some or all tests didn't pass
 */
GTEST_API_ int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
