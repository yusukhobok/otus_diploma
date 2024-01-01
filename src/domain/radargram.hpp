#ifndef GPR_READER_RADARGRAM_HPP
#define GPR_READER_RADARGRAM_HPP

#include <memory>
#include <string>
#include <iostream>
#include <eigen3/Eigen/Core>

#include "attribute_analysis.hpp"
#include "depth_section.hpp"
#include "trajectory.hpp"

using Eigen::MatrixXd;


struct Radargram {
    MatrixXd trace_matrix;
    float delta_distance__m;
    float delta_time__ns;
    std::string gpr_unit;
    std::string antenna_name;
    float frequency__MHz;
    float antenna_distance__m;
    float velocity__m_per_ns;
};


#endif //GPR_READER_RADARGRAM_HPP
