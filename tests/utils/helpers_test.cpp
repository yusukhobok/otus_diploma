#include <gtest/gtest.h>
#include "src/utils/helpers.hpp"


TEST(
        HelperTests,
        given_epsilon__when_epsilon_to_velocity__then_correct_velocity_returned
) {
    double epsilon = 9;

    double velocity = epsilon_to_velocity(epsilon);

    EXPECT_DOUBLE_EQ(velocity, 0.1);
}


TEST(
        HelperTests,
        given_velocity__when_velocity_to_epsilon__then_correct_epsion_returned
) {
    double velocity = 0.1;

    double epsilon = velocity_to_epsilon(velocity);

    EXPECT_DOUBLE_EQ(epsilon, 9);
}
