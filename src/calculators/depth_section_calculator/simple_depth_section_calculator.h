#ifndef GPR_READER_SIMPLE_DEPTH_SECTION_CALCULATOR_H
#define GPR_READER_SIMPLE_DEPTH_SECTION_CALCULATOR_H

#include "depth_section_calculator.h"

class SimpleDepthSectionCalculator : public IDepthSectionCalculator {
public:
    DepthSection calculate(const Radargram& radargram) override;
};


#endif //GPR_READER_SIMPLE_DEPTH_SECTION_CALCULATOR_H
