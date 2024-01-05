#ifndef GPR_READER_GEOSCAN_PROJECT_IMPORTER_H
#define GPR_READER_GEOSCAN_PROJECT_IMPORTER_H


#include "project_importer.h"


#pragma pack(4)

struct FILETIME {
    int dwLowDateTime;
    int dwHighDateTime;
};

struct Header {
    unsigned int label;
    unsigned int id_version;
    unsigned int main_number;
    unsigned int series_number;
    unsigned int profile_number_in_series;
    unsigned int state;
    unsigned int trace_count;
    unsigned int sample_count;
    unsigned int text_label_count;
    unsigned int time_base__ns;
    float eps;
    int delta_x__mm;
    int64_t start_position;
    int64_t start_x;
    int64_t start_y;
    int64_t start_z;
    FILETIME create_time;
    FILETIME manipulation_time;
    unsigned int antenna_distance__mm;
    int start_sample_count;
    int direct_wave_sample;
    float direct_wave_threshold;
    float acceleration_factor;
    unsigned int window_size;
    unsigned int horizontal_window_size;
    int white_procent;
    int black_procent;
    unsigned int scan_mode;
    unsigned int data_accumulation_ratio;
    unsigned int shift;

    void print();
};

struct TraceHeader {
    FILETIME receive_time;
    int position__mm;
    int x__mm;
    int y__mm;
    int z__mm;
    int antenna_index;
    unsigned int label_id;
    unsigned int label_position_sample;

    void print();
};

#pragma pack()


class GeoScanProjectImporter : public ProjectImporter {
public:
    std::shared_ptr<Project> import_project(const std::string& filename) override;
    virtual ~GeoScanProjectImporter() = default;
private:

};


#endif //GPR_READER_GEOSCAN_PROJECT_IMPORTER_H
