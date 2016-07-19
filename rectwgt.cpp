#include "RectWgt.h"

RectWgt::RectWgt(QWidget *parent) : QWidget(parent)
{
    pvbInL=new QVBoxLayout;
    pfrmLIn=new QFormLayout;
    pvbOutL=new QVBoxLayout;
    pvbPicL=new QVBoxLayout;
    phbL=new QHBoxLayout;
    pvbL=new QVBoxLayout;

    pgrpIn=new QGroupBox("Вводные данные:");
    pgrpPic=new QGroupBox("Графическое представление:");
    pgrpOut=new QGroupBox("Расчет:");

    pPix=new QPixmap(230,230);
    pPixLbl=new QLabel;
    pNoteLbl=new QLabel;
    pvbOutL->addWidget(pNoteLbl);
    pPixLbl->resize(230,230);
    pPixLbl->setPixmap(*pPix);
    pPixLbl->setAlignment(Qt::AlignCenter);
    pvbPicL->addWidget(pPixLbl);

    // Вводные данные______________________________________________________________

    QRegExp rgexp("([0-9]{,1})((\\.|\\,)([0-9]{0,3}))?");
    QRegExp rgexpZiro("([0]{,1})?((\\.|\\,)([0-9]{0,3}))?");
    QRegExpValidator* rgexpVal=new QRegExpValidator(rgexp,this);
    QRegExpValidator* rgexpValZiro=new QRegExpValidator(rgexpZiro,this);

    phL=new QLineEdit;
    phL->setFixedWidth(60);
    phL->setAlignment(Qt::AlignRight);
    phL->setValidator(rgexpVal);

    pHL=new QLineEdit;
    pHL->setAlignment(Qt::AlignRight);
    pHL->setFixedWidth(60);
    pHL->setValidator(rgexpVal);

    pWL=new QLineEdit;
    pWL->setAlignment(Qt::AlignRight);
    pWL->setFixedWidth(60);
    pWL->setValidator(rgexpVal);

    pnL=new QLineEdit;
    pnL->setAlignment(Qt::AlignRight);
    pnL->setFixedWidth(60);
    pnL->setValidator(rgexpValZiro);
    pnL->setToolTip("Для керамических лотков - 0,0134\nДля бетонных лотков - 0,0138\nДля кирпичных лотков - 0,0144");

    piL=new QLineEdit;
    piL->setAlignment(Qt::AlignRight);
    piL->setFixedWidth(60);
    piL->setValidator(rgexpValZiro);

    ph=new QLabel("<BODY><B>&h</B> - уровень воды в лотке, м;</BODY>");
    ph->setBuddy(phL);
    pH=new QLabel("<BODY><B>&H</B> - глубина лотка, м;</BODY>");
    pH->setBuddy(pHL);
    pW=new QLabel("<BODY><B>&W</B> - ширина лотка, м;</BODY>");
    pW->setBuddy(pWL);
    pn=new QLabel("<BODY><B>&n</B> - коэффициент шероховатости лотка;</BODY>");
    pn->setBuddy(pnL);
    pi=new QLabel("<BODY><B>&i</B> - гидравлический уклон;</BODY>");
    pi->setBuddy(piL);

    pfrmLIn->addRow(phL,ph);
    pfrmLIn->addRow(pHL,pH);
    pfrmLIn->addRow(pWL,pW);
    pfrmLIn->addRow(pnL,pn);
    pfrmLIn->addRow(piL,pi);

    valStr<<"Q"<<"v"<<"%"<<"h/D"<<"S"<<"x"<<"p"<<"w"<<"R"<<"C"<<"y";
    toolTipStr<<" л/с. - расход;"<<" м/с. - скорость потока;"
             <<"  - процент заполнения лотка;"<<"  - наполнение в долях D;"
            <<" м2. - площадь полного сечения лотка;"
           <<" м. - смоченный периметр;"<<" м. - периметр лотка;"
          <<" м2. - площадь поперечного сечения потока;"<<" м. - гидравлический радиус сечения;"
         <<" - коэффициент Шези;"<<" - показатель степени;";

    for(int i=0; i<valStr.size(); ++i)
    {
        valMap[valStr[i]]=0.0;
        pLblsOut[i]=new QLabel("<BODY><B>"+valStr[i]+"</B>="
                               +QString::number(valMap[valStr[i]],'f',2)+toolTipStr[i]+"</BODY>");
        pvbOutL->addWidget(pLblsOut[i]);
    }

    //Connect_______________________________________________________________________
    connect(phL,SIGNAL(editingFinished()),SLOT(slotPhLChanged()));
    connect(pHL,SIGNAL(editingFinished()),SLOT(slotPHLChanged()));
    connect(pWL,SIGNAL(editingFinished()),SLOT(slotPWLChanged()));
    connect(pnL,SIGNAL(editingFinished()),SLOT(slotPnLChanged()));
    connect(piL,SIGNAL(editingFinished()),SLOT(slotPiLChanged()));

    //Layouts_________________________________________________________________________

    pgrpIn->setLayout(pvbInL);
    pvbInL->addLayout(pfrmLIn);
    pgrpOut->setLayout(pvbOutL);
    pvbOutL->addStretch(1);
    pgrpPic->setLayout(pvbPicL);
    phbL->addWidget(pgrpIn);
    phbL->addWidget(pgrpPic);
    pvbL->addLayout(phbL);
    pvbL->addWidget(pgrpOut);
    setLayout(pvbL);

}
//Расчет_____________________________________________________________________________
void RectWgt::calc()
{
    //Omega w  площадь поперечного сечения потока, м2;
    valMap["w"]=phL->text().toDouble()*pWL->text().toDouble();

    //x смоченный периметр, м;
    valMap["x"]=(pWL->text().toDouble()+2*phL->text().toDouble());

    //p периметр лотка, м;
    valMap["p"]=pWL->text().toDouble()+2*pHL->text().toDouble();

    //R гидравлический радиус сечения, м;
    valMap["R"]=valMap["w"]/valMap["x"];

    //y показатель степени;
    valMap["y"]=2.5*qSqrt(pnL->text().toDouble())-0.13-0.75*qSqrt(valMap["R"])*(qSqrt(pnL->text().toDouble())-0.1);

    //C коэффициент Шези;
    valMap["C"]=(1/pnL->text().toDouble())*qPow(valMap["R"],valMap["y"]);

    //hD наполнение в долях D;
    valMap["h/D"]=phL->text().toDouble()/pHL->text().toDouble();

    //S площадь полного сечения лотка, м2;
    valMap["S"]=pHL->text().toDouble()*pWL->text().toDouble();

    //proc процент заполнения лотка, %;
    valMap["%"]=valMap["w"]*100/valMap["S"];

    //v скорость потока, м/с;
    valMap["v"]=valMap["C"]*qSqrt(valMap["R"]*piL->text().toDouble());

    //Q расход, л/с;
    valMap["Q"]=valMap["w"]*valMap["C"]*qSqrt(valMap["R"]*piL->text().toDouble())*1000;

    //add Labels to pgrpOut

    if(phL->text().toDouble()>=pHL->text().toDouble())
    {
        for(int i=0; i<valStr.size(); ++i)
        {
            pLblsOut[i]->setText("");
        }
        pNoteLbl->setText("Уровень воды не может быть больше значения глубины лотка.");
        drawRect();
    }
    else
    {
        pNoteLbl->setText("");
        for(int i=0; i<valStr.size(); ++i)
        {
            pLblsOut[i]->setText("<BODY><B>"+valStr[i]+"</B>="
                                 +QString::number(valMap[valStr[i]],'f',2)+toolTipStr[i]+"</BODY>");
        }
        draw();
    }
}


//графика____________________________________________________________________
void RectWgt::draw()
{
    pPix=new QPixmap(230,230);
    QPainter painter;
    painter.begin(pPix);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setPen(QPen(Qt::black,2,Qt::SolidLine));
    double scale=(pHL->text().toDouble()>pWL->text().toDouble())?(200/pHL->text().toDouble()):200/pWL->text().toDouble();
    painter.translate((115.0-scale*(pWL->text().toDouble()/2)),(115.0-scale*(pHL->text().toDouble()/2)));
    painter.setClipRect(0.0,((pHL->text().toDouble()-phL->text().toDouble())*scale),
                        scale*pWL->text().toDouble(),phL->text().toDouble()*scale);
    painter.save();
    painter.setPen(Qt::NoPen);
    QLinearGradient gradient(scale*pWL->text().toDouble()/2,((pHL->text().toDouble()-phL->text().toDouble())*scale),
                             scale*pWL->text().toDouble()/2,scale*pHL->text().toDouble());
    gradient.setColorAt(0,Qt::blue);
    gradient.setColorAt(1,Qt::darkBlue);
    painter.setBrush(gradient);

    painter.drawRect(0,0,scale*pWL->text().toDouble(),scale*pHL->text().toDouble());
    painter.setClipping(false);
    painter.restore();

    QPointF points[4]=
    {
        QPointF(0.0,0.0),
        QPointF(0.0,scale*pHL->text().toDouble()),
        QPointF(scale*pWL->text().toDouble(),scale*pHL->text().toDouble()),
        QPointF(scale*pWL->text().toDouble(),0.0)
    };
    painter.drawPolyline(points,4);
    painter.drawLine(0.0,((pHL->text().toDouble()-phL->text().toDouble())*scale),
                     (scale*pWL->text().toDouble()),((pHL->text().toDouble()-phL->text().toDouble())*scale));
    painter.end();
    pPixLbl->setPixmap(*pPix);

}

void RectWgt::drawRect()
{
    pPix=new QPixmap(230,230);
    QPainter painter;
    painter.begin(pPix);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setPen(QPen(Qt::black,2,Qt::SolidLine));
    double scale=(pHL->text().toDouble()>pWL->text().toDouble())?200/pHL->text().toDouble():200/pWL->text().toDouble();
    painter.translate((115.0-scale*(pWL->text().toDouble()/2)),(115.0-scale*(pHL->text().toDouble()/2)));
    QPointF points[4]=
    {
        QPointF(0.0,0.0),
        QPointF(0.0,scale*pHL->text().toDouble()),
        QPointF(scale*pWL->text().toDouble(),scale*pHL->text().toDouble()),
        QPointF(scale*pWL->text().toDouble(),0.0)
    };
    painter.drawPolyline(points,4);
    painter.end();
    pPixLbl->setPixmap(*pPix);
}


//Slots__________________________________________________________________________
void RectWgt::slotPhLChanged()
{
    if(!(phL->text()==""))
    {
        QString str=phL->text();
        if(str[str.length()-1]==',')
        {
            str[str.length()-1]='.';
            str.append("00");
            phL->setText(str);
        }
        if(str[1]==',')
        {
            str[1]='.';
            phL->setText(str);
        }
        if(str[str.length()-1]=='.')
        {
            str.append("00");
            phL->setText(str);
        }
        if(str[0]=='.')
        {
            str='0'+str;
            phL->setText(str);
        }
        if(str[0]==',')
        {
            str[0]='.';
            str='0'+str;
            phL->setText(str);
        }
    }
    if((phL->text()!="")&&(pHL->text()!="")&&(pWL->text()!="")&&(pnL->text()!="")&&(piL->text()!=""))
        calc();
}

void RectWgt::slotPHLChanged()
{
    if(!(pHL->text()==""))
    {
        QString str=pHL->text();
        if(str[str.length()-1]==',')
        {
            str[str.length()-1]='.';
            str.append("00");
            pHL->setText(str);
        }
        if(str[1]==',')
        {
            str[1]='.';
            pHL->setText(str);
        }
        if(str[str.length()-1]=='.')
        {
            str.append("00");
            pHL->setText(str);
        }
        if(str[0]=='.')
        {
            str='0'+str;
            pHL->setText(str);
        }
        if(str[0]==',')
        {
            str[0]='.';
            str='0'+str;
            pHL->setText(str);
        }
    }
    if((phL->text()!="")&&(pHL->text()!="")&&(pWL->text()!="")&&(pnL->text()!="")&&(piL->text()!=""))
        calc();
}

void RectWgt::slotPWLChanged()
{
    if(!(pWL->text()==""))
    {
        QString str=pWL->text();
        if(str[str.length()-1]==',')
        {
            str[str.length()-1]='.';
            str.append("00");
            pWL->setText(str);
        }
        if(str[1]==',')
        {
            str[1]='.';
            pWL->setText(str);
        }
        if(str[str.length()-1]=='.')
        {
            str.append("00");
            pWL->setText(str);
        }
        if(str[0]=='.')
        {
            str='0'+str;
            pWL->setText(str);
        }
        if(str[0]==',')
        {
            str[0]='.';
            str='0'+str;
            pWL->setText(str);
        }
    }
    if((phL->text()!="")&&(pHL->text()!="")&&(pWL->text()!="")&&(pnL->text()!="")&&(piL->text()!=""))
        calc();
}

void RectWgt::slotPnLChanged()
{
    if(!(pnL->text()==""))
    {
        QString str=pnL->text();
        if(str[1]==',')
        {
            str[1]='.';
            pnL->setText(str);
        }
        if(str[str.length()-1]==',')
        {
            str[str.length()-1]='.';
            str.append("00");
            pnL->setText(str);
        }
        if(str[str.length()-1]=='.')
        {
            str.append("00");
            pnL->setText(str);
        }
        if(str[0]=='.')
        {
            str='0'+str;
            pnL->setText(str);
        }
        if(str[0]==',')
        {
            str[0]='.';
            str='0'+str;
            pnL->setText(str);
        }
    }
    if((phL->text()!="")&&(pHL->text()!="")&&(pWL->text()!="")&&(pnL->text()!="")&&(piL->text()!=""))
        calc();
}

void RectWgt::slotPiLChanged()
{

    if(!(piL->text()==""))
    {
        QString str=piL->text();
        if(str[1]==',')
        {
            str[1]='.';
            piL->setText(str);
        }
        if(str[str.length()-1]==',')
        {
            str[str.length()-1]='.';
            str.append("00");
            piL->setText(str);
        }
        if(str[str.length()-1]=='.')
        {
            str.append("00");
            piL->setText(str);
        }
        if(str[0]=='.')
        {
            str='0'+str;
            piL->setText(str);
        }
        if(str[0]==',')
        {
            str[0]='.';
            str='0'+str;
            piL->setText(str);
        }
    }
    if((phL->text()!="")&&(pHL->text()!="")&&(pWL->text()!="")&&(pnL->text()!="")&&(piL->text()!=""))
        calc();
}
