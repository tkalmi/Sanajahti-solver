#include <gtest/gtest.h> //GoogleTest libraries

GTEST_API_ int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
