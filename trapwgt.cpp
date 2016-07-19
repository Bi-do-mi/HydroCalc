#include "TrapWgt.h"

TrapWgt::TrapWgt(QWidget *parent) : QWidget(parent)
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

    pWdownL=new QLineEdit;
    pWdownL->setAlignment(Qt::AlignRight);
    pWdownL->setFixedWidth(60);
    pWdownL->setValidator(rgexpVal);

    pWupL=new QLineEdit;
    pWupL->setAlignment(Qt::AlignRight);
    pWupL->setFixedWidth(60);
    pWupL->setValidator(rgexpVal);

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
    pWdown=new QLabel("<BODY><B>&Wdown</B> - ширина по дну лотка, м;</BODY>");
    pWdown->setBuddy(pWdownL);
    pWup=new QLabel("<BODY><B>W&up</B> - ширина по верху лотка, м;</BODY>");
    pWup->setBuddy(pWupL);
    pn=new QLabel("<BODY><B>&n</B> - коэффициент шероховатости лотка;</BODY>");
    pn->setBuddy(pnL);
    pi=new QLabel("<BODY><B>&i</B> - гидравлический уклон;</BODY>");
    pi->setBuddy(piL);

    pfrmLIn->addRow(phL,ph);
    pfrmLIn->addRow(pHL,pH);
    pfrmLIn->addRow(pWdownL,pWdown);
    pfrmLIn->addRow(pWupL,pWup);
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
    connect(pWdownL,SIGNAL(editingFinished()),SLOT(slotPWdownLChanged()));
    connect(pWupL,SIGNAL(editingFinished()),SLOT(slotPWupLChanged()));
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
void TrapWgt::calc()
{
    //Omega w  площадь поперечного сечения потока, м2;
    valMap["w"]=phL->text().toDouble()*pWdownL->text().toDouble()+phL->text().toDouble()*
            (((pWupL->text().toDouble()-pWdownL->text().toDouble())/2)/pHL->text().toDouble())*phL->text().toDouble();

    //x смоченный периметр, м;
    valMap["x"]=(pWdownL->text().toDouble()+2*qSqrt(qPow(phL->text().toDouble()*
                           (pWupL->text().toDouble()-pWdownL->text().toDouble())/2/pHL->text().toDouble(),2)
                                                    +qPow(phL->text().toDouble(),2)));

    //p периметр лотка, м;
    valMap["p"]=(pWdownL->text().toDouble()+2*qSqrt(qPow(((pWupL->text().toDouble()-pWdownL->text().toDouble())/2),2)
                                                    +qPow(pHL->text().toDouble(),2)));

    //R гидравлический радиус сечения, м;
    valMap["R"]=valMap["w"]/valMap["x"];

    //y показатель степени;
    valMap["y"]=2.5*qSqrt(pnL->text().toDouble())-0.13-0.75*qSqrt(valMap["R"])*(qSqrt(pnL->text().toDouble())-0.1);

    //C коэффициент Шези;
    valMap["C"]=(1/pnL->text().toDouble())*qPow(valMap["R"],valMap["y"]);

    //hD наполнение в долях D;
    valMap["h/D"]=phL->text().toDouble()/pHL->text().toDouble();

    //S площадь полного сечения лотка, м2;
    valMap["S"]=pHL->text().toDouble()*pWdownL->text().toDouble()+((pWupL->text().toDouble()-pWdownL->text().toDouble())/2)
            *pWdownL->text().toDouble();

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
    else if(pWdownL->text().toDouble()>pWupL->text().toDouble())
    {
        for(int i=0; i<valStr.size(); ++i)
        {
            pLblsOut[i]->setText("");
        }
        pNoteLbl->setText("Дно лотка должно быть меньше верха.");
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
void TrapWgt::draw()
{
    pPix=new QPixmap(230,230);
    QPainter painter;
    painter.begin(pPix);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setPen(QPen(Qt::black,2,Qt::SolidLine));
    double scale=(pHL->text().toDouble()>pWupL->text().toDouble())?(200/pHL->text().toDouble()):200/pWupL->text().toDouble();
    painter.translate((115.0-scale*(pWupL->text().toDouble()/2)),(115.0-scale*(pHL->text().toDouble()/2)));

    painter.setClipRect(0.0,((pHL->text().toDouble()-phL->text().toDouble())*scale),
                        scale*pWupL->text().toDouble(),phL->text().toDouble()*scale);
    painter.save();
    painter.setPen(Qt::NoPen);
    QLinearGradient gradient(scale*pWupL->text().toDouble()/2,((pHL->text().toDouble()-phL->text().toDouble())*scale),
                             scale*pWupL->text().toDouble()/2,scale*pHL->text().toDouble());
    gradient.setColorAt(0,Qt::blue);
    gradient.setColorAt(1,Qt::darkBlue);
    painter.setBrush(gradient);
    QPointF points[4]=
    {
        QPointF(0.0,0.0),
        QPointF((scale*(pWupL->text().toDouble()-pWdownL->text().toDouble())/2),scale*pHL->text().toDouble()),
        QPointF(scale*(pWdownL->text().toDouble()+(pWupL->text().toDouble()-pWdownL->text().toDouble())/2),
        scale*pHL->text().toDouble()),
        QPointF(scale*pWupL->text().toDouble(),0.0),
     };
    //painter.drawRect(0,0,scale*pWupL->text().toDouble(),scale*pHL->text().toDouble());
    painter.drawPolygon(points,4);
    painter.setClipping(false);
    painter.restore();

    painter.drawPolyline(points,4);
    painter.drawLine(scale*(((pHL->text().toDouble()-phL->text().toDouble())
                             *((pWupL->text().toDouble()-pWdownL->text().toDouble())/2/pHL->text().toDouble()))),
                     ((pHL->text().toDouble()-phL->text().toDouble())*scale),
                     scale*(pWupL->text().toDouble()-((pHL->text().toDouble()-phL->text().toDouble())
                                                      *((pWupL->text().toDouble()-pWdownL->text().toDouble())
                                                        /2/pHL->text().toDouble()))),
                     ((pHL->text().toDouble()-phL->text().toDouble())*scale));
    painter.end();
    pPixLbl->setPixmap(*pPix);

}

void TrapWgt::drawRect()
{
    pPix=new QPixmap(230,230);
    QPainter painter;
    painter.begin(pPix);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setPen(QPen(Qt::black,2,Qt::SolidLine));
    double scale=(pHL->text().toDouble()>pWupL->text().toDouble())?200/pHL->text().toDouble():200/pWupL->text().toDouble();
    painter.translate((115.0-scale*(pWupL->text().toDouble()/2)),(115.0-scale*(pHL->text().toDouble()/2)));
    QPointF points[4]=
    {
        QPointF(0.0,0.0),
        QPointF((scale*(pWupL->text().toDouble()-pWdownL->text().toDouble())/2),scale*pHL->text().toDouble()),
        QPointF(scale*(pWdownL->text().toDouble()+(pWupL->text().toDouble()-pWdownL->text().toDouble())/2),
        scale*pHL->text().toDouble()),
        QPointF(scale*pWupL->text().toDouble(),0.0)
    };
    painter.drawPolyline(points,4);
    painter.end();
    pPixLbl->setPixmap(*pPix);
}


//Slots__________________________________________________________________________
void TrapWgt::slotPhLChanged()
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
    if((phL->text()!="")&&(pHL->text()!="")&&(pWdownL->text()!="")&&(pWupL->text()!="")&&(pnL->text()!="")&&(piL->text()!=""))
        calc();
}

void TrapWgt::slotPHLChanged()
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
    if((phL->text()!="")&&(pHL->text()!="")&&(pWdownL->text()!="")&&(pWupL->text()!="")&&(pnL->text()!="")&&(piL->text()!=""))
        calc();
}

void TrapWgt::slotPWdownLChanged()
{
    if(!(pWdownL->text()==""))
    {
        QString str=pWdownL->text();
        if(str[str.length()-1]==',')
        {
            str[str.length()-1]='.';
            str.append("00");
            pWdownL->setText(str);
        }
        if(str[1]==',')
        {
            str[1]='.';
            pWdownL->setText(str);
        }
        if(str[str.length()-1]=='.')
        {
            str.append("00");
            pWdownL->setText(str);
        }
        if(str[0]=='.')
        {
            str='0'+str;
            pWdownL->setText(str);
        }
        if(str[0]==',')
        {
            str[0]='.';
            str='0'+str;
            pWdownL->setText(str);
        }
    }
    if((phL->text()!="")&&(pHL->text()!="")&&(pWdownL->text()!="")&&(pWupL->text()!="")&&(pnL->text()!="")&&(piL->text()!=""))
        calc();
}

void TrapWgt::slotPWupLChanged()
{
    if(!(pWupL->text()==""))
    {
        QString str=pWupL->text();
        if(str[str.length()-1]==',')
        {
            str[str.length()-1]='.';
            str.append("00");
            pWupL->setText(str);
        }
        if(str[1]==',')
        {
            str[1]='.';
            pWupL->setText(str);
        }
        if(str[str.length()-1]=='.')
        {
            str.append("00");
            pWupL->setText(str);
        }
        if(str[0]=='.')
        {
            str='0'+str;
            pWupL->setText(str);
        }
        if(str[0]==',')
        {
            str[0]='.';
            str='0'+str;
            pWupL->setText(str);
        }
    }
    if((phL->text()!="")&&(pHL->text()!="")&&(pWdownL->text()!="")&&(pWupL->text()!="")&&(pnL->text()!="")&&(piL->text()!=""))
        calc();
}

void TrapWgt::slotPnLChanged()
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
    if((phL->text()!="")&&(pHL->text()!="")&&(pWdownL->text()!="")&&(pWupL->text()!="")&&(pnL->text()!="")&&(piL->text()!=""))
        calc();
}

void TrapWgt::slotPiLChanged()
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
    if((phL->text()!="")&&(pHL->text()!="")&&(pWdownL->text()!="")&&(pWupL->text()!="")&&(pnL->text()!="")&&(piL->text()!=""))
        calc();
}
