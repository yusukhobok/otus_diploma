#include <QApplication>
#include <iostream>
#include <Eigen/Dense>

#include "src/widgets/mainwindow.h"
#include "src/importers/geoscan_project_importer.h"
#include "src/calculators/depth_section_calculator/simple_depth_section_calculator.h"
#include "src/calculators/attribute_analysis_calculator/energy_attribute_analysis_calculator.h"
#include "src/calculators/attribute_analysis_calculator/cosine_phase_attribute_analysis_calculator.h"

int main(int argc, char *argv[]) {
    std::shared_ptr<GeoScanProjectImporter> geoscan_importer = std::make_shared<GeoScanProjectImporter>();
    auto project = geoscan_importer->import("/home/yuri/Documents/study/cpp-course/hometask/diploma/data/P0128_0000.gpr2");
//    std::shared_ptr<SimpleDepthSectionCalculator> simple_depth_section_calculator = std::make_shared<SimpleDepthSectionCalculator>();
//    project->calculate_depth_section(simple_depth_section_calculator);
//    project->print();

//    std::shared_ptr<EnergyAttributeAnalysisCalculator> energy_attribute_analysis_calculator = std::make_shared<EnergyAttributeAnalysisCalculator>();
//    project->calculate_attribute_analysis(energy_attribute_analysis_calculator);
//    project->print();

//    std::shared_ptr<CosinePhaseAttributeAnalysisCalculator> cosine_phase_attribute_analysis_calculator = std::make_shared<CosinePhaseAttributeAnalysisCalculator>();
//    project->calculate_attribute_analysis(cosine_phase_attribute_analysis_calculator);
//    project->print();

//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    return a.exec();
}
