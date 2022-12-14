//---------------------------------------------------------------------------

#include "video.h"
#include <math.h>

///////////////////////////////////////////////////////////////////
/////////  VARIABLES GLOBALES PRIVADAS               //////////////
///////////////////////////////////////////////////////////////////

VideoCapture camara;
// Capturador de cámara usado actualmente

Mat img_media;
// Imagen media acumulada de la cámara, con profundidad 32F

int frames_img_media;
// Número de frames que se han acumulado en la media img_media

string FiltroVideo= "Todos los formatos (*.avi *.mpg *.wmv *.mov);;Archivos AVI (*.avi);;Archivos MPG (*.mpg *.mpeg);;Archivos WMV (*.wmv);;Archivos MOV (*.mov);;Otros (*.*)";

///////////////////////////////////////////////////////////////////
/////////  FUNCIONES DE PROCESAMIENTO DE VIDEO       //////////////
///////////////////////////////////////////////////////////////////

void rotar_video (int nfoto, string nombre, int modo, int nframes, int codec, double fps)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && foto[nfoto].usada && !nombre.empty() && nframes>0);
    Mat rotada;
    rotar_angulo(nfoto, rotada, 0, 1, modo);
    VideoWriter writer(nombre, codec, fps, rotada.size());
    if (writer.isOpened()) {
        for (int i= 0; i<=nframes; i++) {
            rotar_angulo(nfoto, rotada, i*360.0/nframes, 1, modo);
            namedWindow(Lt1("Imagen a vídeo - Rotación"), 0);
            imshow(Lt1("Imagen a vídeo - Rotación"), rotada);
            waitKey(1);
            writer << rotada;
        }
        writer.release();
        destroyWindow(Lt1("Imagen a vídeo - Rotación"));
    }
}

//---------------------------------------------------------------------------

int inic_camara (int numero)
{
    if (camara.isOpened())
        camara.release();
    camara.open(numero);
    if (camara.isOpened()) {
        Mat img;
        namedWindow(Lt1("Imagen de cámara"), 0);
        camara >> img;
        resizeWindow(Lt1("Imagen de cámara"), img.size().width, img.size().height);
        imshow(Lt1("Imagen de cámara"), img);
        return 1000.0/30;
    }
    else
        return 0;
}

//---------------------------------------------------------------------------

void fin_camara (void)
{
    if (camara.isOpened()) {
        camara.release();
        destroyWindow(Lt1("Imagen de cámara"));
    }
    destroyWindow("Imagen media");
}

//---------------------------------------------------------------------------

void acumular_media (bool primera)
{
    assert(camara.isOpened());
    Mat frame;
    camara >> frame;
    namedWindow(Lt1("Imagen de cámara"), 0);
    imshow(Lt1("Imagen de cámara"), frame);
    if (primera) {
        frames_img_media= 1;
        frame.convertTo(img_media, CV_32FC3);
    }
    else {
        frames_img_media++;
        Mat frame32F;
        frame.convertTo(frame32F, CV_32FC3);
        img_media= frame32F + img_media;
        img_media.convertTo(frame, CV_8UC3, 1.0/frames_img_media);
    }
    namedWindow("Imagen media", 0);
    imshow("Imagen media", frame);
}

//---------------------------------------------------------------------------

void media_a_nueva (int nfoto)
{
    Mat res(img_media.size(), CV_8UC3);
    img_media.convertTo(res, CV_8UC3, 1.0/frames_img_media);
    crear_nueva (nfoto, res);
}

//---------------------------------------------------------------------------

void mostrar_camara (void)
{
    Mat img;
    camara >> img;
    namedWindow(Lt1("Imagen de cámara"), 0);
    imshow(Lt1("Imagen de cámara"), img);
}

//---------------------------------------------------------------------------
