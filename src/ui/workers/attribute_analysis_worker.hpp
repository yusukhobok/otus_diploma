#ifndef GPR_READER_ATTRIBUTE_ANALYSIS_WORKER_H
#define GPR_READER_ATTRIBUTE_ANALYSIS_WORKER_H

#include <QCoreApplication>
#include <QMainWindow>
#include <QThreadPool>
#include <QProgressDialog>
#include <QRunnable>
#include <QThread>
#include "src/project/project.h"


class AttributeAnalysisWorker : public QObject {
Q_OBJECT

public:
    AttributeAnalysisWorker(std::shared_ptr<Project> project,
                            std::shared_ptr<IAttributeAnalysisCalculator> attribute_analysis_calculator) : project(
            project), attribute_analysis_calculator(attribute_analysis_calculator) {}

public slots:

    void calculate() {
        project->calculate_attribute_analysis(attribute_analysis_calculator);
        emit work_finished();
    }

signals:

    void work_finished();

    void work_cancelled();

public slots:

    void stop_work() {
        stop_requested = true;
    }

private:
    std::shared_ptr<Project> project;
    std::shared_ptr<IAttributeAnalysisCalculator> attribute_analysis_calculator;
    bool stop_requested = false;
};


class AttributeAnalysisWorkerRunnable : public QRunnable {
public:
    AttributeAnalysisWorkerRunnable(AttributeAnalysisWorker *worker) : worker(worker) {}

    void run() override {
        worker->calculate();
    }

private:
    AttributeAnalysisWorker *worker;
};


#endif //GPR_READER_ATTRIBUTE_ANALYSIS_WORKER_H
