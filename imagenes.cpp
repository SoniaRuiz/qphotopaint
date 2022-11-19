//---------------------------------------------------------------------------

#include "imagenes.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <winuser.h>
#include "texto.h"



///////////////////////////////////////////////////////////////////
/////////  VARIABLES GLOBALES                        //////////////
///////////////////////////////////////////////////////////////////

ventana foto[MAX_VENTANAS];

tipo_herramienta herr_actual= puntos;

int radio_pincel= 10;

Scalar color_pincel= CV_RGB(255, 255, 255);

int difum_pincel= 10;

bool preguntar_guardar= true;

Mat histbase = imread("histbase.png");

#define MAX_PUNTOS 2000
//array de puntos
Point *apuntos = new Point[MAX_PUNTOS];
//Numero de puntos actual:
int npuntos = 0;

//Variables globales necesarias para dibujar el arcoiris
int rojo = 255, verde = 0, azul = 0;

//Variables globales necesarias para aplicar la perspectiva
Point2f perspectivaorigen[4];
Point2f perspectivadestino[4];
int dimperspectiva[2]; // Dimensiones de la imagen perspectiva

//Variables globales necesarias para dibujar texto
string textoadibujar;
double tamtexto;
Scalar colortexto;



///////////////////////////////////////////////////////////////////
/////////  FUNCIONES DE MANEJO DE VENTANAS           //////////////
///////////////////////////////////////////////////////////////////

void callback (int event, int x, int y, int flags, void *_nfoto);

void SetStatusBar1 (int masfotos);

//---------------------------------------------------------------------------

void inic_fotos (void)
{
    for (int i= 0; i<MAX_VENTANAS; i++) {
        foto[i].usada= false;
        foto[i].modificada= false;
        foto[i].roi= Rect(-1,-1,-1,-1);
    }
}

//---------------------------------------------------------------------------

void fin_fotos (void)
{
    for (int i= 0; i<MAX_VENTANAS; i++) {
        if (foto[i].usada) {
            destroyWindow(foto[i].nombre);
            foto[i].img.release();
            foto[i].usada= false;
        }
    }
}

//---------------------------------------------------------------------------

int primera_libre (void)
{
    int libre= -1;
    for (int i= 0; i<MAX_VENTANAS && libre==-1; i++)
        if (!foto[i].usada)
            libre= i;
    return libre;
}

//---------------------------------------------------------------------------

void escribir_barra_estado (void)
{
    int usadas, modificadas;
    num_fotos(usadas, modificadas);
    w->setStatusBarText(QString::number(usadas)+" fotos abiertas, "+
                        QString::number(modificadas)+" modificadas.");
}

//---------------------------------------------------------------------------

void crear_nueva (int nfoto, int ancho, int alto, Scalar color)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && !foto[nfoto].usada);
    char nombre[100];
    sprintf(nombre, "nueva-%d.bmp", nfoto);
    foto[nfoto].nombre= string(nombre);
    foto[nfoto].img.create(alto, ancho, CV_8UC3);
    foto[nfoto].img=  color;
    namedWindow(foto[nfoto].nombre, WINDOW_NO_POPUP+WINDOW_MOVE_RIGHT);
    foto[nfoto].handle= GetActiveWindow();
    imshow(foto[nfoto].nombre, foto[nfoto].img);
    foto[nfoto].usada= true;
    foto[nfoto].modificada= true;
    foto[nfoto].roi= Rect(-1, -1, -1, -1);
    setMouseCallback(foto[nfoto].nombre, callback, (void*) nfoto);
    escribir_barra_estado();
}

//---------------------------------------------------------------------------

void crear_nueva (int nfoto, Mat img)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && !foto[nfoto].usada && !img.empty());
    char nombre[100];
    sprintf(nombre, "nueva-%d.bmp", nfoto);
    foto[nfoto].nombre= string(nombre);
    foto[nfoto].img= img;
    namedWindow(foto[nfoto].nombre, WINDOW_NO_POPUP+WINDOW_MOVE_RIGHT);
    foto[nfoto].handle= GetActiveWindow();
    imshow(foto[nfoto].nombre, foto[nfoto].img);
    foto[nfoto].usada= true;
    foto[nfoto].modificada= true;
    foto[nfoto].roi= Rect(-1, -1, -1, -1);
    setMouseCallback(foto[nfoto].nombre, callback, (void*) nfoto);
    escribir_barra_estado();
}

//---------------------------------------------------------------------------

void crear_nueva (int nfoto, string nombre)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && !foto[nfoto].usada);
    foto[nfoto].img= imread(nombre);
    if (foto[nfoto].img.empty())
        return;
    foto[nfoto].nombre= nombre;
    namedWindow(foto[nfoto].nombre, WINDOW_NO_POPUP+WINDOW_MOVE_RIGHT);
    foto[nfoto].handle= GetActiveWindow();
    imshow(foto[nfoto].nombre, foto[nfoto].img);
    foto[nfoto].usada= true;
    foto[nfoto].modificada= false;
    foto[nfoto].roi= Rect(-1, -1, -1, -1);
    setMouseCallback(foto[nfoto].nombre, callback, (void*) nfoto);
    escribir_barra_estado();
}

//---------------------------------------------------------------------------

void guardar_foto (int nfoto, string nombre)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && foto[nfoto].usada);
    foto[nfoto].modificada= false;
    // 1. Guardar la imagen con el mismo nombre que tiene
    if (nombre=="")
        imwrite(foto[nfoto].nombre, foto[nfoto].img);
    // 2. Guardar la imagen con un nombre distinto al actual
    else {
        imwrite(nombre, foto[nfoto].img);
        destroyWindow(foto[nfoto].nombre);   // Se debe cambiar el nombre de la
        foto[nfoto].nombre= nombre;
        namedWindow(foto[nfoto].nombre, WINDOW_NO_POPUP+WINDOW_MOVE_RIGHT);
        foto[nfoto].handle= GetActiveWindow();
        imshow(foto[nfoto].nombre, foto[nfoto].img);
        setMouseCallback(foto[nfoto].nombre, callback, (void*) nfoto);
    }
    escribir_barra_estado();
}

//---------------------------------------------------------------------------

void cerrar_foto (int nfoto, bool destruir_ventana)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && foto[nfoto].usada);
    if (destruir_ventana)
        destroyWindow(foto[nfoto].nombre);
    foto[nfoto].img.release();
    foto[nfoto].usada= false;
    escribir_barra_estado();
}

//---------------------------------------------------------------------------

void mostrar (int nfoto)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && foto[nfoto].usada);
    imshow(foto[nfoto].nombre, foto[nfoto].img);
}

//---------------------------------------------------------------------------

int nombre_a_numero (string nombre)
{
    int i;
    for (i= 0; i<MAX_VENTANAS; i++)
        if (foto[i].usada && foto[i].nombre==nombre)
            return i;
    return -1;
}

//---------------------------------------------------------------------------

int foto_activa (void)
{
    int j= 0;
    HWND hijo= GetActiveWindow();
    do {
        for (int i= 0; i<MAX_VENTANAS; i++)
            if (foto[i].usada && hijo==foto[i].handle) {
                return i;
            }
        hijo= GetNextWindow(hijo, GW_HWNDNEXT);
        j++;
    } while (j<100);
    return -1;
}

//---------------------------------------------------------------------------

int num_fotos (int &usadas, int &modificadas)
{
    usadas= 0;
    modificadas= 0;
    for (int i= 0; i<MAX_VENTANAS; i++) {
        if (foto[i].usada) {
            usadas++;
            if (foto[i].modificada)
                modificadas++;
        }
    }
    return usadas;
}

///////////////////////////////////////////////////////////////////
/////////  FUNCIONES DEL CALLBACK PRINCIPAL          //////////////
///////////////////////////////////////////////////////////////////

int downx, downy;
// Posición inicial del ratón al pinchar sobre la imagen actual

//---------------------------------------------------------------------------
void ninguna_accion (int factual, int x, int y)
{
    Mat res= foto[factual].img.clone();
    circle(res, Point(x, y), radio_pincel, CV_RGB(255,255,255), 1, CV_AA);
    imshow(foto[factual].nombre, res);
}
//---------------------------------------------------------------------------
void cb_close (int factual)
{
    if (!foto[factual].img.empty() && foto[factual].usada) {
        if (foto[factual].modificada && preguntar_guardar) {
            QString cadena= "El archivo " + QString::fromLatin1(foto[factual].nombre.c_str()) +
                    " ha sido modificado.\n¿Desea guardarlo?";
            int resp= QMessageBox::question(w, "Archivo modificado", cadena,
                                            QMessageBox::Yes | QMessageBox::No );
            if (resp==QMessageBox::Yes)
                guardar_foto(factual);
        }
        cerrar_foto(factual, false);
    }
}
//---------------------------------------------------------------------------

void cb_punto (int factual, int x, int y, Scalar color = CV_RGB(255,255,255))
{
    Scalar colorpunto = (color == (Scalar)CV_RGB(255,255,255)) ? color_pincel : color;
    Mat im= foto[factual].img;  // Ojo: esto no es una copia, sino a la misma
    // Redibujar la ventana
    RedrawWindow(foto[factual].handle,NULL,NULL,RDW_UPDATENOW);
    if (difum_pincel==0){
        circle(im, Point(x, y), radio_pincel, colorpunto, -1, CV_AA);
    }
    else {
        //Operaciones necesarias para pintar un círculo suavizado
        //Imagen igual que la original pero con el color del pincel
        Mat res(im.size(), im.type(), colorpunto);
        //Otra igual que la anterior pero con fondo negro
        Mat cop(im.size(), im.type(), CV_RGB(0,0,0));
        //En la imagen negra pintamos un círculo
        circle(cop, Point(x, y), radio_pincel, colorpunto, -1, CV_AA);
        //Suavizamos el círculo
        blur(cop, cop, Size(difum_pincel*2+1, difum_pincel*2+1));
        //El resto de operaciones son multiplicaciones e inversas para
        //poner el círculo suavizado sobre la imagen.
        multiply(res, cop, res, 1.0/255.0);
        bitwise_not(cop, cop);
        multiply(im, cop, im, 1.0/255.0);
        im= res + im;
    }
    imshow(foto[factual].nombre, im);
    foto[factual].modificada= true;
}

void cb_linea (int factual, int x, int y)
{
    Mat im= foto[factual].img;  // Ojo: esto no es una copia, sino a la misma
    // Redibujar la ventana
    RedrawWindow(foto[factual].handle,NULL,NULL,RDW_UPDATENOW);
    if (difum_pincel==0)
        line(im, Point(downx, downy), Point(x,y), color_pincel, radio_pincel);
    else {
        Mat res(im.size(), im.type(), color_pincel);
        Mat cop(im.size(), im.type(), CV_RGB(0,0,0));
        line(cop, Point(downx, downy), Point(x,y), CV_RGB(255,255,255), radio_pincel);
        blur(cop, cop, Size(difum_pincel*2+1, difum_pincel*2+1));
        multiply(res, cop, res, 1.0/255.0);
        bitwise_not(cop, cop);
        multiply(im, cop, im, 1.0/255.0);
        im= res + im;
    }
    imshow(foto[factual].nombre, im);
    foto[factual].modificada= true;
}

void cb_rectangulo (int factual, int x, int y)
{
    Mat im= foto[factual].img;  // Ojo: esto no es una copia, sino a la misma
    // Redibujar la ventana
    RedrawWindow(foto[factual].handle,NULL,NULL,RDW_UPDATENOW);
    if (difum_pincel==0)
        rectangle(im, Point(downx, downy), Point(x,y), color_pincel, radio_pincel);
    else {
        Mat res(im.size(), im.type(), color_pincel);
        Mat cop(im.size(), im.type(), CV_RGB(0,0,0));
        rectangle(cop, Point(downx, downy), Point(x,y), CV_RGB(255,255,255), radio_pincel);
        blur(cop, cop, Size(difum_pincel*2+1, difum_pincel*2+1));
        multiply(res, cop, res, 1.0/255.0);
        bitwise_not(cop, cop);
        multiply(im, cop, im, 1.0/255.0);
        im= res + im;
    }
    imshow(foto[factual].nombre, im);
    foto[factual].modificada= true;
}

void cb_elipse (int factual, int x, int y)
{
    Mat im= foto[factual].img;  // Ojo: esto no es una copia, sino a la misma
    // Redibujar la ventana
    RedrawWindow(foto[factual].handle,NULL,NULL,RDW_UPDATENOW);
    if (difum_pincel==0)
        ellipse(im, Point(downx, downy), Size(abs(x-downx),abs(y-downy)),0,0,360, color_pincel, radio_pincel-1);
    else {
        Mat res(im.size(), im.type(), color_pincel);
        Mat cop(im.size(), im.type(), CV_RGB(0,0,0));
        ellipse(cop, Point(downx, downy), Size(abs(x-downx),abs(y-downy)),0,0,360, CV_RGB(255,255,255), radio_pincel-1);
        blur(cop, cop, Size(difum_pincel*2+1, difum_pincel*2+1));
        multiply(res, cop, res, 1.0/255.0);
        bitwise_not(cop, cop);
        multiply(im, cop, im, 1.0/255.0);
        im= res + im;
    }
    imshow(foto[factual].nombre, im);
    foto[factual].modificada= true;
}

void cb_poligono (int factual)
{
    Mat im= foto[factual].img;  // Ojo: esto no es una copia, sino a la misma
    // Redibujar la ventana
    RedrawWindow(foto[factual].handle,NULL,NULL,RDW_UPDATENOW);
    if (difum_pincel==0)
        fillConvexPoly(im, apuntos, npuntos, color_pincel);
    else {
        Mat res(im.size(), im.type(), color_pincel);
        Mat cop(im.size(), im.type(), CV_RGB(0,0,0));
        fillConvexPoly(cop, apuntos, npuntos, CV_RGB(255,255,255));
        blur(cop, cop, Size(difum_pincel*2+1, difum_pincel*2+1));
        multiply(res, cop, res, 1.0/255.0);
        bitwise_not(cop, cop);
        multiply(im, cop, im, 1.0/255.0);
        im= res + im;
    }
    imshow(foto[factual].nombre, im);
    foto[factual].modificada= true;
}

void cb_arcoiris (int factual, int x, int y)
{
    if(rojo == 255 && verde < 255 && azul == 0) //Estoy en la primera fase de colores
    {
        verde = verde + 17; //Sumo de 17 en 17 para que haya quince tonos de cada fase
    }
    else if (rojo > 0 && verde == 255 && azul == 0)
    {
        rojo = rojo - 17;
    }
    else if (rojo == 0 && verde == 255 && azul < 255)
    {
        azul = azul + 17;
    }
    else if (rojo == 0 && verde > 0 && azul == 255)
    {
        verde = verde - 17;
    }
    else if (rojo < 255 && verde == 0 && azul == 255)
    {
        rojo = rojo + 17;
    }
    else if (rojo == 255 && verde == 0 && azul > 0)
    {
        azul = azul - 17;
    }
    else{
        //Cuando finalizo, reinicimaos y comenzamos con el rojo de nuevo.
        rojo = 255;
        verde = 0;
        azul = 0;
    }
    //Llamamos al método que dibuja el punto pasándole el color de éste
    cb_punto (factual, x, y, CV_RGB(rojo,verde,azul));
}

//---------------------------------------------------------------------------

void cb_ver_linea (int factual, int x, int y)
{
    Mat res= foto[factual].img.clone();
    line(res, Point(downx, downy), Point(x,y), color_pincel, radio_pincel);
    imshow(foto[factual].nombre, res);
}

//---------------------------------------------------------------------------

void cb_ver_rectangulo (int factual, int x, int y)
{
    Mat res= foto[factual].img.clone();
    rectangle(res, Point(downx, downy), Point(x,y), color_pincel, radio_pincel -1);
    imshow(foto[factual].nombre, res);
}

//---------------------------------------------------------------------------

void cb_ver_elipse (int factual, int x, int y)
{
    Mat res= foto[factual].img.clone();
    ellipse(res, Point(downx, downy), Size(abs(x-downx),abs(y-downy)),0,0,360, color_pincel, radio_pincel-1);
    imshow(foto[factual].nombre, res);
}

//---------------------------------------------------------------------------


void cb_ver_poligono (int factual)
{

    Mat res= foto[factual].img.clone();
    //hacer fillPoly.
    fillConvexPoly(res, apuntos, npuntos, color_pincel);
    imshow(foto[factual].nombre, res);
}

//---------------------------------------------------------------------------

void cb_seleccionar (int factual, int x, int y)
{
    Mat im= foto[factual].img;
    Rect nuevo= Rect(min(downx, x), min(downy, y),
                     max(downx, x)-min(downx, x)+1, max(downy, y)-min(downy, y)+1);
    if (nuevo.x<0)
        nuevo.x= 0;
    if (nuevo.y<0)
        nuevo.y= 0;
    if (nuevo.x+nuevo.width>im.size().width)
        nuevo.width= im.size().width-nuevo.x;
    if (nuevo.y+nuevo.height>im.size().height)
        nuevo.height= im.size().height-nuevo.y;
    foto[factual].roi= nuevo;
}

//---------------------------------------------------------------------------

void cb_ver_seleccion (int factual, int x, int y, bool foto_roi)
{
    Mat res= foto[factual].img.clone();
    Point p1, p2;
    if (foto_roi) {
        p1.x= foto[factual].roi.x;
        p1.y= foto[factual].roi.y;
        p2.x= foto[factual].roi.x+foto[factual].roi.width-1;
        p2.y= foto[factual].roi.y+foto[factual].roi.height-1;
    }
    else {
        p1= Point(downx, downy);
        p2= Point(x,y);
    }
    rectangle(res, p1, p2, CV_RGB(255,foto_roi?0:255,0),2);
    imshow(foto[factual].nombre, res);
}

//---------------------------------------------------------------------------

void actualizar_dimperspectiva (int x, int y){
    //Actualizamos con la dimensión X introducida por el usuario
    dimperspectiva[0] = x;
    //Actualizamos con la dimensión Y introducida por el usuario
    dimperspectiva[1] = y;
}

void cb_perspectiva (){
    //Calculamos la perspectiva a partir de los cuatro puntos seleccionados por el usuario
    Mat matriz3x3 = getPerspectiveTransform(perspectivaorigen, //puntos de la primera imagen selecionadmos por el usuario
                                            perspectivadestino); //dimensiones de la imagen destino
    Mat imgtransformada;

    //Aplicamos la transformación perspectiva a la imagen original.
    //El resultado se almacenará en 'imgtransformada'
    warpPerspective(foto[foto_activa()].img,
            imgtransformada,
            matriz3x3,
            cv::Size(dimperspectiva[0],dimperspectiva[1])
            );

    //Mostramos la imagen perspectiva generada
    crear_nueva(primera_libre(),imgtransformada);
}

//Lo que ocurre cuando el usuario pincha con el ratón
//Este callback engloba todos los eventos posibles
void callback (int event, int x, int y, int flags, void *_nfoto)
{
    int factual= (int) _nfoto;

    // 1. Eventos y casos especiales
    // 1.1. Evento cerrar ventana
    if (event==EVENT_CLOSE) {
        cb_close(factual);
        return;
    }

    // 1.2. El ratón se sale de la ventana
    if (x>=foto[factual].img.size().width || y>=foto[factual].img.size().height)
        return;

    // 1.3. Se inicia la pulsación del ratón (evento de click)
    if (event==EVENT_LBUTTONDOWN) {
        downx= x;
        downy= y;
    }

    // 2. Según la herramienta actual
    switch (herr_actual) {
        // 2.1. Herramienta PUNTOS
        case puntos:
            if (flags==EVENT_FLAG_LBUTTON)
                cb_punto(factual, x, y);
            else
                //En el caso que pase el ratón por encima del panel pero sin hacer click
                ninguna_accion(factual, x, y);
            break;
            // 2.2. Herramienta LINEAS
        case lineas:
            if (event==EVENT_LBUTTONUP)
                cb_linea(factual, x, y);
            else if (event==EVENT_MOUSEMOVE && flags==EVENT_FLAG_LBUTTON)
                cb_ver_linea(factual, x, y);
            else
                ninguna_accion(factual, x, y);
            break;
            // 2.3. Herramienta RECTANGULOS
        case rectangulos:
            if (event==EVENT_LBUTTONUP)
                cb_rectangulo(factual, x, y);
            else if (event==EVENT_MOUSEMOVE && flags==EVENT_FLAG_LBUTTON)
                cb_ver_rectangulo(factual, x, y);
            else
                ninguna_accion(factual, x, y);
            break;
            // 2.4. Herramienta ELIPSE
        case elipses:
            if (event==EVENT_LBUTTONUP)
                cb_elipse(factual, x, y);
            else if (event==EVENT_MOUSEMOVE && flags==EVENT_FLAG_LBUTTON)
                cb_ver_elipse(factual, x, y);
            else
                ninguna_accion(factual, x, y);
            break;

            // 2.5. Herramienta POLIGONO
        case poligonos:
            if (event==EVENT_LBUTTONDOWN){//cada vez que pulsamos con el ratón añadimos un punto
                apuntos[npuntos] = Point(x,y);
                npuntos++;
            }
            if(event == EVENT_RBUTTONUP)//Cuando termina la pulsación pintamos el polígono
            {
                cb_poligono(factual);
                npuntos = 0;
            }
            else if(npuntos>1) //siempre que haya más de un prunto previsualizamos
                cb_ver_poligono(factual);

            break;
            // 2.6. Herramienta SELECCIONAR
        case seleccionar:
            if (event==EVENT_LBUTTONUP)
                cb_seleccionar(factual, x, y);
            else if (event==EVENT_MOUSEMOVE)
                cb_ver_seleccion(factual, x, y, flags!=EVENT_FLAG_LBUTTON);
            break;
            // 2.7. Herramienta ARCOIRIS
        case arcoiris:
            if (flags==EVENT_FLAG_LBUTTON)
                cb_arcoiris(factual, x, y);
            else
                //En el caso que pase el ratón por encima del panel pero sin hacer click
                ninguna_accion(factual, x, y);
            break;
        case perspectiva:

            if(npuntos >= 0 && npuntos <= 3){

                if (event==EVENT_LBUTTONDOWN){
                    //Almacenamos la posición de la pulsación de ratón
                    perspectivaorigen[npuntos] = Point2f( x, y );

                    //Si hemos alcanzado los cuatro puntos
                    if(npuntos == 3){
                        //Reiniciamos el contador
                        npuntos = 0;
                        //Actualizamos el array con las dimensiones
                        //que el usuario introdujo
                        perspectivadestino[0] = Point2f{0,0};
                        perspectivadestino[1] = Point2f{dimperspectiva[0],0};
                        perspectivadestino[2] = Point2f{0,dimperspectiva[1]};
                        perspectivadestino[3] = Point2f{dimperspectiva[0],dimperspectiva[1]};

                        //Calculamos las perspectiva.
                        cb_perspectiva();

                    }
                    else{
                        //Si no, incrementamos la variable
                        npuntos++;
                    }
                }
                else
                    //En el caso que pase el ratón por encima del panel pero sin hacer click
                    ninguna_accion(factual, x, y);
            }
            break;
            case texto:
                if (event == EVENT_LBUTTONDOWN){
                    Texto texto;
                    texto.exec();
                    dibujar_texto(x, y);
                }
                else{
                    //En el caso que pase el ratón
                    //por encima del panel pero sin hacer click
                    ninguna_accion(factual, x, y);
                }
                break;
            break;
    }
}


///////////////////////////////////////////////////////////////////
/////////  FUNCIONES DE PROCESAMIENTO DE IMAGENES    //////////////
///////////////////////////////////////////////////////////////////

void invertir (int nfoto, int nres)
{
    Mat img(foto[nfoto].img.size(), foto[nfoto].img.type());
    bitwise_not(foto[nfoto].img, img);
    crear_nueva(nres, img);
}

//---------------------------------------------------------------------------

void rotar_angulo (int nfoto, Mat &imgRes, double angulo, double escala, int modo)
{
    Mat imagen= foto[nfoto].img;
    double w= imagen.size().width, h= imagen.size().height;
    Size sres;
    if (modo==0) {
        sres.width= w*escala;
        sres.height= h*escala;
    }
    else if (modo==1)
        sres.width= sres.height= sqrt(w*w + h*h)*escala;
    else
        sres.width= sres.height= min(w, h)*escala/sqrt(2.0);
    imgRes.create(sres, imagen.type());
    double sa= sin(angulo*M_PI/180), ca= cos(angulo*M_PI/180);
    double cx= -w/2*ca-h/2*sa, cy= w/2*sa-h/2*ca;
    Mat M= getRotationMatrix2D(Point2f(0,0), angulo, escala);
    M.at<double>(0,2)= sres.width/2+cx*escala;
    M.at<double>(1,2)= sres.height/2+cy*escala;
    imgRes= color_pincel;
    warpAffine(imagen, imgRes, M, sres);
}

//---------------------------------------------------------------------------

void rotar_exacto (int nfoto, int nres, int grado)
{
    Mat entrada= foto[nfoto].img;
    Mat salida;
    if (grado==0)
        salida= entrada.clone();
    else if (grado==1) {
        transpose(entrada, salida);
        flip(salida, salida, 1);
    }
    else if (grado==2)
        flip(entrada, salida, -1);
    else if (grado==3) {
        transpose(entrada, salida);
        flip(salida, salida, 0);
    }
    crear_nueva(nres, salida);
}

//---------------------------------------------------------------------------

void ver_brillo_contraste (int nfoto, int suma, double prod, double gama, bool guardar)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && foto[nfoto].usada);
    Mat img;
    foto[nfoto].img.convertTo(img, CV_8UC3, prod, suma);

    Mat img32f;
    img.convertTo(img32f, CV_32FC3, 1.0/255);
    cv::pow(img32f, gama, img32f);
    img32f.convertTo(img, CV_8UC3, 255);

    imshow(foto[nfoto].nombre, img);
    if (guardar) {
        img.copyTo(foto[nfoto].img);
        foto[nfoto].modificada= true;
    }
}

//---------------------------------------------------------------------------

void ver_suavizado (int nfoto, int ntipo, int tamx, int tamy, bool guardar)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && foto[nfoto].usada);
    Mat img= foto[nfoto].img.clone();
    if (ntipo==CV_BLUR)
        blur(foto[nfoto].img, img, Size(tamx, tamy));
    else if (ntipo==CV_GAUSSIAN)
        GaussianBlur(foto[nfoto].img, img, Size(tamx, tamy), 0);
    else if(ntipo == CV_MEDIAN)
        medianBlur(foto[nfoto].img, img, tamx*2+1);//tamx*2+1 para conseguir un número impar
    else if(ntipo == 5){
        Mat mapa1(img.size(), CV_32FC1);
        Mat mapa2(img.size(), CV_32FC1);
        for (int y= 0; y<img.size().height; y++)
            for (int x= 0; x<img.size().width; x++) {
                /*float a = x+rand()%(2*tamx+1) - tamx;
                if(a > 0)
                    a = 0;
                else if(a>img.size().width)
                    a=img.size().width - 1;*/
                mapa1.at<float>(y, x)= x+rand()%(2*tamx+1) - tamx; // Con 'at' se accede primero a la posición 'y' y luego a la 'x'
                mapa2.at<float>(y, x)= y+rand()%(2*tamy+1) - tamy; // Mapa2
            }
        remap(foto[nfoto].img, img, mapa1, mapa2, INTER_NEAREST);
    }
    else if(ntipo == 6)//Acristalado
    {
        Mat mapa1(img.size(), CV_32FC1);
        Mat mapa2(img.size(), CV_32FC1);
        for (int y= 0; y<img.size().height; y++)
            for (int x= 0; x<img.size().width; x++) {
                mapa1.at<float>(y, x)= x - x%tamx + y%tamx;
                mapa2.at<float>(y, x)= y - y%tamy + x%tamy;
            }
        remap(foto[nfoto].img, img, mapa1, mapa2, INTER_NEAREST);

    }

    imshow(foto[nfoto].nombre, img);
    if (guardar) {
        img.copyTo(foto[nfoto].img);
        foto[nfoto].modificada= true;
    }
}

//---------------------------------------------------------------------------

void media_ponderada (int nf1, int nf2, int nueva, double peso)
{
    Mat img= foto[nf1].img.clone();
    Mat cop;
    resize(foto[nf2].img, cop, img.size());
    addWeighted(img, peso, cop, 1.0-peso, 0, img);
    crear_nueva(nueva, img);
}

//---------------------------------------------------------------------------

void ecualiza_histograma(int nfoto)
{
    //TODO lo recomendable sería convertir la imagen a escala de grises y ecualizala
    vector<Mat> canales;
    split(foto[nfoto].img, canales);

    for(int i = 0; i< 3; i++)
    {
        equalizeHist(canales[i], canales[i]);
    }

    Mat res;
    cv::merge(canales, res);

    crear_nueva(primera_libre(), res);
}

//---------------------------------------------------------------------------

string Lt1(string cadena)
{
    QString temp= QString::fromUtf8(cadena.c_str());
    return temp.toLatin1().data();
}

//---------------------------------------------------------------------------

void espacio_color (int nfoto, int espacio)
{
    Mat res;

    cvtColor(foto[nfoto].img, res, espacio);
    vector<Mat> canales;
    split(res, canales);
    cvtColor(canales[0], canales[0], COLOR_GRAY2BGR);
    cvtColor(canales[1], canales[1], COLOR_GRAY2BGR);
    cvtColor(canales[2], canales[2], COLOR_GRAY2BGR);
    crear_nueva(primera_libre(), canales[0]);
    crear_nueva(primera_libre(), canales[1]);
    crear_nueva(primera_libre(), canales[2]);

}
//---------------------------------------------------------------------------

void balance_blancos (int nfoto){
    //Convertimos la imagen a YCrCb
    Mat yub;

    cvtColor(foto[nfoto].img, yub, COLOR_BGR2YUV);

    //Calculamos la media de los canales para que el canal medio de luz sea 128
    Scalar media = mean(yub);
    Scalar suma;
    suma.val[0] = 0;
    suma.val[1] = 128 - media.val[1];
    suma.val[2] = 128 - media.val[2];
    add(yub, suma, yub);
    //Convertimos la imagen de nuevo a BGR
    cvtColor(yub, yub, COLOR_YUV2BGR);
    //Creamos la nueva imagen
    crear_nueva(primera_libre(), yub);

}

//---------------------------------------------------------------------------

void mostrar_histograma (int nfoto, int ncanal){
    int canales[1] = {ncanal};
    int bins[1] = {256};
    float rango[2] = {0,256};
    const float *rangos[] = {rango};
    //Almacena la imagen resultante
    Mat hist;
    calcHist(&(foto[nfoto].img), 1, canales, noArray(), hist, 1, bins, rangos);
    Mat imghist = histbase.clone();
    double mini, maxi;
    minMaxLoc(hist, &mini, &maxi);
    for (int i= 0; i<256; i++){
        //Los valores fijos los toma del propio tamaño de la imagen del histograma
        rectangle(imghist, Rect(i*392.0/256, 185-182*hist.at<float>(i)/maxi,
                  392.0/256, 182*hist.at<float>(i)/maxi), CV_RGB(0,0,0), -1);
    }

    crear_nueva(primera_libre(),imghist);
        //qDebug("Celda %d: %g", i, hist.at<float>(i));
}

//---------------------------------------------------------------------------

void capturar_desde_camara (){
    VideoCapture cap;
    //Utilizamos 0 para capturar desde la cámara por defecto
    cap.open(0);

    //Comprobamos si se ha abierto la cámara correctamente
    if(!cap.isOpened())  {
        qDebug("Error, no se puede abrir el capturador.");
        return;
    }
    else{
        //Esperamos a que el usuario pulse una tecla
        while (waitKey(1) == -1) {
            Mat frame;
            if (!cap.read(frame))
                break;
            //Mostramos la imagen del frame
            imshow("Frame", frame);
        }
    }
}

//---------------------------------------------------------------------------

void capturar_desde_video(){
    //Abrimos el fichero de vídeo desde archivo
    QString nombre= QFileDialog::getOpenFileName();

    //Hacemos que openCV reconozca el fichero abierto como un vídeo
    //e iniciamos su reproducción
    VideoCapture cap;
    cap.open(nombre.toLatin1().data());

    //Para establecer el número de frames por segundo (30 por defecto):
    //     cap.set(CAP_PROP_FPS, 30);
    //Para leer el número de frames por segundo:
    //     double fps = cap.get(CV_CAP_PROP_FPS);

    //Esperamos a que el usuario pulse una tecla
    while (waitKey(1)==-1) {
        //En el momento en que el usuario pulse una tecla, se parará
        //la reproducción del vídeo, mostrando por pantalla el
        //último frame capturado.
        Mat frame;
        if (!cap.read(frame))
            break;
        imshow("Frame", frame);
    }
}

//---------------------------------------------------------------------------
void copiar_a_nueva(int fotoactiva, int fotonueva){
    //Obtenemos la región de interés seleccionada de la imagen actual.
    Rect ROI = foto[fotoactiva].roi;
    //Si la imagen tiene una región de interés seleccionada
    if(ROI != Rect(-1, -1, -1, -1)){
        //Creamos una imagen a partir del fragmento
        Mat fragmento = foto[fotoactiva].img(ROI);
        //Mostramos la imagen
        imshow("ROI", fragmento);

    }
    else{
        //Si no hay región de interés seleccionada, copio la imagen completa
        crear_nueva (fotonueva, foto[fotoactiva].img);
    }
}

//---------------------------------------------------------------------------

void mostrar_histograma2D (int fotoactiva, int canales[], int dimx, int dimy){

    //---------------------------
    // Calculamos el histograma
    //---------------------------

    //Imagen de la cual obtendremos su histograma
    Mat image = foto[fotoactiva].img;

    // Tamaño del histograma seleccionado por el usuario. En teoría,
    // el histograma es de 256x256 celdas (bins). Pero, para obtener buenos
    // resultados, como máximo tendrá un número máximo de 32x32 celdas.
    int tamhist[] = {dimx, dimy};

    // El tono de color varia desde los valores 0 a 179 (según la documentación de cvtColor)
    float colorranges[] = { 0, 180 };

    // La imagen del histograma tiene los siguientes límites (8bits)
    float histranges[] = { 0, 256 };

    //matriz que indica los límites
    const float* ranges[] = { colorranges, histranges };

    //Imagen donde guardaremos el histograma resultante
    MatND hist;
    calcHist( &image,
              1,  //histograma a partir de una única imagen origen
              canales, Mat(), // sin máscara (aplicar una máscara sirve para si queremos
              //calcular el histograma de una región de la imagen)
              hist, 2, tamhist, ranges,
              true, // el histograma es uniforme
              false ); //el histograma no será resultado de un acumulado (conjunto de imágenes)


    //---------------------------
    // Dibujamos el histograma
    //---------------------------

    double valormaximo_hist = 0;
    //Obtenemos el valor máximo del histograma
    minMaxLoc(hist, 0, &valormaximo_hist, 0, 0);

    int escala = 10;
    //Imagen que se mostrará con los puntos del histograma (lo multiplicamos por diez para que
    //tenga un tamaño más grande que 32x32)
    Mat histImg = Mat(dimy*escala, dimx*escala, CV_8UC3, Scalar(255,255,255));

    //por cada 'celda' de la imagen
    for( int x = 0; x < dimx; x++ )
        for( int y = 0; y < dimy; y++ )
        {
            //obtenemos el valor de esa celda en la imagen original
            float valorceldaactual = hist.at<float>(x, y);

            int intensity = cvRound(valorceldaactual*255/valormaximo_hist);

            //Dibujamos un rectángulo relleno en cada celda. Será de un color u otro según
            //la intensidad calculada
            rectangle( histImg, //imagen que contendrá el rectángulo
                       Point((x*escala)+6, (y*escala)+6), //vértice del rectángulo (le sumamos
                       //6 porque ese es el tamaño que ocupará la escala de color)
                       Point(((x+1)*escala)+6 - 1, ((y+1)*escala)+6 - 1), //vértice del rectángulo
                       //(opuesto al anterior)(le sumamos 6 porque ese es el tamaño que ocupará
                       //la escala de color)
                       Scalar::all(intensity), //el rectángulo tendrá una intensidad u otra
                       //dependiendo del valor de su celda
                       CV_FILLED );//rectángulo relleno
        }

    //---------------------------
    // Creamos la escala de color
    //---------------------------
    int azul=0,verde=0,rojo=0;
    int y = 0;

    //Color R-G
    if(canales[0] == 2 && canales[1] == 1){
        while(rojo < 255 && verde < 255)
        {
          verde++;
          rojo++;
          y++;

          //Dibujamos en el eje de las x (donde está el rojo)
          rectangle(histImg,Point(y,0),Point(y+1,5),Scalar(0,0,rojo),1);
          rectangle(histImg,Point(y,5),Point(y+1,6),Scalar(64,64,64),1);
          //Dibujamos en el eje de las y (donde está el verde)
          rectangle(histImg,Point(0,y),Point(5,y+1),Scalar(0,verde,0),1);
          rectangle(histImg,Point(5,y),Point(6,y+1),Scalar(64,64,64),1);
        }
    }
    //Color R-B
    else if(canales[0] == 2 && canales[1] == 0){
        while(rojo < 255 && azul < 255)
        {
          azul++;
          rojo++;
          y++;
          //Dibujamos en el eje de las y (donde está el azul)
          rectangle(histImg,Point(0,y),Point(5,y+1),Scalar(azul,0,0),1);
          rectangle(histImg,Point(5,y),Point(6,y+1),Scalar(64,64,64),1);
          //Dibujamos en el eje de las x (donde está el rojo)
          rectangle(histImg,Point(y,0),Point(y+1,5),Scalar(0,0,rojo),1);
          rectangle(histImg,Point(y,5),Point(y+1,6),Scalar(64,64,64),1);
        }
    }
    //Color B-G
    else if(canales[0] == 0 && canales[1] == 1){
        while(azul < 255 && verde < 255)
        {
          azul++;
          verde++;
          y++;
          //Dibujamos en el eje de las y (verde)
          rectangle(histImg,Point(0,y),Point(5,y+1),Scalar(0,verde,0),1);
          rectangle(histImg,Point(5,y),Point(6,y+1),Scalar(64,64,64),1);
          //Dibujamos en el eje de las x (azul)
          rectangle(histImg,Point(y,0),Point(y+1,5),Scalar(azul,0,0),1);
          rectangle(histImg,Point(y,5),Point(y+1,6),Scalar(64,64,64),1);
        }
    }
    crear_nueva(primera_libre(),histImg);
}

//---------------------------------------------------------------------------

void rotar_reescalar (int x, int y, int angulo)
{
    //Imagen original
    Mat imagenoriginal = foto[foto_activa()].img;
    //Imagen que contendrá la imagen reescalada
    Mat imagenreescalada;
    //Localizamos el centro de la imagen
    cv::Point2f centro(x/2, y/2);

    //Para reescalar, utilizamos el método 'resize' de opencv
    resize(imagenoriginal, imagenreescalada, Size(x,y));

    //Para rotar la imagen, utilizamos el método 'getRotationMatrix2D' de opencv
    //Retorna la matriz afín de rotación
    Mat matriz = getRotationMatrix2D(centro, //rotamos la imagen desde su centro
                                     angulo,//la rotramos el ángulo indicado por la persona
                                     1); //escala

    Mat imagenfinal;
    //Una vez obtenida la matriz, aplicamos la transformación afín a la imagen reescalada
    warpAffine(imagenreescalada,imagenfinal,matriz,Size(x,y));
    //Mostramos la imagen
    crear_nueva(primera_libre(),imagenfinal);
}

//---------------------------------------------------------------------------

void escala_grises(){
    //Obtenemos la imagen abierta por el usuario
    Mat imagenactiva = foto[foto_activa()].img;
    //Imagen que almacenará la imagen en escala de grises
    Mat imagengris;
    //Cambiamos a escala de grises
    cv::cvtColor(imagenactiva, //imagen abierta por el usuario
                 imagengris, //imagen resultado en escala de grises
                 cv::COLOR_BGR2GRAY, //espacio de color
                 3 //Número de canales de la imagen resultante
                 );
    //Mostramos la imagen
    crear_nueva(primera_libre(),imagengris);
}

//---------------------------------------------------------------------------

void color_falso(){
    //Convertimos a escala de grises la imagen activa
    escala_grises();
    //Obtenemos la imagen abierta por el usuario
    Mat imagengris = foto[foto_activa()].img;
    Mat imagencolorfalso;
    //Aplicamos la paleta de color falso
    applyColorMap(imagengris, imagencolorfalso, COLORMAP_HSV);
    //mostramos la imagen
    crear_nueva(primera_libre(),imagencolorfalso);

}

//---------------------------------------------------------------------------
void actualizar_variables_texto(string texto, double tam, Scalar color)
{
    textoadibujar = texto;
    tamtexto = tam;
    colortexto = color;
}

void dibujar_texto(int x, int y){
    int fotoactiva = foto_activa();
    //Obtenemos la imagen abierta por el usuario
    Mat imagenactiva = foto[fotoactiva].img;
    Mat capa;
    double alfa = 0.3;

    // Copiamos la imagen origen a la imagen que hará de capa de transparencia
    imagenactiva.copyTo(capa);
    //Dibujamos la sombra del texto
    //Consiste en dibujar texto con una pequeña desviación en su posición
    //y que al estar dibujado sobre la imagen 'capa' se visualizará
    //con transparencia
    putText(capa, textoadibujar, Point(x+3,y+3), FONT_HERSHEY_SIMPLEX,
            tamtexto,Scalar(0,0,0), 2.0, 4);
    //Suavizamos el texto
    blur(capa, capa, Size(4, 4));

    //Mezclamos la imagen original junto con la imagen capa indicando el
    //factor transparencia
    cv::addWeighted(capa, alfa, imagenactiva, 1 - alfa, 0, imagenactiva);

    //Añadimos el texto para que quede superpuesto sobre su sombra
    putText(imagenactiva, textoadibujar, Point(x,y), FONT_HERSHEY_SIMPLEX,
            tamtexto, colortexto, 2.0, 4);

    //Mostraos la imagen
    foto[fotoactiva].modificada = true;
}
