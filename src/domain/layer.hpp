#ifndef GPR_READER_LAYES_H
#define GPR_READER_LAYES_H


#include <eigen3/Eigen/Core>


struct Layer {
    Eigen::VectorXi sample_vector;
    std::string layer_name;

    bool operator==(const Layer& other) const {
        return sample_vector == other.sample_vector && layer_name == other.layer_name;
    }
};


#endif //GPR_READER_LAYES_H
