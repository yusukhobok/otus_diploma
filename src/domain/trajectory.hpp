#ifndef GPR_READER_TRAJECTORY_H
#define GPR_READER_TRAJECTORY_H


#include <eigen3/Eigen/Core>


struct Trajectory {
    bool has_trajectory;
    Eigen::VectorXd x;
    Eigen::VectorXd y;

    bool operator==(const Trajectory& other) const {
        return has_trajectory == other.has_trajectory && x == other.x && y == other.y;
    }
};


#endif //GPR_READER_TRAJECTORY_H
