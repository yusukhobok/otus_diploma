#ifndef GPR_READER_ATTRIBUTE_ANALYSIS_CALCULATOR_H
#define GPR_READER_ATTRIBUTE_ANALYSIS_CALCULATOR_H


#include "src/domain/attribute_analysis.hpp"
#include "src/domain/radargram.hpp"

class IAttributeAnalysisCalculator {
public:
    virtual AttributeAnalysis calculate(const Radargram& radargram) = 0;
};


#endif //GPR_READER_ATTRIBUTE_ANALYSIS_CALCULATOR_H
