#ifndef GPR_READER_PROJECT_H
#define GPR_READER_PROJECT_H

#include <memory>
#include <string>

#include "src/domain/radargram.hpp"
#include "src/domain/attribute_analysis.hpp"
#include "src/domain/depth_section.hpp"
#include "src/domain/layer.hpp"
#include "src/domain/trajectory.hpp"
#include "src/calculators/attribute_analysis_calculator/attribute_analysis_calculator.h"
#include "src/calculators/depth_section_calculator/depth_section_calculator.h"

class Project {
private:
    int traces_count;
    int samples_count;
    std::shared_ptr<Radargram> radargram;
    std::shared_ptr<AttributeAnalysis> attribute_analysis;
    std::shared_ptr<DepthSection> depth_section;
    std::shared_ptr<Trajectory> trajectory;
    std::vector<std::shared_ptr<Layer>> layers;
public:
    Project();
    bool validate();
    void reflect();
    void remove_trace(float distance__m);
    void trim(float time__ns);
    void remove_air_wave();
    void smooth();
    void calculate_depth_section(std::shared_ptr<IDepthSectionCalculator> calculator);
    void calculate_attribute_analysis(std::shared_ptr<IAttributeAnalysisCalculator> calculator);
    void add_layer(const Layer& layer);
    void clear_layers();
    void export_layers_to_csv(std::string filename);
};


#endif //GPR_READER_PROJECT_H
