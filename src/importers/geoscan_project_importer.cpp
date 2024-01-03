#include <iostream>
#include <fstream>
#include <fmt/core.h>
#include <fmt/chrono.h>
#include <chrono>
#include <eigen3/Eigen/Core>

#include "geoscan_project_importer.h"
#include "src/utils/helpers.hpp"


std::chrono::time_point<std::chrono::system_clock> filetime_to_timepoint(FILETIME filetime) {
    std::chrono::file_clock::duration duration{(static_cast<int64_t>(filetime.dwHighDateTime) << 32)
                                               | filetime.dwLowDateTime};
    std::chrono::time_point<std::chrono::system_clock> time_point{duration};
    return time_point;
}


void Header::print() {
    std::string text = fmt::format(
            "trace count = {}\nsample count = {}\ntime_base = {} ns\nepsilon={}\ndelta x={} mm\nantenna distance={} mm",
            trace_count, sample_count, time_base__ns, eps, delta_x__mm, antenna_distance__mm
    );
    std::cout << text << std::endl;
}


void TraceHeader::print() {
    std::string text = fmt::format(
            "time={}\nposition={}\nx={}\ny={}\nz={}\nantenna_index={}\nlabel_id={}\nlabel_position_sample={}",
            filetime_to_timepoint(receive_time), position__mm, x__mm, y__mm, z__mm, antenna_index, label_id, label_position_sample
    );
    std::cout << text << std::endl;
}


std::shared_ptr<Project> GeoScanProjectImporter::import(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("File not exists.");
    }

    Header header{};
    file.read(reinterpret_cast<char*>(&header), sizeof(header));
    file.seekg(512 + sizeof(int) * 256 + sizeof(float) * header.sample_count, std::ios::beg);

    std::shared_ptr<Eigen::MatrixXf> trace_matrix = std::make_shared<Eigen::MatrixXf>(header.trace_count, header.sample_count);
    for (unsigned int i = 0; i < header.trace_count; ++ i) {
        TraceHeader trace_header{};
        file.read(reinterpret_cast<char*>(&trace_header), sizeof(trace_header));
        file.seekg(8, std::ios::cur);

        Eigen::VectorXf trace_vector(header.sample_count);
        file.read(reinterpret_cast<char*>(trace_vector.data()), header.sample_count * sizeof(float));
        trace_matrix->row(i) = trace_vector;
    }
    file.close();

    std::shared_ptr<Radargram> radargram = std::make_shared<Radargram>(
        *trace_matrix,
        static_cast<double>(header.delta_x__mm) / 1000,
        static_cast<double>(header.time_base__ns) / header.sample_count,
        "ОКО-2",
        "АБ-1000",
        1000.0,
        static_cast<double>(header.antenna_distance__mm) / 1000,
        epsilon_to_velocity(header.eps)
    );

    return std::make_shared<Project>(radargram);
}

