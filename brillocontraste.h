#ifndef BRILLOCONTRASTE_H
#define BRILLOCONTRASTE_H

#include <QDialog>

namespace Ui {
    class brillocontraste;
}

class brillocontraste : public QDialog {
    Q_OBJECT
public:
    brillocontraste(QWidget *parent = 0);
    ~brillocontraste();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::brillocontraste *ui;
    int nfoto;
    double suma;
    double multiplica;
    double gama;
    void actualiza_suma_multiplica();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_brillocontraste_accepted();
    void on_horizontalSlider_2_valueChanged(int value);
    void on_horizontalSlider_valueChanged(int value);
    void on_spinBox_2_valueChanged(int );
    void on_spinBox_valueChanged(int );
    void on_horizontalSlider_3_valueChanged(int value);
    void on_spinBox_3_valueChanged(int arg1);
};

#endif // BRILLOCONTRASTE_H
