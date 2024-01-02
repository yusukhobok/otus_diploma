#ifndef GPR_READER_PROJECT_H
#define GPR_READER_PROJECT_H

#include <memory>
#include <string>

#include "src/domain/radargram.hpp"
#include "src/domain/attribute_analysis.hpp"
#include "src/domain/depth_section.hpp"
#include "src/domain/layer.hpp"
#include "src/calculators/attribute_analysis_calculator/attribute_analysis_calculator.h"
#include "src/calculators/depth_section_calculator/depth_section_calculator.h"

class Project {
private:
    int trace_count;
    int sample_count;
    std::shared_ptr<Radargram> radargram;
    std::shared_ptr<AttributeAnalysis> attribute_analysis;
    std::shared_ptr<DepthSection> depth_section;
    std::vector<std::shared_ptr<Layer>> layers;
public:
    Project(std::shared_ptr<Radargram> radargram);
    void print() const;
    bool validate() const;
    void reflect();
    void remove_trace(int trace);
    void trim(int sample);
    void remove_air_wave();
    void calculate_depth_section(std::shared_ptr<IDepthSectionCalculator> calculator);
    void calculate_attribute_analysis(std::shared_ptr<IAttributeAnalysisCalculator> calculator);
    void add_layer(const Layer& layer);
    void clear_layers();
    void export_layers_to_csv(std::string filename);
};


#endif //GPR_READER_PROJECT_H
