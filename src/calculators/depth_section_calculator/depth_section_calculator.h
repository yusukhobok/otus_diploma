#ifndef GPR_READER_DEPTH_SECTION_CALCULATOR_H
#define GPR_READER_DEPTH_SECTION_CALCULATOR_H

#include "src/domain/depth_section.hpp"
#include "src/domain/radargram.hpp"

class IDepthSectionCalculator {
public:
    virtual DepthSection calculate(const Radargram& radargram) = 0;
};

#endif //GPR_READER_DEPTH_SECTION_CALCULATOR_H
