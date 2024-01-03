#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QMainWindow>
#include "src/project/project.h"
#include "src/importers/geoscan_project_importer.h"
#include "src/trajectory_importers/csv_trajectory_importer.h"
#include "src/calculators/depth_section_calculator/simple_depth_section_calculator.h"
#include "src/calculators/attribute_analysis_calculator/energy_attribute_analysis_calculator.h"
#include "src/calculators/attribute_analysis_calculator/cosine_phase_attribute_analysis_calculator.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    std::shared_ptr<Project> project;
    std::shared_ptr<GeoScanProjectImporter> geoscan_project_importer;
    std::shared_ptr<CsvTrajectoryImporter> csv_trajectory_importer;
    std::shared_ptr<SimpleDepthSectionCalculator> simple_depth_section_calculator;
    std::shared_ptr<EnergyAttributeAnalysisCalculator> energy_attribute_analysis_calculator;
    std::shared_ptr<CosinePhaseAttributeAnalysisCalculator> cosine_phase_attribute_analysis_calculator;
    void configure_slots();

public slots:
    void import_project_from_geoscan();
    void reflect();
    void remove_trace();
    void trim();
    void remove_air_wave();
    void calculate_depth_section();
    void calculate_energy();
    void calculate_cosine_phase();
    void remove_attribute_analysis();
    void import_trajectory_from_csv();
    void add_layer();
    void clear_layers();
    void export_layers_to_csv();

    void display_radargram();
    void radargram_mouse_press(QMouseEvent* event);
};

#endif // MAINWINDOW_H
