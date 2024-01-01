#ifndef GPR_READER_HELPERS_HPP
#define GPR_READER_HELPERS_HPP

#include <cmath>
#include <cassert>

const double AIR_VELOCITY__m_per_ns = 0.3;

double epsilon_to_velocity(double epsilon) {
    assert(epsilon > 0);
    return AIR_VELOCITY__m_per_ns / sqrt(epsilon);
}

double velocity_to_epsilon(double velocity__m_per_ns) {
    assert(velocity__m_per_ns > 0);
    assert(velocity__m_per_ns <= AIR_VELOCITY__m_per_ns);
    return pow((AIR_VELOCITY__m_per_ns / velocity__m_per_ns), 2);
}


#endif //GPR_READER_HELPERS_HPP
