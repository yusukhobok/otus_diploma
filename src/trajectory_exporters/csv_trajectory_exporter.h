#ifndef GPR_READER_CSV_TRAJECTORY_EXPORTER_H
#define GPR_READER_CSV_TRAJECTORY_EXPORTER_H

#include <string>
#include <vector>
#include "trajectory_exporter.hpp"

class CsvTrajectoryExporter : public ITrajectoryExporter {
public:
    Trajectory export_trajectory(int trace_count, const std::string& filename) override;
private:
    std::vector<std::string> read_csv(const std::string& filename);
};


#endif //GPR_READER_CSV_TRAJECTORY_EXPORTER_H
