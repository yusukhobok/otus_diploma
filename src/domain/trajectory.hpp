#ifndef GPR_READER_TRAJECTORY_HPP
#define GPR_READER_TRAJECTORY_HPP


#include <eigen3/Eigen/Core>

using Eigen::VectorXd;

class Trajectory {
    VectorXd x_vector;
    VectorXd y_vector;
    VectorXd z_vector;
};


#endif //GPR_READER_TRAJECTORY_HPP
