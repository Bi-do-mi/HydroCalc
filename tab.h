#ifndef TAB_H
#define TAB_H

#include"pipewgt.h"
#include"rectpipewgt.h"
#include"rectwgt.h"
#include"trapwgt.h"

class Tab : public QWidget
{
    Q_OBJECT
private:
 QTabWidget* tab;

public:
    Tab(QWidget *parent = 0);
    ~Tab();
};

#endif // TAB_H
