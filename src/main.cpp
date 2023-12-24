#include "src/widgets/mainwindow.h"
#include "src/domain/radargram.h"

#include <QApplication>
#include <iostream>

#include <Eigen/Dense>
using Eigen::MatrixXd;

int main(int argc, char *argv[])
{
    MatrixXd m(2,2);
    m(0,0) = 3;
    m(1,0) = 2.5;
    m(0,1) = -1;
    m(1,1) = m(1,0) + m(0,1);
    std::cout << m << std::endl;

    Radargram r(10, 20);
    std::cout << r.x << std::endl;

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
