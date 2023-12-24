#include "src/widgets/mainwindow.h"
#include "src/domain/radargram.h"

#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    Radargram r(10, 20);
    std::cout << r.x << std::endl;

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
