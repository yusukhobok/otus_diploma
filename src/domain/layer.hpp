#ifndef GPR_READER_LAYES_H
#define GPR_READER_LAYES_H


#include <eigen3/Eigen/Core>

using Eigen::VectorXd;

struct Layer {
    VectorXd time_vector;
    std::string layer_name;
};


#endif //GPR_READER_LAYES_H
