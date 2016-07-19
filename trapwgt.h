#ifndef TrapWgt_H
#define TrapWgt_H

#include <QtWidgets>
#include <QWidget>

class TrapWgt : public QWidget
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
    QLabel* pWdown;
    QLabel* pWup;
    QLabel* pn;
    QLabel* pi;

    QLineEdit* phL;
    QLineEdit* pHL;
    QLineEdit* pWupL;
    QLineEdit* pWdownL;
    QLineEdit* pnL;
    QLineEdit* piL;

    //Out_______________________
    QLabel* pLblsOut[11];


public:
    explicit TrapWgt(QWidget *parent = 0);
private:
    void calc();
    void draw();
    void drawRect();
signals:

public slots:
    void slotPhLChanged();
    void slotPHLChanged();
    void slotPWdownLChanged();
    void slotPWupLChanged();
    void slotPnLChanged();
    void slotPiLChanged();
};

#endif // TrapWgt_H
