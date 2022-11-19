#include "rotaryreescalar.h"
#include "ui_rotaryreescalar.h"
#include "imagenes.h"

rotaryreescalar::rotaryreescalar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::rotaryreescalar)
{
    ui->setupUi(this);
}

rotaryreescalar::~rotaryreescalar()
{
    delete ui;
}

void rotaryreescalar::on_buttonBox_accepted()
{
    //Obtenemos las dimensiones de la nueva imagen
    int x = ui->spinBox->value();
    int y = ui->spinBox_2->value();

    //Obtenemos el ángulo de la nueva imagen
    int angulo = ui->spinBox_3->value();

    //Enviamos los datos al método que aplica las transformaciones
    rotar_reescalar(x, y, angulo);
}
