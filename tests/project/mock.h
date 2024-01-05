#ifndef GPR_READER_MOCK_H
#define GPR_READER_MOCK_H

#include <gmock/gmock.h>
#include "src/calculators/depth_section_calculator/depth_section_calculator.h"


class MockDepthSectionCalculator : public IDepthSectionCalculator {
public:
    MOCK_METHOD(DepthSection, calculate, (const Radargram& ), (override));
};




#endif //GPR_READER_MOCK_H
