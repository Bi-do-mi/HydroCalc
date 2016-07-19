#ifndef RECTPIPEWGT_H
#define RECTPIPEWGT_H

#include <QtWidgets>
#include <QWidget>

class RectPipeWgt : public QWidget
{
    Q_OBJECT

private:

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
    QLabel* pH;
    QLabel* pW;
    QLabel* pn;
    QLabel* pi;

    QLineEdit* phL;
    QLineEdit* pHL;
    QLineEdit* pWL;
    QLineEdit* pnL;
    QLineEdit* piL;

    //Out_______________________
    QLabel* pLblsOut[11];


public:
    explicit RectPipeWgt(QWidget *parent = 0);
private:
    void calc();
    void draw();
    void drawRect();
signals:

public slots:
    void slotPhLChanged();
    void slotPHLChanged();
    void slotPWLChanged();
    void slotPnLChanged();
    void slotPiLChanged();
};

#endif // RECTPIPEWGT_H
