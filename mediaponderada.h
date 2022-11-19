#ifndef MEDIAPONDERADA_H
#define MEDIAPONDERADA_H

#include <QDialog>

#include "imagenes.h"

namespace Ui {
    class mediaponderada;
}

class mediaponderada : public QDialog {
    Q_OBJECT
public:
    mediaponderada(QWidget *parent = 0);
    ~mediaponderada();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::mediaponderada *ui;
    int nfoto, nfoto2;
    Mat imgUno, imgDos, imgRes;
    int corresp[MAX_VENTANAS];

private slots:
    void on_buttonBox_accepted();
    void on_listWidget_currentRowChanged(int currentRow);
    void on_horizontalSlider_valueChanged(int value);
};

#endif // MEDIAPONDERADA_H
