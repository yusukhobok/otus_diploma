#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include "mock.h"
#include "src/project/project.h"


class ProjectTest : public ::testing::Test {
protected:
    MockDepthSectionCalculator *depth_section_calculator;
    MockAttributeAnalysisCalculator *attribute_analysis_calculator;
    MockTrajectoryImporter *trajectory_importer;
    MockProjectImporter *project_importer;
    const Radargram radargram = create_radargram(10, 20);
    const DepthSection depth_section = create_depth_section(20);
    const AttributeAnalysis attribute_analysis = create_attribute_analysis(10, 20);
    const Trajectory trajectory = create_trajectory(10);

    void SetUp() override {
        depth_section_calculator = new MockDepthSectionCalculator;
        attribute_analysis_calculator = new MockAttributeAnalysisCalculator;
        trajectory_importer = new MockTrajectoryImporter;
        project_importer = new MockProjectImporter;
    }

    void TearDown() override {
        testing::Mock::VerifyAndClearExpectations(depth_section_calculator);
        testing::Mock::VerifyAndClearExpectations(attribute_analysis_calculator);
        testing::Mock::VerifyAndClearExpectations(trajectory_importer);
        testing::Mock::VerifyAndClearExpectations(project_importer);
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

    static DepthSection create_depth_section(int sample_count) {
        Eigen::VectorXf depth_vector = Eigen::VectorXf::Random(sample_count);
        return DepthSection{
                .has_depth_section=true,
                .depth_vector=depth_vector
        };
    }

    static Trajectory create_trajectory(int trace_count) {
        Eigen::VectorXd x = Eigen::VectorXd::Random(trace_count);
        Eigen::VectorXd y = Eigen::VectorXd::Random(trace_count);
        return Trajectory{
            .has_trajectory=true,
            .x=x,
            .y=y
        };
    }

    static AttributeAnalysis create_attribute_analysis(int trace_count, int sample_count) {
        Eigen::MatrixXf attribute_matrix = Eigen::MatrixXf::Random(trace_count, sample_count);
        return AttributeAnalysis{
                .has_attribute_analysis=true,
                .attribute_matrix=attribute_matrix,
                .name="Test Attribute Analysis"
        };
    }
};


TEST_F(
        ProjectTest,
        given_project__when_validate__then_correct_result_returned
) {
    auto project = Project(std::make_shared<Radargram>(radargram));

    bool validation_result = project.validate();

    ASSERT_TRUE(validation_result);
}


TEST_F(
        ProjectTest,
        given_project__when_reflect__then_radargram_reflected
) {
    auto project = Project(std::make_shared<Radargram>(radargram));
    Radargram original_radargram = *project.get_radargram();

    project.reflect();

    auto radargram = project.get_radargram();
    ASSERT_EQ(original_radargram.trace_matrix.rows(), radargram->trace_matrix.rows());
    ASSERT_EQ(original_radargram.trace_matrix.cols(), radargram->trace_matrix.cols());
    ASSERT_DOUBLE_EQ(original_radargram.trace_matrix(0, 0),
                     radargram->trace_matrix(project.get_trace_count() - 1, 0));
}


TEST_F(
        ProjectTest,
        given_project__when_remove_trace__then_trace_count_changed
) {
    auto project = Project(std::make_shared<Radargram>(radargram));
    Radargram original_radargram = *project.get_radargram();
    int original_trace_count = project.get_trace_count();

    project.remove_trace(5);

    auto radargram = project.get_radargram();
    ASSERT_EQ(original_radargram.trace_matrix.rows() - 1, radargram->trace_matrix.rows());
    ASSERT_EQ(original_radargram.trace_matrix.cols(), radargram->trace_matrix.cols());
    ASSERT_EQ(original_trace_count - 1, project.get_trace_count());
}


TEST_F(
        ProjectTest,
        given_project__when_trim__then_sample_count_changed
) {
    auto project = Project(std::make_shared<Radargram>(radargram));
    Radargram original_radargram = *project.get_radargram();
    int original_sample_count = project.get_sample_count();
    int sample = 10;

    project.trim(sample);

    auto radargram = project.get_radargram();
    ASSERT_EQ(original_radargram.trace_matrix.rows(), radargram->trace_matrix.rows());
    ASSERT_EQ(original_radargram.trace_matrix.cols() - sample, radargram->trace_matrix.cols());
    ASSERT_EQ(original_sample_count - sample, project.get_sample_count());
}


TEST_F(
        ProjectTest,
        given_project__when_remove_air_wave__then_matrix_size_not_changed
) {
    auto project = Project(std::make_shared<Radargram>(radargram));
    Radargram original_radargram = *project.get_radargram();

    project.remove_air_wave();

    auto radargram = project.get_radargram();
    ASSERT_EQ(original_radargram.trace_matrix.rows(), radargram->trace_matrix.rows());
    ASSERT_EQ(original_radargram.trace_matrix.cols(), radargram->trace_matrix.cols());
}


TEST_F(
        ProjectTest,
        given_project__when_calculate_depth_section__then_depth_section_calculated
) {
    EXPECT_CALL(*depth_section_calculator, calculate(radargram))
            .WillOnce(testing::Return(depth_section));
    auto project = Project(std::make_shared<Radargram>(radargram));

    project.calculate_depth_section(std::shared_ptr<MockDepthSectionCalculator>(depth_section_calculator));

    ASSERT_EQ(*project.get_depth_section(), depth_section);
}


TEST_F(
        ProjectTest,
        given_project__when_calculate_attribute_analysis__then_attribute_analysis_calculated
) {
    EXPECT_CALL(*attribute_analysis_calculator, calculate(radargram))
            .WillOnce(testing::Return(attribute_analysis));
    auto project = Project(std::make_shared<Radargram>(radargram));

    project.calculate_attribute_analysis(std::shared_ptr<MockAttributeAnalysisCalculator>(attribute_analysis_calculator));

    ASSERT_EQ(*project.get_attribute_analysis(), attribute_analysis);
}


TEST_F(
        ProjectTest,
        given_project__when_remove_attribute_analysis__then_attribute_analysis_removed
) {
    EXPECT_CALL(*attribute_analysis_calculator, calculate(radargram))
            .WillOnce(testing::Return(attribute_analysis));
    auto project = Project(std::make_shared<Radargram>(radargram));
    project.calculate_attribute_analysis(std::shared_ptr<MockAttributeAnalysisCalculator>(attribute_analysis_calculator));

    project.remove_attribute_analysis();

    ASSERT_FALSE(project.get_attribute_analysis()->has_attribute_analysis);
}


TEST_F(
        ProjectTest,
        given_project__when_import_trajectory__then_trajectory_imported_correctly
) {
    EXPECT_CALL(*trajectory_importer, import_trajectory(10, "test"))
            .WillOnce(testing::Return(trajectory));
    auto project = Project(std::make_shared<Radargram>(radargram));

    project.import_trajectory(std::shared_ptr<MockTrajectoryImporter>(trajectory_importer), "test");

    ASSERT_EQ(*project.get_trajectory(), trajectory);
}


TEST_F(
        ProjectTest,
        given_project__when_add_layer__then_layer_added
) {
    auto project = Project(std::make_shared<Radargram>(radargram));

    project.add_layer("layer name");

    auto layers = project.get_layers();
    ASSERT_EQ(layers.size(), 1);
    ASSERT_EQ(layers[0]->layer_name, "layer name");
    ASSERT_EQ(layers[0]->sample_vector.size(), project.get_trace_count());
}


TEST_F(
        ProjectTest,
        given_project__when_clear_layers__then_layer_added
) {
    auto project = Project(std::make_shared<Radargram>(radargram));
    project.add_layer("layer name");

    project.clear_layers();

    auto layers = project.get_layers();
    ASSERT_TRUE(layers.empty());
}


TEST_F(
        ProjectTest,
        given_trace__when_trace_to_distance__then_correct_distance_returned
) {
    auto project = Project(std::make_shared<Radargram>(radargram));

    auto distance = project.trace_to_distance(5);

    ASSERT_DOUBLE_EQ(distance, 2.5);
}


TEST_F(
        ProjectTest,
        given_trace__when_distance_to_trace__then_correct_trace_returned
) {
    auto project = Project(std::make_shared<Radargram>(radargram));

    auto trace = project.distance_to_trace(2.5);

    ASSERT_EQ(trace, 5);
}


TEST_F(
        ProjectTest,
        given_trace__when_sample_to_time__then_correct_time_returned
) {
    auto project = Project(std::make_shared<Radargram>(radargram));

    auto time = project.sample_to_time(5);

    ASSERT_DOUBLE_EQ(time, 1);
}


TEST_F(
        ProjectTest,
        given_trace__when_time_to_sample__then_correct_sample_returned
) {
    auto project = Project(std::make_shared<Radargram>(radargram));

    auto sample = project.time_to_sample(5);

    ASSERT_EQ(sample, 25);
}


TEST_F(
        ProjectTest,
        given_trace__when_get_distance_max__m__then_correct_distance_returned
) {
    auto project = Project(std::make_shared<Radargram>(radargram));

    auto distance = project.get_distance_max__m();

    ASSERT_DOUBLE_EQ(distance, 4.5);
}


TEST_F(
        ProjectTest,
        given_trace__when_get_time_max__ns__then_correct_time_returned
) {
    auto project = Project(std::make_shared<Radargram>(radargram));

    auto time = project.get_time_max__ns();

    ASSERT_NEAR(time, 3.8, 1e-3);
}
