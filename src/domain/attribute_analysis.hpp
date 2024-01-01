#ifndef GPR_READER_ATTRIBUTE_ANALYSIS_HPP
#define GPR_READER_ATTRIBUTE_ANALYSIS_HPP

#include <eigen3/Eigen/Core>

using Eigen::MatrixXd;

struct AttributeAnalysis {
    MatrixXd attribute_matrix;
    std::string name;
};


#endif //GPR_READER_ATTRIBUTE_ANALYSIS_HPP
