#include <fstream>
#include <iostream>
#include "csv_trajectory_importer.h"

std::vector<std::string> CsvTrajectoryImporter::read_csv(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Ошибка открытия CSV файла.");
    }
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    file.close();
    return lines;
}


Trajectory CsvTrajectoryImporter::import_trajectory(int trace_count, const std::string& filename) {
    std::vector<std::string> lines = read_csv(filename);
    if (static_cast<int>(lines.size()) != trace_count) {
        throw std::runtime_error("Некорректное количество строк в файле.");
    }

    Eigen::VectorXd x = Eigen::VectorXd::Zero(trace_count);
    Eigen::VectorXd y = Eigen::VectorXd::Zero(trace_count);
    for (int i = 0; i < trace_count; ++i) {
        std::istringstream iss(lines[i]);
        std::string token;

        int j = 0;
        while (std::getline(iss, token, ',')) {
            if (j == 1) {
                    x(i) = std::stod(token);
            } else {
                y(i) = std::stod(token);
            }
            ++ j;
        }
    }
    return Trajectory{.x=x, .y=y};
}
