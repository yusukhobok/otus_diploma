#include <gtest/gtest.h>
#include <iostream>
#include "../src/domain/radargram.h"

TEST(
        ExampleTests,
        given_when_then
) {
    Radargram r(10, 20);
    std::cout << r.y << std::endl;

    ASSERT_EQ(r.x, 10);
}
