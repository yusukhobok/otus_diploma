#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include "mock.h"
#include "src/project/project.h"


class ProjectTest : public ::testing::Test{
protected:
    MockDepthSectionCalculator *depth_section_calculator;
    const Radargram radargram = create_radargram(10, 20);
    DepthSection depth_section;

    void SetUp() override {
        depth_section_calculator = new MockDepthSectionCalculator;
//        EXPECT_CALL(*depth_section_calculator, calculate(radargram))
//                .WillOnce(testing::Return(depth_section));
    }

    void TearDown() override {
        testing::Mock::VerifyAndClearExpectations(depth_section_calculator);
    }

    static Radargram create_radargram(int trace_count, int sample_count) {
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
};


TEST_F(
        ProjectTest,
        given_project__when_validate__then_correct_result_returned
)
{
    auto project = Project(std::make_shared<Radargram>(radargram));

    bool validation_result = project.validate();

    ASSERT_TRUE(validation_result);
}