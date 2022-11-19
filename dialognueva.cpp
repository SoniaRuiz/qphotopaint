#include "dialognueva.h"
#include "ui_dialognueva.h"

#include <QColorDialog>

DialogNueva::DialogNueva(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNueva)
{
    ui->setupUi(this);
}

DialogNueva::~DialogNueva()
{
    delete ui;
}

void DialogNueva::changeEvent(QEvent *e)
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

void DialogNueva::on_pushButton_clicked()
{
    QColor color2= QColorDialog::getColor();
    if (color2.isValid()) {
        color= color2;
        QString estilo= "background-color: rgb(";
        estilo+= QString::number(color.red())+",";
        estilo+= QString::number(color.green())+",";
        estilo+= QString::number(color.blue())+")";
        ui->pushButton->setStyleSheet(estilo);
    }
}

int DialogNueva::red()
{
    return color.red();
}

int DialogNueva::green()
{
    return color.green();
}

int DialogNueva::blue()
{
    return color.blue();
}

int DialogNueva::getWidth()
{
    return ui->spinBox->value();
}

int DialogNueva::getHeight()
{
    return ui->spinBox_2->value();
}
