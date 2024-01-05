#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "src/ui/workers/attribute_analysis_worker.hpp"
#include "modal_progress_dialog.hpp"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), project(nullptr),
                                          geoscan_project_importer(std::make_shared<GeoScanProjectImporter>()),
                                          csv_trajectory_importer(std::make_shared<CsvTrajectoryImporter>()),
                                          simple_depth_section_calculator(
                                                  std::make_shared<SimpleDepthSectionCalculator>()),
                                          energy_attribute_analysis_calculator(
                                                  std::make_shared<EnergyAttributeAnalysisCalculator>()),
                                          cosine_phase_attribute_analysis_calculator(
                                                  std::make_shared<CosinePhaseAttributeAnalysisCalculator>()) {
    ui->setupUi(this);
    configure_plot_widgets();
    QObject::connect(ui->radargram_plot, &QCustomPlot::mousePress, this, &MainWindow::radargram_mouse_press);
    configure_slots();
}


MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::configure_slots() {
    QObject::connect(ui->action_import_from_geoscan, &QAction::triggered, this, &MainWindow::import_project_from_geoscan);
    QObject::connect(ui->action_reflect, &QAction::triggered, this, &MainWindow::reflect);
    QObject::connect(ui->action_remove_trace, &QAction::triggered, this, &MainWindow::remove_trace);
    QObject::connect(ui->action_trim, &QAction::triggered, this, &MainWindow::trim);
    QObject::connect(ui->action_remove_air_wave, &QAction::triggered, this, &MainWindow::remove_air_wave);
    QObject::connect(ui->action_show_info, &QAction::triggered, this, &MainWindow::show_info);
    QObject::connect(ui->action_calculate_depth_section, &QAction::triggered, this, &MainWindow::calculate_depth_section);
    QObject::connect(ui->action_energy, &QAction::triggered, this, &MainWindow::calculate_energy);
    QObject::connect(ui->action_cosine_phase, &QAction::triggered, this, &MainWindow::calculate_cosine_phase);
    QObject::connect(ui->action_remove_attribute_analysis, &QAction::triggered, this, &MainWindow::remove_attribute_analysis);
    QObject::connect(ui->action_import_trajectory_from_csv, &QAction::triggered, this, &MainWindow::import_trajectory_from_csv);
    QObject::connect(ui->action_add_layer, &QAction::triggered, this, &MainWindow::add_layer);
    QObject::connect(ui->action_clear_layers, &QAction::triggered, this, &MainWindow::clear_layers);
    QObject::connect(ui->action_export_layers_to_csv_, &QAction::triggered, this, &MainWindow::export_layers_to_csv);
}


void MainWindow::configure_plot_widgets() {
    ui->radargram_plot->axisRect()->setupFullAxesBox(true);
    ui->radargram_plot->xAxis->setLabel("Distance, m");
    ui->radargram_plot->yAxis->setLabel("Time, ns");
    ui->radargram_plot->yAxis->setRangeReversed(true);
    ui->trace_plot->yAxis->setLabel("Time, ns");
    ui->trace_plot->xAxis->setLabel("Amplitude");
}


void MainWindow::display_trace() {
    auto radargram = project->get_radargram();
    auto position = project->get_position();

    ui->trace_plot->clearPlottables();
    ui->trace_plot->yAxis->setRangeReversed(true);
    ui->trace_plot->addGraph(ui->trace_plot->yAxis, ui->trace_plot->xAxis);
    QVector<double> x_trace(project->get_sample_count()), y_trace(project->get_sample_count());
    for (int sample = 0; sample < project->get_sample_count(); ++sample) {
        x_trace[sample] = project->sample_to_time(sample);
        y_trace[sample] = radargram->trace_matrix(position.trace, sample);
    }
    ui->trace_plot->graph(0)->setData(x_trace, y_trace);
    ui->trace_plot->yAxis->setRange(0, project->get_time_max__ns());
    ui->trace_plot->xAxis->setRange(radargram->trace_matrix.minCoeff(), radargram->trace_matrix.maxCoeff());

    display_position_on_trace();
    ui->trace_plot->replot();
}

void MainWindow::display_position_on_trace() {
    auto radargram = project->get_radargram();
    auto position = project->get_position();
    ui->trace_plot->addGraph(ui->trace_plot->yAxis, ui->trace_plot->xAxis);
    QVector<double> x_trace_position(1, project->sample_to_time(position.sample));
    QVector<double> y_trace_position(1, radargram->trace_matrix(position.trace, position.sample));
    ui->trace_plot->graph(1)->setData(x_trace_position, y_trace_position);
    ui->trace_plot->graph(1)->setScatterStyle(QCPScatterStyle::ssPlus);
    ui->trace_plot->graph(1)->setPen(QPen(QBrush(Qt::red), 2));
}


void MainWindow::display_trajectory() {
    auto trajectory = project->get_trajectory();
    if (!trajectory->has_trajectory) {
        ui->trajectory_plot->setVisible(false);
        return;
    }
    ui->trajectory_plot->setVisible(true);
    ui->trajectory_plot->clearPlottables();
    ui->trajectory_plot->addGraph();
    QVector<double> x(project->get_trace_count()), y(project->get_trace_count());
    for (int trace = 0; trace < project->get_trace_count(); ++trace) {
        x[trace] = trajectory->x(trace);
        y[trace] = trajectory->y(trace);
    }
    ui->trajectory_plot->graph(0)->setData(x, y);
    ui->trajectory_plot->xAxis->setRange(trajectory->x.minCoeff(), trajectory->x.maxCoeff());
    ui->trajectory_plot->yAxis->setRange(trajectory->y.minCoeff(), trajectory->y.maxCoeff());
    ui->trajectory_plot->graph(0)->setScatterStyle(QCPScatterStyle::ssDot);
    ui->trajectory_plot->graph(0)->setPen(QPen(QBrush(Qt::blue), 2));
    ui->trajectory_plot->graph(0)->setLineStyle(QCPGraph::lsNone);

    display_position_on_trajectory();
    ui->trajectory_plot->replot();
}


void MainWindow::display_position_on_trajectory() {
    auto trajectory = project->get_trajectory();
    auto position = project->get_position();
    ui->trajectory_plot->addGraph();
    QVector<double> x_traj_position(1, trajectory->x(position.trace));
    QVector<double> y_traj_position(1, trajectory->y(position.trace));
    ui->trajectory_plot->graph(1)->setData(x_traj_position, y_traj_position);
    ui->trajectory_plot->graph(1)->setScatterStyle(QCPScatterStyle::ssPlus);
    ui->trajectory_plot->graph(1)->setPen(QPen(QBrush(Qt::red), 2));
}


void MainWindow::display_radargram() {
    if (project == nullptr) {
        return;
    }
    auto radargram = project->get_radargram();
    auto attribute_analysis = project->get_attribute_analysis();

    ui->radargram_plot->clearPlottables();
    ui->radargram_plot->xAxis->setRange(0, project->get_distance_max__m());
    ui->radargram_plot->yAxis->setRange(0, project->get_time_max__ns());

    auto color_map = new QCPColorMap(ui->radargram_plot->xAxis, ui->radargram_plot->yAxis);
    color_map->data()->setSize(project->get_trace_count(), project->get_sample_count());
    color_map->data()->setRange(QCPRange(0, project->get_distance_max__m()), QCPRange(0, project->get_time_max__ns()));
    float value;
    for (int trace = 0; trace < project->get_trace_count(); ++trace) {
        for (int sample = 0; sample < project->get_sample_count(); ++sample) {
            if (attribute_analysis->has_attribute_analysis) {
                value = attribute_analysis->attribute_matrix(trace, sample);
            } else {
                value = radargram->trace_matrix(trace, sample);
            }
            color_map->data()->setCell(trace, sample, value);
        }
    }

    auto color_scale = new QCPColorScale(ui->radargram_plot);
    ui->radargram_plot->plotLayout()->addElement(0, 1, color_scale);
    color_scale->setType(QCPAxis::atRight);
    color_map->setColorScale(color_scale);
    color_map->setGradient(QCPColorGradient::gpGrayscale);
    color_map->rescaleDataRange();
    auto margin_group = new QCPMarginGroup(ui->radargram_plot);
    ui->radargram_plot->axisRect()->setMarginGroup(QCP::msBottom | QCP::msTop, margin_group);
    color_scale->setMarginGroup(QCP::msBottom | QCP::msTop, margin_group);
    ui->radargram_plot->rescaleAxes();
    ui->radargram_plot->replot();

    display_position_on_radargram();
    display_layers();
    display_depth_section();
}


void MainWindow::display_position_on_radargram() {
    auto position = project->get_position();
    ui->radargram_plot->addGraph();
    QVector<double> x(1, project->trace_to_distance(position.trace));
    QVector<double> y(1, project->sample_to_time(position.sample));
    ui->radargram_plot->graph(0)->setData(x, y);
    ui->radargram_plot->graph(0)->setScatterStyle(QCPScatterStyle::ssPlus);
    ui->radargram_plot->graph(0)->setPen(QPen(QBrush(Qt::red), 2));
    ui->radargram_plot->replot();
}


void MainWindow::display_depth_section() {
    auto depth_section = project->get_depth_section();
    if (!depth_section->has_depth_section) {
        ui->radargram_plot->yAxis2->setVisible(false);
    } else {
        ui->radargram_plot->yAxis2->setVisible(true);
        ui->radargram_plot->yAxis2->setRange(0, depth_section->depth_vector[depth_section->depth_vector.size() - 1]);
        ui->radargram_plot->yAxis2->setLabel("Depth, m");
        ui->radargram_plot->yAxis2->setTickLabels(true);
        ui->radargram_plot->yAxis2->setRangeReversed(true);
        ui->radargram_plot->addGraph(ui->radargram_plot->yAxis2);
    }
    ui->radargram_plot->replot();
}


void MainWindow::display_layers() {
    int layer_index = 1;
    for (const auto& layer : project->get_layers()) {
        ui->radargram_plot->addGraph();
        QVector<double> x_layer(project->get_trace_count());
        QVector<double> y_layer(project->get_trace_count());
        for (int trace = 0; trace < project->get_trace_count(); ++trace) {
            x_layer[trace] = project->trace_to_distance(trace);
            y_layer[trace] = project->sample_to_time(layer->sample_vector(trace));
        }
        ui->radargram_plot->graph(layer_index)->setData(x_layer, y_layer);
        ui->radargram_plot->graph(layer_index)->setPen(QPen(QBrush(Qt::blue), 2));
        ++layer_index;
    }
    ui->radargram_plot->replot();
}


void MainWindow::display_all() {
    display_radargram();
    display_trace();
    display_trajectory();
}


void MainWindow::radargram_mouse_press(QMouseEvent* event) {
    QPoint point = event->pos();
    double distance__m = ui->radargram_plot->xAxis->pixelToCoord(point.x());
    double time__ns = ui->radargram_plot->yAxis->pixelToCoord(point.y());
    int trace = project->distance_to_trace(distance__m);
    int sample = project->time_to_sample(time__ns);
    try {
        project->set_position(trace, sample);
        display_all();
    } catch (const std::runtime_error& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}


void MainWindow::import_project_from_geoscan() {
    QString filename = QFileDialog::getOpenFileName(this, "Import from GeoScan", "", "GeoScan radargram (*.gpr2)");
    if (filename == "") {
        return;
    }
    try {
        project = geoscan_project_importer->import_project(filename.toStdString());
        display_all();
    } catch (const std::runtime_error& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}


void MainWindow::reflect() {
    if (project == nullptr) {
        return;
    }
    try {
        project->reflect();
        display_all();
    } catch (const std::runtime_error& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}


void MainWindow::remove_trace() {
    if (project == nullptr) {
        return;
    }
    try {
        project->remove_trace(project->get_position().trace);
        display_all();
    } catch (const std::runtime_error& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}


void MainWindow::trim() {
    if (project == nullptr) {
        return;
    }
    try {
        project->trim(project->get_position().sample);
        display_all();
    } catch (const std::runtime_error& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}


void MainWindow::remove_air_wave() {
    if (project == nullptr) {
        return;
    }
    try {
        project->remove_air_wave();
        display_all();
    } catch (const std::runtime_error& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}


void MainWindow::calculate_depth_section() {
    if (project == nullptr) {
        return;
    }
    try {
        project->calculate_depth_section(simple_depth_section_calculator);
        display_all();
    } catch (const std::runtime_error& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}


void MainWindow::calculate_attribute_analysis(std::shared_ptr<IAttributeAnalysisCalculator> attribute_analysis_calculator) {
    AttributeAnalysisWorker worker(project, attribute_analysis_calculator);
    QThreadPool thread_pool;
    thread_pool.setMaxThreadCount(1);

    ModalProgressDialog modal_dialog(this);
    QObject::connect(&worker, &AttributeAnalysisWorker::work_finished, &modal_dialog, &QDialog::accept);
    QObject::connect(&worker, &AttributeAnalysisWorker::work_cancelled, &modal_dialog, &QDialog::reject);
    QObject::connect(&modal_dialog, &QDialog::rejected, &worker, &AttributeAnalysisWorker::stop_work);
    QObject::connect(&worker, &AttributeAnalysisWorker::work_finished, this, &MainWindow::display_all);

    AttributeAnalysisWorkerRunnable *runnable = new AttributeAnalysisWorkerRunnable(&worker);
    thread_pool.start(runnable);
    modal_dialog.exec();
    thread_pool.waitForDone();
}


void MainWindow::calculate_energy() {
    if (project == nullptr) {
        return;
    }
    calculate_attribute_analysis(energy_attribute_analysis_calculator);
}


void MainWindow::calculate_cosine_phase() {
    if (project == nullptr) {
        return;
    }
    calculate_attribute_analysis(cosine_phase_attribute_analysis_calculator);
}


void MainWindow::remove_attribute_analysis() {
    if (project == nullptr) {
        return;
    }
    try {
        project->remove_attribute_analysis();
        display_all();
    } catch (const std::runtime_error& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}


void MainWindow::import_trajectory_from_csv() {
    if (project == nullptr) {
        return;
    }
    QString filename = QFileDialog::getOpenFileName(this, "Import Trajectory from CSV", "", "CSV (*.csv)");
    if (filename == "") {
        return;
    }
    try {
        project->import_trajectory(csv_trajectory_importer, filename.toStdString());
        display_all();
    } catch (const std::runtime_error& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}


void MainWindow::add_layer() {
    if (project == nullptr) {
        return;
    }
    QString layer_name = QInputDialog::getText(this, "Layer Name", "");
    if (layer_name.isEmpty()) {
        return;
    }
    try {
        project->add_layer(layer_name.toStdString());
        display_all();
    } catch (const std::runtime_error& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}


void MainWindow::clear_layers() {
    if (project == nullptr) {
        return;
    }
    try {
        project->clear_layers();
        display_all();
    } catch (const std::runtime_error& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}


void MainWindow::export_layers_to_csv() {
    if (project == nullptr) {
        return;
    }
    QString filename = QFileDialog::getSaveFileName(this, "Export Layers to CSV", "", "CSV (*.csv)");
    if (filename == "") {
        return;
    }
    try {
        project->export_layers_to_csv(filename.toStdString());
    } catch (const std::runtime_error& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}


void MainWindow::show_info() {
    if (project == nullptr) {
        return;
    }
    try {

        std::string text = project->get_info();
        QMessageBox::information(this, "Info about Radargram", QString::fromStdString(text));
    } catch (const std::runtime_error& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}
