#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QColorDialog>
#include <QMessageBox>
#include <windows.h>
#include <opencv2/opencv.hpp>
using namespace cv;

#include "acercade.h"
#include "brillocontraste.h"
#include "dialognueva.h"
#include "imagenes.h"
#include "mediadevideo.h"
#include "mediaponderada.h"
#include "rotaravideo.h"
#include "suavizados.h"
#include "histograma2d.h"
#include "transfperspectiva.h"
#include "rotaryreescalar.h"
#include "texto.h"

QString FiltroImagen = "Todos los formatos (*.bmp *.dib *.jpg *.jpeg *.jpe *.tif *.tiff *.png *.gif);;Archivos BMP (*.bmp *.dib);;Archivos JPG (*.jpg *.jpeg *.jpe);;Archivos TIF (*.tif *.tiff);;Archivos GIF (*.gif);;Otros (*.*)";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::show()
{
    QMainWindow::show();
    move(x(), 0);
}

void MainWindow::setStatusBarText(QString cadena)
{
    ui->statusBar->showMessage(cadena, 0);
}

int MainWindow::comprobar_primera_libre (void)
{
    int pl= primera_libre();
    if (pl==-1)
        QMessageBox::warning(this, "Error al crear imagen",
                             "Lo siento. No se pueden crear más ventanas.");
    return pl;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_actionSalir_triggered()
{
    close();
}

void MainWindow::on_actionNueva_imagen_triggered()
{
    int pl= comprobar_primera_libre();
    if (pl!=-1) {
        DialogNueva nueva;
        if (nueva.exec()) {
            Scalar color= CV_RGB(nueva.red(), nueva.green(), nueva.blue());
            crear_nueva(pl, nueva.getWidth(), nueva.getHeight(), color);
        }
    }
}

void MainWindow::on_actionAbrir_imagen_triggered()
{
    int pl= comprobar_primera_libre();
    if (pl!=-1) {
        QString nombre= QFileDialog::getOpenFileName(this, "Abrir imagen", ".", FiltroImagen);
        if (!nombre.isEmpty())
            crear_nueva(pl, nombre.toLatin1().data());
    }
}

void MainWindow::on_actionPunto_triggered()
{
    herr_actual= puntos;
    ui->toolButton_4->setChecked(true);
}

void MainWindow::on_actionLinea_triggered()
{
    herr_actual= lineas;
    ui->toolButton_5->setChecked(true);
}

void MainWindow::on_actionSeleccionar_triggered()
{
    herr_actual= seleccionar;
    ui->toolButton_6->setChecked(true);
}

void MainWindow::on_toolButton_2_clicked()
{
    on_actionNueva_imagen_triggered();
}

void MainWindow::on_pushButton_clicked()
{
    QColor color= QColorDialog::getColor();
    if (color.isValid()) {
        QString estilo= "background-color: rgb(";
        estilo+= QString::number(color.red())+",";
        estilo+= QString::number(color.green())+",";
        estilo+= QString::number(color.blue())+")";
        ui->pushButton->setStyleSheet(estilo);
        color_pincel= CV_RGB(color.red(), color.green(), color.blue());
    }
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    radio_pincel= value;
}

void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    difum_pincel= value;
}

void MainWindow::on_toolButton_clicked()
{
    on_actionAbrir_imagen_triggered();
}

void MainWindow::on_actionGuardar_triggered()
{
    int fa= foto_activa();
    if (fa!=-1)
        guardar_foto(fa);
}

void MainWindow::on_actionGuardar_como_triggered()
{
    int fa= foto_activa();
    if (fa!=-1) {
        QString nombre= QFileDialog::getSaveFileName(this, "Guardar imagen", QString::fromLatin1(foto[fa].nombre.c_str()), FiltroImagen);
        if (!nombre.isEmpty())
            guardar_foto(fa, nombre.toLatin1().data());
    }
}

void MainWindow::on_actionCerrar_triggered()
{
    int fa= foto_activa();
    if (fa!=-1) {
        if (foto[fa].modificada && preguntar_guardar) {
            QString nombre= QString::fromLatin1(foto[fa].nombre.c_str());
            nombre= "El archivo " + nombre + " ha sido modificado.\n¿Desea guardarlo?";
            int resp= QMessageBox::question(this, "Archivo modificado", nombre,
                                            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            if (resp==QMessageBox::Yes)
                guardar_foto(fa);
            else if (resp==QMessageBox::Cancel)
                return;
            else
                foto[fa].modificada= false;
        }
        cerrar_foto(fa);
    }
}

void MainWindow::on_actionPreguntar_si_guardar_triggered()
{
    preguntar_guardar= ui->actionPreguntar_si_guardar->isChecked();
}

void MainWindow::on_actionColor_del_pincel_triggered()
{
    on_pushButton_clicked();
}

void MainWindow::on_actionSeleccionar_todo_triggered()
{
    int fa= foto_activa();
    if (fa!=-1) {
        foto[fa].roi= Rect(-1, -1, -1, -1);
        mostrar(fa);
    }
}

void MainWindow::on_toolButton_4_clicked()
{
    herr_actual= puntos;
}

void MainWindow::on_toolButton_5_clicked()
{
    herr_actual= lineas;
}

void MainWindow::on_toolButton_6_clicked()
{
    herr_actual= seleccionar;
}

void MainWindow::on_actionInvertir_triggered()
{
    int fa= foto_activa();
    if (fa!=-1) {
        int pl= comprobar_primera_libre();
        if (pl!=-1)
            invertir(fa, pl);
    }
}

void MainWindow::on_actionRotar_90_triggered()
{
    int fa= foto_activa();
    if (fa!=-1) {
        int pl= comprobar_primera_libre();
        if (pl!=-1)
            rotar_exacto(fa, pl, 1);
    }
}

void MainWindow::on_actionRotar_180_triggered()
{
    int fa= foto_activa();
    if (fa!=-1) {
        int pl= comprobar_primera_libre();
        if (pl!=-1)
            rotar_exacto(fa, pl, 1);
    }
}

void MainWindow::on_actionRotar_91_triggered()
{
    int fa= foto_activa();
    if (fa!=-1) {
        int pl= comprobar_primera_libre();
        if (pl!=-1)
            rotar_exacto(fa, pl, 1);
    }
}

void MainWindow::on_actionBrillo_contraste_triggered()
{
    if (foto_activa()!=-1) {
        brillocontraste bc;
        bc.exec();
    }
}

void MainWindow::on_actionGausiano_triggered()
{
    if (foto_activa()!=-1) {
        suavizados sg(CV_GAUSSIAN);
        sg.exec();
    }
}

void MainWindow::on_actionMedia_triggered()
{
    if (foto_activa()!=-1) {
        suavizados sg(CV_BLUR);
        sg.exec();
    }
}

void MainWindow::on_actionMedia_ponderada_triggered()
{
    if (foto_activa()!=-1) {
        int pl= comprobar_primera_libre();
        if (pl!=-1) {
            mediaponderada mp;
            mp.exec();
        }
    }
}

void MainWindow::on_actionRotar_imagen_triggered()
{
    if (foto_activa()!=-1) {
        rotaravideo rv;
        rv.exec();
    }
}

void MainWindow::on_actionAcerca_de_triggered()
{
    acercade ad;
    ad.exec();
}

void MainWindow::on_actionImagen_media_triggered()
{
    mediadevideo mv;
    mv.exec();
}



void MainWindow::on_actionRect_ngulo_triggered()
{
    herr_actual = rectangulos;

    ui->toolButton_7->setChecked(true);
}

void MainWindow::on_toolButton_7_clicked()
{
    herr_actual = rectangulos;
}

void MainWindow::on_toolButton_8_clicked()
{
    herr_actual = elipses;
}

void MainWindow::on_actionElipses_triggered()
{
    herr_actual = elipses;
    ui->toolButton_8->setChecked(true);
}

void MainWindow::on_actionTri_ngulo_triggered()
{
    herr_actual = poligonos;
    ui->toolButton_9->setChecked(true);
}

void MainWindow::on_toolButton_9_clicked()
{
    herr_actual = poligonos;
}

void MainWindow::on_actionMediana_triggered()
{
    if(foto_activa() != -1){
        suavizados sg(CV_MEDIAN);
        sg.exec();
    }
    //TODO: si está seleccionada el suavizado de mediana, ocultamos el selector de la y
    //TODO: establecer un máximo en el número de puntos de la X
}

void MainWindow::on_actionAleatorio_triggered()
{
    //TODO: definir constante aleatroia al igual que con CV_MEDIAn y sustituir el else por else if (CV_RANDOM)
    //TODO: corregir el máximo de los puntos por el número de píxeles de la imagen en X e Y
    if(foto_activa() != -1){
        suavizados sg(5);
        sg.exec();
    }
}

void MainWindow::on_actionAcristalado_triggered()
{
    //TODO: definir constante CV_Acristalado

    if(foto_activa() != -1){
        suavizados sg(6);
        sg.exec();
    }
}

void MainWindow::on_actionXYZ_triggered()
{
    if(foto_activa() != -1){
        espacio_color(foto_activa(), COLOR_BGR2XYZ);
    }
}

void MainWindow::on_actionHLS_triggered()
{
    if(foto_activa() != -1){
        espacio_color(foto_activa(), COLOR_BGR2HLS);
    }
}

void MainWindow::on_actionYCrCb_triggered()
{
    if(foto_activa() != -1){

        espacio_color(foto_activa(), COLOR_BGR2YCrCb);
    }
}

void MainWindow::on_actionLab_triggered()
{
    if(foto_activa() != -1){
        espacio_color(foto_activa(), COLOR_BGR2Lab);
    }
}

void MainWindow::on_actionRGB_triggered()
{
    if(foto_activa() != -1){
        espacio_color(foto_activa(), COLOR_BGR2RGB);
    }
}

void MainWindow::on_actionBalance_de_blancos_triggered()
{
    if(foto_activa() != -1){
        balance_blancos(foto_activa());
    }
}

void MainWindow::on_actionR_triggered()
{
    if(foto_activa() != -1){
        mostrar_histograma(foto_activa(), 2);
    }
}

void MainWindow::on_actionG_triggered()
{
    if(foto_activa() != -1){
        mostrar_histograma(foto_activa(), 1);
    }
}

void MainWindow::on_actionB_triggered()
{
    if(foto_activa() != -1){
        mostrar_histograma(foto_activa(), 0);
    }
}

void MainWindow::on_actionEcualizado_triggered()
{
    if(foto_activa() != -1){
        ecualiza_histograma(foto_activa());
    }
}

void MainWindow::on_actionCapturar_de_c_mara_triggered()
{
    //Llamamos directamente a la función 'capturar_desde_camara()' porque
    //para capturar una imagen desde la cámara conectada al PC
    //no es necesario que exista una foto activa
    capturar_desde_camara();
}

void MainWindow::on_actionCapturar_de_v_deo_triggered()
{
    //Llamamos directamente a la función 'capturar_desde_video()' porque
    //para capturar una imagen desde un vídeo, no es necesario
    //que exista una foto activa
    capturar_desde_video();
}

void MainWindow::on_actionCopiar_a_nueva_triggered()
{
    //Si existe una imagen en pantalla desde la cuál se desea hacer una copia
    if (foto_activa()!=-1) {
        //Obtenemos la siguiente posición del array en la que podemos crear una imagen
        int pl = comprobar_primera_libre();
        if (pl != -1) {
            //Enviamos la foto activa y la posición libre encontrada
            //al método 'copiar_a_nueva'
            copiar_a_nueva(foto_activa(), pl);
        }
    }
}

void MainWindow::on_actionVer_histograma_2D_triggered()
{
    if(foto_activa() != -1){
        histograma2d h2d;
        h2d.exec();
    }
}

void MainWindow::on_actionArco_irirs_triggered()
{
    //Indicamos que la herramienta seleccionada es 'arcoiris'
    herr_actual = arcoiris;
    //Seleccionamos el botón con el icono del arco iris
    ui->toolButton_10->setChecked(true);
}

void MainWindow::on_toolButton_10_clicked()
{
    //Indicamos que la herramienta seleccionada es 'arcoiris'
    herr_actual = arcoiris;
}

void MainWindow::on_actionPerspectiva_triggered()
{
    //Si existe una imagen activa
    if(foto_activa() != -1){
        //Abrimos el formulario de introducción de dimensiones para
        //la imagen perspectiva
        transfperspectiva transf;
        transf.exec();
    }
}

void MainWindow::on_actionRotar_imagen_y_reescalar_triggered()
{
    //Si existe una imagen activa
    if(foto_activa() != -1){
        //Abrimos el formulario de introducción de ángulo y
        //tamaño de la imagen reescalada
        rotaryreescalar rotar;
        rotar.exec();
    }
}


void MainWindow::on_actionConvertir_a_escala_de_grises_triggered()
{
    //Si existe una imagen activa
    if(foto_activa() != -1){
        escala_grises();
    }
}

void MainWindow::on_actionColor_falso_triggered()
{
    //Si existe una imagen activa
    if(foto_activa() != -1){
        color_falso();
    }
}

void MainWindow::on_actionTexto_triggered()
{
    //Si existe una imagen activa sobre la que añadir texto
    if(foto_activa() != -1){
        //Actualizamos el tipo de herramienta utilizada
        herr_actual = texto;
        MessageBoxA(NULL, "Por favor, pulse en la posicion de "
                          "la imagen sobre la que desea dibujar texto.",
                          "Dibujar texto", MB_OK);
    }
}
