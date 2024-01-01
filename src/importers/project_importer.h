#ifndef GPR_READER_PROJECT_IMPORTER_H
#define GPR_READER_PROJECT_IMPORTER_H

#include <memory>
#include <string>
#include "src/project/project.h"

class ProjectImporter {
public:
    virtual std::shared_ptr<Project> import(std::string filename) = 0;
};


#endif //GPR_READER_PROJECT_IMPORTER_H
