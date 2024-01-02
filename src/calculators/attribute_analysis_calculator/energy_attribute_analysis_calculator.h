#ifndef GPR_READER_ENERGY_ATTRIBUTE_ANALYSIS_CALCULATOR_H
#define GPR_READER_ENERGY_ATTRIBUTE_ANALYSIS_CALCULATOR_H

#include "attribute_analysis_calculator.h"


class EnergyAttributeAnalysisCalculator: public IAttributeAnalysisCalculator {
public:
    AttributeAnalysis calculate(const Radargram& radargram) override;
    virtual ~EnergyAttributeAnalysisCalculator() = default;
};

#endif //GPR_READER_ENERGY_ATTRIBUTE_ANALYSIS_CALCULATOR_H
