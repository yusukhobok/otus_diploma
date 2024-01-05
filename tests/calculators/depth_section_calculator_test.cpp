#include <gtest/gtest.h>
#include <iostream>

#include "src/domain/radargram.hpp"
#include "src/calculators/depth_section_calculator/simple_depth_section_calculator.h"


Radargram create_radargram2(int trace_count, int sample_count) {  // todo - rename
    Eigen::MatrixXf trace_matrix = Eigen::MatrixXf::Random(trace_count, sample_count);
    return Radargram{
            .trace_matrix=trace_matrix,
            .delta_distance__m=0.5,
            .delta_time__ns=0.2,
            .gpr_unit="GPR Unit",
            .antenna_name="Antenna name",
            .frequency__MHz=250.0,
            .antenna_distance__m=0.01,
            .velocity__m_per_ns=0.1,
    };
}

TEST(
        DepthSectionCalculatorTests,
        given_radargram__when_calculate_simple_depth_section__then_correct_depth_section_returned
) {
    int trace_count = 10;
    int sample_count = 20;
    Radargram radargram = create_radargram2(trace_count, sample_count);
    auto calculator = SimpleDepthSectionCalculator();

    auto depth_section = calculator.calculate(radargram);

    EXPECT_TRUE(depth_section.has_depth_section);
    EXPECT_EQ(depth_section.depth_vector.rows(), sample_count);
}
