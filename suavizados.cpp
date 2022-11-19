#include "suavizados.h"
#include "ui_suavizados.h"

#include "imagenes.h"

suavizados::suavizados(int tipo, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::suavizados)
{
    ui->setupUi(this);
    nfoto= foto_activa();
    num_tipo= tipo;
}

suavizados::~suavizados()
{
    delete ui;
}

void suavizados::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void suavizados::on_spinBox_valueChanged(int valor)
{
    ui->horizontalSlider->setValue(valor);
}

void suavizados::on_spinBox_2_valueChanged(int valor)
{
    ui->horizontalSlider_2->setValue(valor);
}

void suavizados::on_horizontalSlider_valueChanged(int value)
{
    ui->spinBox->setValue(value);
    if (ui->checkBox->isChecked())
        ver_suavizado(nfoto, num_tipo,
                      ui->horizontalSlider->value()*2-1,
                      ui->horizontalSlider_2->value()*2-1);
}

void suavizados::on_horizontalSlider_2_valueChanged(int value)
{
    ui->spinBox_2->setValue(value);
    if (ui->checkBox->isChecked())
        ver_suavizado(nfoto, num_tipo,
                      ui->horizontalSlider->value()*2-1,
                      ui->horizontalSlider_2->value()*2-1);
}

void suavizados::on_buttonBox_accepted()
{
    ver_suavizado(nfoto, num_tipo,
                  ui->horizontalSlider->value()*2-1,
                  ui->horizontalSlider_2->value()*2-1, true);
}

void suavizados::on_buttonBox_rejected()
{
    mostrar(nfoto);
}

