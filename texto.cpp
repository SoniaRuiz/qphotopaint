#include "texto.h"
#include "ui_texto.h"
#include "imagenes.h"

#include <QColorDialog>

Scalar color = CV_RGB(0,0,0);

Texto::Texto(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Texto)
{
    ui->setupUi(this);
}

Texto::~Texto()
{
    delete ui;
}

void Texto::on_buttonBox_accepted()
{
    //Texto
    QString text = ui->lineEdit->text();
    //Tamaño del texto
    double tam = ui->doubleSpinBox->value();

    if(text != ""){
        actualizar_variables_texto(text.toStdString(), tam, color);
        //Actualizamos al color negro por defecto
        color = CV_RGB(0,0,0);
    }
    else{
        return;
    }
}

void Texto::on_pushButton_clicked()
{
    QColor colortexto = QColorDialog::getColor();
    if (colortexto.isValid()) {
        QString estilo= "background-color: rgb(";
        estilo+= QString::number(colortexto.red())+",";
        estilo+= QString::number(colortexto.green())+",";
        estilo+= QString::number(colortexto.blue())+")";
        ui->pushButton->setStyleSheet(estilo);
        color = CV_RGB(colortexto.red(), colortexto.green(), colortexto.blue());
    }
}
