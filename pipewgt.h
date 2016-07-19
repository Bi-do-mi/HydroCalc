#ifndef PIPEWGT_H
#define PIPEWGT_H

#include <QtWidgets>
#include <QWidget>

class PipeWgt : public QWidget
{
    Q_OBJECT

private:
    qreal betta;
    qreal Rad;
    qreal Striang;
    qreal b;
    qreal hTriang;
    const qreal PI = 3.14159265;
    QLabel* pPixLbl;
    QPixmap* pPix;
    QLabel* pNoteLbl;

    QGroupBox* pgrpIn;
    QGroupBox* pgrpOut;
    QGroupBox* pgrpPic;

    QVBoxLayout* pvbInL;
    QFormLayout* pfrmLIn;
    QVBoxLayout* pvbOutL;
    QVBoxLayout* pvbPicL;
    QHBoxLayout* phbL;
    QVBoxLayout* pvbL;

    QMap<QString, qreal> valMap;
    QStringList valStr;
    QStringList toolTipStr;

    //In_________________________
    QLabel* ph;
    QLabel* pD;
    QLabel* pn;
    QLabel* pi;

    QLineEdit* phL;
    QLineEdit* pDL;
    QLineEdit* pnL;
    QLineEdit* piL;

    //Out_______________________
    QLabel* pLblsOut[12];


public:
    explicit PipeWgt(QWidget *parent = 0);
private:
    void calc();
    void draw();
    void drawCircle();
signals:

public slots:
    void slotPhLChanged();
    void slotPDLChanged();
    void slotPnLChanged();
    void slotPiLChanged();
};

#endif // PIPEWGT_H
