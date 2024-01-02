#ifndef GPR_READER_COSINE_PHASE_ATTRIBUTE_ANALYSIS_CALCULATOR_H
#define GPR_READER_COSINE_PHASE_ATTRIBUTE_ANALYSIS_CALCULATOR_H

#include "attribute_analysis_calculator.h"

class CosinePhaseAttributeAnalysisCalculator : public IAttributeAnalysisCalculator {
public:
    AttributeAnalysis calculate(const Radargram& radargram) override;
    virtual ~CosinePhaseAttributeAnalysisCalculator() = default;
};


#endif //GPR_READER_COSINE_PHASE_ATTRIBUTE_ANALYSIS_CALCULATOR_H
