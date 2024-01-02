#include <cmath>
#include <algorithm>

#include "energy_attribute_analysis_calculator.h"


const int WINDOWS_LENGTH = 51;


AttributeAnalysis EnergyAttributeAnalysisCalculator::calculate(const Radargram& radargram){
    AttributeAnalysis result = AttributeAnalysis{
            .attribute_matrix=Eigen::MatrixXf::Zero(radargram.trace_matrix.rows(), radargram.trace_matrix.cols()),
            .name="Energy"
    };
    for (int i = 0; i < radargram.trace_matrix.rows(); ++i) {
        for (int j = 0; j < radargram.trace_matrix.cols(); ++j) {
            int start = std::max(0, j - WINDOWS_LENGTH / 2);
            int finish = std::min(static_cast<int>(radargram.trace_matrix.cols()) - 1, j + WINDOWS_LENGTH / 2);
            float sum = radargram.trace_matrix.row(i).segment(start, finish - start + 1).sum();
            result.attribute_matrix(i, j) = sum;
        }
    }
    return result;
}
