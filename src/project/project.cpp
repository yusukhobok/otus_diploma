#include <memory>
#include <iostream>

#include "project.h"


Project::Project():
    traces_count(0),
    samples_count(0),
    radargram(std::make_shared<Radargram>()),
    depth_section(std::make_shared<DepthSection>()),
    trajectory(std::make_shared<Trajectory>())
{
}


bool Project::validate() {
    return true;
}


void Project::reflect() {

}


void Project::remove_trace(float distance__m) {

}


void Project::trim(float time__ns) {

}


void Project::remove_air_wave() {

}


void Project::smooth() {

}


void Project::calculate_depth_section(std::shared_ptr<IDepthSectionCalculator> calculator) {
    *depth_section = calculator->calculate(*radargram);
}


void Project::calculate_attribute_analysis(std::shared_ptr<IAttributeAnalysisCalculator> calculator) {
    *attribute_analysis = calculator->calculate(*radargram);
}


void Project::add_layer(const Layer& layer) {
    std::shared_ptr<Layer> new_layer = std::make_shared<Layer>(layer);
    layers.push_back(new_layer);
}


void Project::clear_layers() {
    layers.clear();
}

void Project::export_layers_to_csv(std::string filename) {

}

