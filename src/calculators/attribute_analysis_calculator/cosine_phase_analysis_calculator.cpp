#include "cosine_phase_attribute_analysis_calculator.h"

const int COSINE_PHASE_FACTOR = 10;

AttributeAnalysis CosinePhaseAttributeAnalysisCalculator::calculate(const Radargram& radargram){
    AttributeAnalysis result = AttributeAnalysis{
            .has_attribute_analysis=true,
            .attribute_matrix=Eigen::MatrixXf::Zero(radargram.trace_matrix.rows(), radargram.trace_matrix.cols()),
            .name="Cosine Phase"
    };
    for (int i = 0; i < radargram.trace_matrix.rows(); ++i) {
        Eigen::VectorXf trace = radargram.trace_matrix.row(i);
        for (int j = 0; j < trace.rows(); ++j) {
            double sum = 0;
            for (int k = -COSINE_PHASE_FACTOR; k <= +COSINE_PHASE_FACTOR; ++k) {
                if ((k != 0) and (j - k >= 0) and (j - k < trace.rows())) {
                    sum += pow(sin(M_PI * k /2), 2) * trace[j - k] / k;
                }
            }
            double x = (sum / M_PI) * 2;
            double y = trace[j];
            if (x == 0 and y == 0) {
                result.attribute_matrix(i, j) = 0;
            } else {
                result.attribute_matrix(i, j) = static_cast<float>(y / (sqrt(pow(x, 2) + pow(y,2))));
            }
        }
    }
    return result;
}
