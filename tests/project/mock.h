#ifndef GPR_READER_MOCK_H
#define GPR_READER_MOCK_H

#include <gmock/gmock.h>
#include "src/calculators/depth_section_calculator/depth_section_calculator.h"
#include "src/calculators/attribute_analysis_calculator/attribute_analysis_calculator.h"
#include "src/project/project.h"
#include "src/importers/project_importer.h"


class MockDepthSectionCalculator : public IDepthSectionCalculator {
public:
    MOCK_METHOD(DepthSection, calculate, (const Radargram& ), (override));
};


class MockAttributeAnalysisCalculator : public IAttributeAnalysisCalculator {
public:
    MOCK_METHOD(AttributeAnalysis, calculate, (const Radargram& ), (override));
};


class MockTrajectoryImporter : public ITrajectoryImporter {
public:
    MOCK_METHOD(Trajectory, import_trajectory, (int, const std::string& ), (override));
};


class MockProjectImporter : public ProjectImporter {
public:
    MOCK_METHOD(std::shared_ptr<Project>, import_project, (const std::string& ), (override));
};



#endif //GPR_READER_MOCK_H
