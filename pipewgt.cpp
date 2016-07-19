#include "pipewgt.h"

PipeWgt::PipeWgt(QWidget *parent) : QWidget(parent)
{
    betta=0.0;
    Rad=0.0;
    Striang=0.0;
    b=0.0;
    hTriang=0.0;

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

    pDL=new QLineEdit;
    pDL->setAlignment(Qt::AlignRight);
    pDL->setFixedWidth(60);
    pDL->setValidator(rgexpVal);

    pnL=new QLineEdit;
    pnL->setAlignment(Qt::AlignRight);
    pnL->setFixedWidth(60);
    pnL->setValidator(rgexpValZiro);
    pnL->setToolTip("Для керамических труб - 0,0134\nДля бетонных труб - 0,0138\nДля кирпичных коллекторов - 0,0144");

    piL=new QLineEdit;
    piL->setAlignment(Qt::AlignRight);
    piL->setFixedWidth(60);
    piL->setValidator(rgexpValZiro);

    ph=new QLabel("<BODY><B>&h</B> - уровень воды в трубе, м;</BODY>");
    ph->setBuddy(phL);
    pD=new QLabel("<BODY><B>&D</B> - диаметр трубы, м;</BODY>");
    pD->setBuddy(pDL);
    pn=new QLabel("<BODY><B>&n</B> - коэффициент шероховатости трубы;</BODY>");
    pn->setBuddy(pnL);
    pi=new QLabel("<BODY><B>&i</B> - гидравлический уклон;</BODY>");
    pi->setBuddy(piL);

    pfrmLIn->addRow(phL,ph);
    pfrmLIn->addRow(pDL,pD);
    pfrmLIn->addRow(pnL,pn);
    pfrmLIn->addRow(piL,pi);

    valStr<<"Q"<<"v"<<"%"<<"h/D"<<"r"<<"S"<<"x"<<"p"<<"w"<<"R"<<"C"<<"y";
    toolTipStr<<" л/с. - расход;"<<" м/с. - скорость потока;"
             <<"  - процент заполнения трубы;"<<"  - наполнение в долях D;"
            <<" м. - радиус трубы;"<<" м2. - площадь полного сечения трубы;"
           <<" м. - смоченный периметр;"<<" м. - периметр окружности;"
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
    connect(pDL,SIGNAL(editingFinished()),SLOT(slotPDLChanged()));
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
void PipeWgt::calc()
{
    //r радиус трубы, м;
    valMap["r"]=pDL->text().toDouble()/2;

    //hTriang высота равнобедренного треугольника;
    hTriang=qFabs(valMap["r"]-phL->text().toDouble());

    //b основание треугольника вписанного в окружность;
    b=qSqrt(qPow(valMap["r"],2.0)-qPow(hTriang,2.0))*2;

    //Striang площадь треугольника, м2;
    Striang=0.5*b*qSqrt(qPow(valMap["r"],2.0)-qPow(b,2.0)/4);

    //Rad радиус описанной окружности треугольника;
    Rad=(hTriang==0)?0.0:(qPow(valMap["r"],2.0)*b)/(4*Striang);

    //Betta внутр. угол треугольника между равнобедр. стор.
    betta=(!hTriang)?0.0:(hTriang<=(valMap["r"]/2+valMap["r"]/4))
            ?(180-qRadiansToDegrees(qAsin(b/(2*Rad))))
           :qRadiansToDegrees(qAsin(b/(2*Rad)));

    //Omega w  площадь поперечного сечения потока, м2;
    valMap["w"]=(phL->text().toDouble()>valMap["r"])?(PI*qPow(valMap["r"],2.0)/360)*((phL->text().toDouble()>valMap["r"])?
                360-betta:(phL->text().toDouble()==valMap["r"])?180:betta)+Striang:
        (PI*qPow(valMap["r"],2.0)/360)*((phL->text().toDouble()>valMap["r"])?
      360-betta:(phL->text().toDouble()==valMap["r"])?180:betta)-Striang;

    //x смоченный периметр, м;
    valMap["x"]=(PI*valMap["r"]*(phL->text().toDouble()<valMap["r"]?betta:(360-betta)))/180;

    //p периметр окружности, м;
    valMap["p"]=(PI*valMap["r"]*360)/180;

    //R гидравлический радиус сечения, м;
    valMap["R"]=valMap["w"]/valMap["x"];

    //y показатель степени;
    valMap["y"]=2.5*qSqrt(pnL->text().toDouble())-0.13-0.75*qSqrt(valMap["R"])*(qSqrt(pnL->text().toDouble())-0.1);

    //C коэффициент Шези;
    valMap["C"]=(1/pnL->text().toDouble())*qPow(valMap["R"],valMap["y"]);

    //hD наполнение в долях D;
    valMap["h/D"]=phL->text().toDouble()/pDL->text().toDouble();

    //S площадь полного сечения трубы, м2;
    valMap["S"]=PI*qPow((pDL->text().toDouble()/2),2.0);

    //proc процент заполнения трубы, %;
    valMap["%"]=valMap["w"]*100/valMap["S"];

    //v скорость потока, м/с;
    valMap["v"]=valMap["C"]*qSqrt(valMap["R"]*piL->text().toDouble());

    //Q расход, л/с;
    valMap["Q"]=valMap["w"]*valMap["C"]*qSqrt(valMap["R"]*piL->text().toDouble())*1000;

    //add Labels to pgrpOut

    if(phL->text().toDouble()>=pDL->text().toDouble())
    {
        for(int i=0; i<valStr.size(); ++i)
        {
            pLblsOut[i]->setText("");
        }
        pNoteLbl->setText("Уровень воды не может быть больше значения диаметра трубы.");
        drawCircle();
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
void PipeWgt::draw()
{
    pPix=new QPixmap(230,230);
    QPainter painter;
    painter.begin(pPix);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setPen(QPen(Qt::black,2,Qt::SolidLine));
    painter.translate(15.0,15.0);
    double scale=200/pDL->text().toDouble();
    painter.setClipRect(0,(scale*(pDL->text().toDouble()-(valMap["r"]+((phL->text().toDouble()<valMap["r"])?(-hTriang):hTriang)))),
            200,215-scale*(pDL->text().toDouble()-(valMap["r"]+((phL->text().toDouble()<valMap["r"])?(-hTriang):hTriang))));
    painter.save();
    painter.setPen(Qt::NoPen);
    QLinearGradient gradient((scale*(valMap["r"]-b/2+valMap["r"])),
            (scale*(pDL->text().toDouble()-(valMap["r"]+((phL->text().toDouble()<valMap["r"])?(-hTriang):hTriang)))),
            (scale*(valMap["r"]-b/2+valMap["r"])),
            (scale*(pDL->text().toDouble()-(valMap["r"]+((phL->text().toDouble()<valMap["r"])?(-hTriang):hTriang))+phL->text().toDouble())));
    gradient.setColorAt(0,Qt::blue);
    gradient.setColorAt(1,Qt::darkBlue);
    painter.setBrush(gradient);
    painter.drawEllipse(0,0,scale*pDL->text().toDouble(),scale*pDL->text().toDouble());
    painter.setClipping(false);
    painter.restore();
    painter.drawEllipse(0,0,scale*pDL->text().toDouble(),scale*pDL->text().toDouble());
    painter.drawLine(scale*(valMap["r"]-b/2),scale*(pDL->text().toDouble()-(valMap["r"]+((phL->text().toDouble()<valMap["r"])?(-hTriang):hTriang))),
            scale*(valMap["r"]+b/2),scale*(pDL->text().toDouble()-(valMap["r"]+((phL->text().toDouble()<valMap["r"])?(-hTriang):hTriang))));
    painter.end();
    pPixLbl->setPixmap(*pPix);

}

void PipeWgt::drawCircle()
{
    pPix=new QPixmap(230,230);
    QPainter painter;
    painter.begin(pPix);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setPen(QPen(Qt::black,2,Qt::SolidLine));
    painter.translate(15.0,15.0);
    double scale=200/pDL->text().toDouble();
    painter.drawEllipse(0,0,scale*pDL->text().toDouble(),scale*pDL->text().toDouble());
    painter.end();
    pPixLbl->setPixmap(*pPix);
}


//Slots__________________________________________________________________________
void PipeWgt::slotPhLChanged()
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
    if((phL->text()!="")&&(pDL->text()!="")&&(pnL->text()!="")&&(piL->text()!=""))
        calc();
}

void PipeWgt::slotPDLChanged()
{
    if(!(pDL->text()==""))
    {
        QString str=pDL->text();
        if(str[str.length()-1]==',')
        {
            str[str.length()-1]='.';
            str.append("00");
            pDL->setText(str);
        }
        if(str[1]==',')
        {
            str[1]='.';
            pDL->setText(str);
        }
        if(str[str.length()-1]=='.')
        {
            str.append("00");
            pDL->setText(str);
        }
        if(str[0]=='.')
        {
            str='0'+str;
            pDL->setText(str);
        }
        if(str[0]==',')
        {
            str[0]='.';
            str='0'+str;
            pDL->setText(str);
        }
    }
    if((phL->text()!="")&&(pDL->text()!="")&&(pnL->text()!="")&&(piL->text()!=""))
        calc();
}

void PipeWgt::slotPnLChanged()
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
    if((phL->text()!="")&&(pDL->text()!="")&&(pnL->text()!="")&&(piL->text()!=""))
        calc();
}

void PipeWgt::slotPiLChanged()
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
    if((phL->text()!="")&&(pDL->text()!="")&&(pnL->text()!="")&&(piL->text()!=""))
        calc();
}
