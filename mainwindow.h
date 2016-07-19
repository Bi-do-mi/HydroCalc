#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<tab.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QMenu* file;
    QMenu* about;
    QAction* actAbout;
    QAction* actQuit;
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void slotAbout();
    void slotQuit();
};

#endif // MAINWINDOW_H
