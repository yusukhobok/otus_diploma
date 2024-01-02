#ifndef GPR_READER_CSV_TRAJECTORY_IMPORTER_H
#define GPR_READER_CSV_TRAJECTORY_IMPORTER_H

#include <string>
#include <vector>
#include "trajectory_importer.hpp"

class CsvTrajectoryImporter : public ITrajectoryImporter {
public:
    Trajectory import_trajectory(int trace_count, const std::string& filename) override;
private:
    static std::vector<std::string> read_csv(const std::string& filename);
};


#endif //GPR_READER_CSV_TRAJECTORY_IMPORTER_H
