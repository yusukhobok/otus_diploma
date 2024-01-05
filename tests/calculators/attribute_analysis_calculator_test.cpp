#include <gtest/gtest.h>
#include <iostream>

#include "src/domain/radargram.hpp"
#include "src/calculators/attribute_analysis_calculator/cosine_phase_attribute_analysis_calculator.h"
#include "src/calculators/attribute_analysis_calculator/energy_attribute_analysis_calculator.h"


Radargram create_radargram(int trace_count, int sample_count) {
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
        AttributeAnalysisCalculatorTests,
        given_radargram__when_calculate_cosine_phase__then_correct_attribute_analysis_result_returned
) {
    int trace_count = 10;
    int sample_count = 20;
    Radargram radargram = create_radargram(trace_count, sample_count);
    auto calculator = CosinePhaseAttributeAnalysisCalculator();

    auto attribute_analysis = calculator.calculate(radargram);

    EXPECT_TRUE(attribute_analysis.has_attribute_analysis);
    EXPECT_EQ(attribute_analysis.name, "Cosine Phase");
    EXPECT_EQ(attribute_analysis.attribute_matrix.rows(), trace_count);
    EXPECT_EQ(attribute_analysis.attribute_matrix.cols(), sample_count);
}


TEST(
        AttributeAnalysisCalculatorTests,
        given_radargram__when_calculate_energy__then_correct_attribute_analysis_result_returned
) {
    int trace_count = 10;
    int sample_count = 20;
    Radargram radargram = create_radargram(trace_count, sample_count);
    auto calculator = EnergyAttributeAnalysisCalculator();

    auto attribute_analysis = calculator.calculate(radargram);

    EXPECT_TRUE(attribute_analysis.has_attribute_analysis);
    EXPECT_EQ(attribute_analysis.name, "Energy");
    EXPECT_EQ(attribute_analysis.attribute_matrix.rows(), trace_count);
    EXPECT_EQ(attribute_analysis.attribute_matrix.cols(), sample_count);
}
