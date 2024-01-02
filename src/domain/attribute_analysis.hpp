#ifndef GPR_READER_ATTRIBUTE_ANALYSIS_HPP
#define GPR_READER_ATTRIBUTE_ANALYSIS_HPP

#include <eigen3/Eigen/Core>


struct AttributeAnalysis {
    bool has_attribute_analysis;
    Eigen::MatrixXf attribute_matrix;
    std::string name;
};


#endif //GPR_READER_ATTRIBUTE_ANALYSIS_HPP
