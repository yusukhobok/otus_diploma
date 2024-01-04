#include <memory>
#include <iostream>
#include <ranges>
#include <algorithm>
#include <cmath>
#include <fmt/format.h>
#include <fstream>

#include "project.h"


Project::Project(std::shared_ptr<Radargram> radargram) :
        trace_count(radargram->trace_matrix.rows()),
        sample_count(radargram->trace_matrix.cols()),
        radargram(std::move(radargram)),
        attribute_analysis(std::make_shared<AttributeAnalysis>(false,Eigen::MatrixXf(trace_count, sample_count), "")),
        depth_section(std::make_shared<DepthSection>(false, Eigen::VectorXf(sample_count))),
        trajectory(std::make_shared<Trajectory>(false,Eigen::VectorXd(trace_count), Eigen::VectorXd(trace_count))),
        position(0, 0) {
    if (!validate()) {
        throw std::runtime_error("Проект в невалидном состоянии.");
    }
}


void Project::print() const {
    std::cout << fmt::format(
            "traces count = {}\nsamples count = {}\n"
            "delta_distance = {} m\ndelta time = {} ns\nantenna distance = {} m\nvelocity = {} m/ns\n"
            "min = {}\nmax = {}\nmean = {}\nAA name = `{}`\nmin AA = {}\nmax AA = {}\nmean AA = {}\nmax depth = {}\n"
            "layer count = {}\n"
            "x0 = {}\ny0 = {}\nx1 = {}\ny1 = {}\n",
            trace_count,
            sample_count,
            radargram->delta_distance__m,
            radargram->delta_time__ns,
            radargram->antenna_distance__m,
            radargram->velocity__m_per_ns,
            radargram->trace_matrix.minCoeff(),
            radargram->trace_matrix.maxCoeff(),
            radargram->trace_matrix.mean(),
            attribute_analysis->name,
            attribute_analysis->attribute_matrix.minCoeff(),
            attribute_analysis->attribute_matrix.maxCoeff(),
            attribute_analysis->attribute_matrix.mean(),
            depth_section->depth_vector.maxCoeff(),
            layers.size(),
            trajectory->x(0),
            trajectory->y(0),
            trajectory->x(trajectory->x.size() - 1),
            trajectory->y(trajectory->y.size() - 1)
    );
    for (const auto &layer: layers) {
        std::cout << fmt::format(
                "min = {} max = {} mean = {}\n",
                layer->sample_vector.minCoeff(), layer->sample_vector.maxCoeff(),
                layer->sample_vector.mean()
        );
    }
}


bool Project::validate() const {
    if (trace_count != radargram->trace_matrix.rows()) {
        return false;
    }
    if (sample_count != radargram->trace_matrix.cols()) {
        return false;
    }
    if (radargram->delta_distance__m <= 0) {
        return false;
    }
    if (radargram->delta_time__ns <= 0) {
        return false;
    }
    if (radargram->frequency__MHz <= 0) {
        return false;
    }
    if (radargram->antenna_distance__m <= 0) {
        return false;
    }
    if (radargram->velocity__m_per_ns <= 0) {
        return false;
    }
    if (trace_count != attribute_analysis->attribute_matrix.rows()) {
        return false;
    }
    if (sample_count != attribute_analysis->attribute_matrix.cols()) {
        return false;
    }
    if (sample_count != depth_section->depth_vector.rows()) {
        return false;
    }
    if (!std::ranges::all_of(layers, [&](const std::shared_ptr<Layer> &layer) {
        return trace_count == layer->sample_vector.rows();
    })) {
        return false;
    }
    if (position.trace < 0 || position.trace >= trace_count || position.sample < 0 || position.sample >= sample_count) {
        return false;
    }
    return true;
}


void Project::reflect() {
    std::cout << radargram->trace_matrix(0, 0) << std::endl;
    Eigen::MatrixXf new_trace_matrix = radargram->trace_matrix.colwise().reverse();
    radargram->trace_matrix = new_trace_matrix;
    std::cout << radargram->trace_matrix(0, 0) << std::endl;
    Eigen::MatrixXf new_attribute_matrix = attribute_analysis->attribute_matrix.colwise().reverse();
    attribute_analysis->attribute_matrix = new_attribute_matrix;
    depth_section->depth_vector.reverse();
    if (!validate()) {
        throw std::runtime_error("Проект в невалидном состоянии.");
    }
}


void Project::remove_trace(int trace) {
    Eigen::MatrixXf new_trace_matrix(radargram->trace_matrix.rows() - 1, radargram->trace_matrix.cols());
    new_trace_matrix.topRows(trace) = radargram->trace_matrix.topRows(trace);
    new_trace_matrix.bottomRows(radargram->trace_matrix.rows() - trace - 1) = radargram->trace_matrix.bottomRows(
            radargram->trace_matrix.rows() - trace - 1);
    radargram->trace_matrix = new_trace_matrix;

    Eigen::MatrixXf new_attribute_matrix(attribute_analysis->attribute_matrix.rows() - 1,
                                         attribute_analysis->attribute_matrix.cols());
    new_attribute_matrix.topRows(trace) = attribute_analysis->attribute_matrix.topRows(trace);
    new_attribute_matrix.bottomRows(
            attribute_analysis->attribute_matrix.rows() - trace - 1) = attribute_analysis->attribute_matrix.bottomRows(
            attribute_analysis->attribute_matrix.rows() - trace - 1);
    attribute_analysis->attribute_matrix = new_attribute_matrix;

    for (auto &layer: layers) {
        Eigen::VectorXi new_sample_vector(layer->sample_vector.size() - 1);
        new_sample_vector.head(trace) = layer->sample_vector.head(trace);
        new_sample_vector.tail(layer->sample_vector.size() - trace - 1) = layer->sample_vector.tail(
                layer->sample_vector.size() - trace - 1);
        layer->sample_vector = new_sample_vector;
    }

    --trace_count;
    position.trace = 0;

    if (!validate()) {
        throw std::runtime_error("Проект в невалидном состоянии.");
    }
}


void Project::trim(int sample) {
    if (sample < 5) {
        return;
    }

    Eigen::MatrixXf new_trace_matrix(radargram->trace_matrix.rows(), sample);
    new_trace_matrix = radargram->trace_matrix.leftCols(sample);
    radargram->trace_matrix = new_trace_matrix;

    Eigen::MatrixXf new_attribute_matrix(radargram->trace_matrix.rows(), sample);
    new_attribute_matrix = radargram->trace_matrix.leftCols(sample);
    attribute_analysis->attribute_matrix = new_attribute_matrix;

    depth_section->depth_vector = depth_section->depth_vector.head(sample);

    sample_count = sample;
    position.sample = 0;

    if (!validate()) {
        throw std::runtime_error("Проект в невалидном состоянии.");
    }
}


void Project::remove_air_wave() {
    Eigen::VectorXf mean = radargram->trace_matrix.colwise().mean();
    radargram->trace_matrix.rowwise() -= mean.transpose();
    if (!validate()) {
        throw std::runtime_error("Проект в невалидном состоянии.");
    }
}


void Project::calculate_depth_section(std::shared_ptr<IDepthSectionCalculator> calculator) {
    *depth_section = calculator->calculate(*radargram);
    if (!validate()) {
        throw std::runtime_error("Проект в невалидном состоянии.");
    }
}


void Project::calculate_attribute_analysis(std::shared_ptr<IAttributeAnalysisCalculator> calculator) {
    *attribute_analysis = calculator->calculate(*radargram);
    if (!validate()) {
        throw std::runtime_error("Проект в невалидном состоянии.");
    }
}


void Project::remove_attribute_analysis() {
    attribute_analysis->has_attribute_analysis = false;
}


void Project::import_trajectory(std::shared_ptr<ITrajectoryImporter> trajectory_importer, const std::string& filename) {
    *trajectory = trajectory_importer->import_trajectory(trace_count, filename);
    if (!validate()) {
        throw std::runtime_error("Проект в невалидном состоянии.");
    }
}


void Project::add_layer(const std::string& layer_name) {
    Eigen::VectorXi sample_vector = Eigen::VectorXi::Constant(trace_count, position.sample);
    auto layer = Layer{.sample_vector=sample_vector, .layer_name=layer_name};

    std::shared_ptr<Layer> new_layer = std::make_shared<Layer>(layer);
    layers.push_back(new_layer);
    if (!validate()) {
        throw std::runtime_error("Проект в невалидном состоянии.");
    }
}


void Project::clear_layers() {
    layers.clear();
    if (!validate()) {
        throw std::runtime_error("Проект в невалидном состоянии.");
    }
}


void Project::export_layers_to_csv(const std::string& filename) {
    std::ofstream csv_file(filename);

    if (!csv_file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл для записи.");
    }

    for (const auto& layer : layers) {
        csv_file << layer->layer_name << ",";
        for (int i = 0; i < layer->sample_vector.size(); ++i) {
            csv_file << layer->sample_vector[i];
            if (i < layer->sample_vector.size() - 1) {
                csv_file << ",";
            }
        }
        csv_file << "\n";
    }

    csv_file.close();
}


std::shared_ptr<Radargram> Project::get_radargram() {
    return radargram;
}


std::shared_ptr<AttributeAnalysis> Project::get_attribute_analysis() {
    return attribute_analysis;
}


std::shared_ptr<DepthSection> Project::get_depth_section() {
    return depth_section;
}


std::shared_ptr<Trajectory> Project::get_trajectory() {
    return trajectory;
}


std::vector<std::shared_ptr<Layer>> Project::get_layers() {
    return layers;
}


int Project::get_trace_count() const {
    return trace_count;
}


int Project::get_sample_count() const {
    return sample_count;
}


double Project::trace_to_distance(int trace) const {
    return trace * radargram->delta_distance__m;
}


int Project::distance_to_trace(double distance__m) const {
    return round(distance__m / radargram->delta_distance__m);
}


double Project::sample_to_time(int sample) const {
    return sample * radargram->delta_time__ns;
}


int Project::time_to_sample(double time__ns) const {
    return round(time__ns / radargram->delta_time__ns);
}


Position Project::get_position() const {
    return position;
}


void Project::set_position(int trace, int sample) {
    if (trace < 0 || trace >= trace_count || sample < 0 || sample >= sample_count) {
        return;
    }
    position.trace = trace;
    position.sample = sample;
}


double Project::get_distance_max__m() const {
    return trace_to_distance(trace_count - 1);
}


double Project::get_time_max__ns() const {
    return sample_to_time(sample_count - 1);
}


std::string Project::get_info() const {
    return fmt::format(
        "traces count = {}\nsamples count = {}\n"
        "delta distance = {:.3f} m\ndelta time = {:.3f} ns\nantenna distance = {:.3f} m\nvelocity = {:.3f} m/ns\n"
        "antenna name = `{}`\ngpr unit = `{}`\nfrequency = {:.1f} MHz\nlayer count = {}",
        trace_count,
        sample_count,
        radargram->delta_distance__m,
        radargram->delta_time__ns,
        radargram->antenna_distance__m,
        radargram->velocity__m_per_ns,
        radargram->antenna_name,
        radargram->gpr_unit,
        radargram->frequency__MHz,
        layers.size()
    );
}
