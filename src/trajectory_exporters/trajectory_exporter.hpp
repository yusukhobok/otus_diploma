#ifndef GPR_READER_TRAJECTORY_EXPORTER_H
#define GPR_READER_TRAJECTORY_EXPORTER_H

#include <string>
#include "src/domain/trajectory.hpp"

class ITrajectoryExporter {
public:
    virtual Trajectory export_trajectory(int trace_count, const std::string& filename) = 0;
};

#endif //GPR_READER_TRAJECTORY_EXPORTER_H
