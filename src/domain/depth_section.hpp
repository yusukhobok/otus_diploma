#ifndef GPR_READER_DEPTH_SECTION_HPP
#define GPR_READER_DEPTH_SECTION_HPP


#include <eigen3/Eigen/Core>


struct DepthSection {
    bool has_depth_section;
    Eigen::VectorXf depth_vector;

    bool operator==(const DepthSection& other) const {
        return has_depth_section == other.has_depth_section && depth_vector == other.depth_vector;
    }
};



#endif //GPR_READER_DEPTH_SECTION_HPP
