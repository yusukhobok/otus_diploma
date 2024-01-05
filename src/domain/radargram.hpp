#ifndef GPR_READER_RADARGRAM_HPP
#define GPR_READER_RADARGRAM_HPP

#include <memory>
#include <string>
#include <iostream>
#include <eigen3/Eigen/Core>

#include "attribute_analysis.hpp"
#include "depth_section.hpp"


struct Radargram {
    Eigen::MatrixXf trace_matrix;
    float delta_distance__m;
    float delta_time__ns;
    std::string gpr_unit;
    std::string antenna_name;
    float frequency__MHz;
    float antenna_distance__m;
    float velocity__m_per_ns;

    bool operator==(const Radargram& other) const {
        return trace_matrix == other.trace_matrix &&
               delta_distance__m == other.delta_distance__m &&
               delta_time__ns == other.delta_time__ns &&
               gpr_unit == other.gpr_unit &&
               antenna_name == other.antenna_name &&
               frequency__MHz == other.frequency__MHz &&
               antenna_distance__m == other.antenna_distance__m &&
               velocity__m_per_ns == other.velocity__m_per_ns;
    }
};


#endif //GPR_READER_RADARGRAM_HPP
