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
#include "src/domain/trajectory.hpp"
#include "src/trajectory_importers/trajectory_importer.hpp"
#include "src/domain/position.hpp"

class Project {
private:
    int trace_count;
    int sample_count;
    std::shared_ptr<Radargram> radargram;
    std::shared_ptr<AttributeAnalysis> attribute_analysis;
    std::shared_ptr<DepthSection> depth_section;
    std::shared_ptr<Trajectory> trajectory;
    std::vector<std::shared_ptr<Layer>> layers;
    Position position;

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
    void import_trajectory(std::shared_ptr<ITrajectoryImporter> trajectory_importer, const std::string& filename);
    void add_layer(const Layer& layer);
    void clear_layers();
    void export_layers_to_csv(std::string filename);

    std::shared_ptr<Radargram> get_radargram();
    std::shared_ptr<AttributeAnalysis> get_attribute_analysis();
    std::shared_ptr<DepthSection> get_depth_section();
    std::shared_ptr<Trajectory> get_trajectory();
    std::vector<std::shared_ptr<Layer>> get_layers();
    int get_trace_count() const;
    int get_sample_count() const;
    double trace_to_distance(int trace) const;
    int distance_to_trace(double distance__m) const;
    double sample_to_time(int sample) const;
    int time_to_sample(double time__ns) const;
    Position get_position() const;
    void set_position(int trace, int sample);
    double get_distance_max__m() const;
    double get_time_max__ns() const;
};


#endif //GPR_READER_PROJECT_H
