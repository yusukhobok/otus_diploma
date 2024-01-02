#ifndef GPR_READER_DEPTH_SECTION_HPP
#define GPR_READER_DEPTH_SECTION_HPP


#include <eigen3/Eigen/Core>


struct DepthSection {
    bool has_depth_section;
    Eigen::VectorXf depth_vector;
};



#endif //GPR_READER_DEPTH_SECTION_HPP
