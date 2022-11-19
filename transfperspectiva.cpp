#include "transfperspectiva.h"
#include "ui_transfperspectiva.h"
#include "imagenes.h"

transfperspectiva::transfperspectiva(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::transfperspectiva)
{
    ui->setupUi(this);
}

transfperspectiva::~transfperspectiva()
{
    delete ui;
}

void transfperspectiva::on_buttonBox_accepted()
{
    //Obtenemos las dimensiones introducidas por el usuario
    int x = ui->spinBoxX->value();
    int y = ui->spinBoxY->value();

    //Actualizamos la variable global 'dimperspectiva' con los
    //datos introducidos por el usuario
    actualizar_dimperspectiva(x,y);

    //Actualizamos el tipo de herramienta utilizada
    herr_actual = perspectiva;
    MessageBoxA(NULL, "Por favor, pulse en cuatro lugares "
                      "distintos de la imagen.", "Perspectiva", MB_OK);
}
