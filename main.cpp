#include "mainwindow.h"
#include"tab.h"
#include"pipewgt.h"
#include <QApplication>
#include <QWidget>
#include<QtWidgets>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.resize(800,600);
    w.show();

    return a.exec();
}
