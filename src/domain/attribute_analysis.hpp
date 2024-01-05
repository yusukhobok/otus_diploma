#ifndef GPR_READER_ATTRIBUTE_ANALYSIS_HPP
#define GPR_READER_ATTRIBUTE_ANALYSIS_HPP

#include <eigen3/Eigen/Core>


struct AttributeAnalysis {
    bool has_attribute_analysis;
    Eigen::MatrixXf attribute_matrix;
    std::string name;

    bool operator==(const AttributeAnalysis &other) const {
        return has_attribute_analysis == other.has_attribute_analysis && attribute_matrix == other.attribute_matrix &&
               name == other.name;
    }
};


#endif //GPR_READER_ATTRIBUTE_ANALYSIS_HPP
