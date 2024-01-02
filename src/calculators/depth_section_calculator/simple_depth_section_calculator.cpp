#include "simple_depth_section_calculator.h"
#include <eigen3/Eigen/Core>
#include <cmath>

DepthSection SimpleDepthSectionCalculator::calculate(const Radargram& radargram) {
    int sample_count = radargram.trace_matrix.cols();
    float time_max = sample_count * radargram.delta_time__ns;
    Eigen::VectorXf time_vector = Eigen::VectorXf::LinSpaced(radargram.trace_matrix.cols(), 0, time_max);
    Eigen::VectorXf depth_vector = time_vector.array().pow(2) * pow(radargram.velocity__m_per_ns, 2) - pow(radargram.antenna_distance__m, 2);
    for (auto& el : depth_vector) {
        if (el < 0) {
            el = 0;
        }
    }
    depth_vector = depth_vector.array().sqrt() / 2;
    return DepthSection{.has_depth_section=true, .depth_vector = depth_vector};
}
