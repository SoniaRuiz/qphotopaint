#ifndef SUAVIZADOS_H
#define SUAVIZADOS_H

#include <QDialog>

namespace Ui {
    class suavizados;
}

class suavizados : public QDialog {
    Q_OBJECT
public:
    suavizados(int tipo, QWidget *parent = 0);
    ~suavizados();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::suavizados *ui;
    int nfoto;
    int num_tipo;            // Tipo de suavizado que se aplica: CV_GAUSSIAN, CV_BLUR

private slots:
    void on_buttonBox_rejected();
    void on_buttonBox_accepted();
    void on_horizontalSlider_2_valueChanged(int value);
    void on_horizontalSlider_valueChanged(int value);
    void on_spinBox_2_valueChanged(int );
    void on_spinBox_valueChanged(int );
};

#endif // SUAVIZADOS_H
