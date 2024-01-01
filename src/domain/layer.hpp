#ifndef GPR_READER_LAYES_H
#define GPR_READER_LAYES_H


#include <eigen3/Eigen/Core>


struct Layer {
    Eigen::VectorXf time_vector;
    std::string layer_name;
};


#endif //GPR_READER_LAYES_H
