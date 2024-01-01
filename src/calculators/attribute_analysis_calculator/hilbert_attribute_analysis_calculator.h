#ifndef GPR_READER_HILBERT_ATTRIBUTE_ANALYSIS_CALCULATOR_H
#define GPR_READER_HILBERT_ATTRIBUTE_ANALYSIS_CALCULATOR_H

#include "attribute_analysis_calculator.h"

class HilbertAttributeAnalysisCalculator : public IAttributeAnalysisCalculator {
public:
    AttributeAnalysis calculate(const Radargram& radargram) override;
};


#endif //GPR_READER_HILBERT_ATTRIBUTE_ANALYSIS_CALCULATOR_H
