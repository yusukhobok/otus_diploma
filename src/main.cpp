#include <QApplication>
#include <iostream>
#include <Eigen/Dense>

#include "src/widgets/mainwindow.h"
#include "src/importers/geoscan_project_importer.h"

int main(int argc, char *argv[])
{
    std::shared_ptr<GeoScanProjectImporter> geoscan_importer = std::make_shared<GeoScanProjectImporter>();
    auto project = geoscan_importer->import("");
    std::cout << project->validate() << std::endl;

//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    return a.exec();
}
