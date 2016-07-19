#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QIcon iconAbout("water.ico");
    setWindowIcon(iconAbout);
    file=new QMenu("&Файл");
    about=new QMenu("&Справка");
    actQuit=new QAction("&Выход", this);
    actQuit->setShortcut(Qt::CTRL+Qt::Key_Q);
    actAbout=new QAction("&О программе", this);
    connect(actAbout,SIGNAL(triggered(bool)),SLOT(slotAbout()));
    connect(actQuit,SIGNAL(triggered(bool)),SLOT(slotQuit()));
    file->addAction(actQuit);
    about->addAction(actAbout);
    menuBar()->addMenu(file);
    menuBar()->addMenu(about);

    Tab* ptab=new Tab(this);
    setCentralWidget(ptab);
}

MainWindow::~MainWindow()
{

}

void MainWindow::slotAbout()
{
    QString strAbout="<p><span style=font-size:16px><strong>HydroCalc</strong><br />v. Haznidon</span></p>"
            "<p>Задача данной программы в вычислении и графическом представлении пропускной "
            "способности воды труб и лотков прямоугольного и трапецеидального сечения.<br />"
            "Данная программа применима только к спокойному типу потока жидкости.<br />"
            "Принцип расчета основан на формуле академика Н.Н. Павловского.</p>"
            "<p>Разработал:&nbsp;Джигкаев Марат<br />"
            "email:&nbsp;bidomi@mail.ru<br />"
            "Дата: 18.07.2016</p>";

    QMessageBox::about(this,"О программе",strAbout);
}

void MainWindow::slotQuit()
{
    QApplication::quit();
}
