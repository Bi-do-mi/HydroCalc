#include "tab.h"

Tab::Tab(QWidget *parent)
    : QWidget(parent)
{
    tab=new QTabWidget;
    tab->addTab(new PipeWgt, "Круглая труба");
    tab->addTab(new RectPipeWgt, "Прямоугольная труба");
    tab->addTab(new RectWgt, "Прямоугольный лоток");
    tab->addTab(new TrapWgt, "Трапецеидальний лоток");

    QHBoxLayout* lt=new QHBoxLayout;
    lt->addWidget(tab);
    setLayout(lt);
}

Tab::~Tab()
{

}
