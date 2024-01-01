#ifndef GPR_READER_GEOSCAN_PROJECT_IMPORTER_H
#define GPR_READER_GEOSCAN_PROJECT_IMPORTER_H


#include "project_importer.h"

class GeoScanProjectImporter : public ProjectImporter {
public:
    std::shared_ptr<Project> import(std::string filename) override;
};


#endif //GPR_READER_GEOSCAN_PROJECT_IMPORTER_H
