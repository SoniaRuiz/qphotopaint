#include "histograma2d.h"
#include "ui_histograma2d.h"
#include "imagenes.h"

histograma2d::histograma2d(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::histograma2d)
{
    ui->setupUi(this);
}

histograma2d::~histograma2d()
{
    delete ui;
}

void histograma2d::on_buttonBox_accepted()
{
    //Obtenemos los canales de color seleccionados
    int canales[2] = {} ;
    if(ui->radioButton->isChecked()){
        canales[0] = 2;
        canales[1] = 1;
    }
    else if(ui->radioButton_2->isChecked()){
        canales[0] = 2;
        canales[1] = 0;
    }
    else if(ui->radioButton_3->isChecked()){
        canales[0] = 0;
        canales[1] = 1;
    }
    else{
        return;
    }

    //Seleccionamos las dimensiones
    int x = ui->spinBox->value();
    int y = ui->spinBox_2->value();

    //Enviamos los datos introducidos por el usuario
    //al método 'mostrar_histograma2D'
    mostrar_histograma2D(foto_activa(), canales, x, y);
}
