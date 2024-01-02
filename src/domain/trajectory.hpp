#ifndef GPR_READER_TRAJECTORY_H
#define GPR_READER_TRAJECTORY_H


#include <eigen3/Eigen/Core>


struct Trajectory {
    Eigen::VectorXd x;
    Eigen::VectorXd y;
};


#endif //GPR_READER_TRAJECTORY_H
