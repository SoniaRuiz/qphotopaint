#ifndef IMAGENES_H
#define IMAGENES_H

#include <windows.h>
#include <string>
using namespace std;
#include <opencv2/opencv.hpp>
using namespace cv;

///////////////////////////////////////////////////////////////////
/////////  DEFINICIONES PUBLICAS                     //////////////
///////////////////////////////////////////////////////////////////

#define MAX_VENTANAS 50

#define min(a, b)      (((a) < (b)) ? (a) : (b))
#define max(a, b)      (((a) > (b)) ? (a) : (b))


///////////////////////////////////////////////////////////////////
/////////  TIPOS DE DATOS PROPIOS                    //////////////
///////////////////////////////////////////////////////////////////

// STRUCT ventana
//    Representa la información asociada a una imagen que está siendo
//    editada actualmente en la aplicación

struct ventana {
  bool usada;          // Indica si se está usando actualmente esta posición
  bool modificada;     // Imagen modificada después de abrirla o crearla
  string nombre;       // Nombre del fichero que contiene la imagen
  HWND handle;         // Manejador de Windows de la ventana de HighGUI
  Rect roi;            // Región de interés seleccionada (-1 si no hay ninguna)
  Mat img;             // La imagen en sí
};

// ENUM tipo_herramienta
//    Enumerado con los distintos tipos posibles de herramientas
//    que se pueden usar. Añadir nuevas aquí

enum tipo_herramienta {puntos, lineas, seleccionar, rectangulos, elipses, poligonos, arcoiris, perspectiva, texto};

//TODO: HACER EL PINCEL CONTINUO
///////////////////////////////////////////////////////////////////
/////////  VARIABLES PÚBLICAS                        //////////////
///////////////////////////////////////////////////////////////////

extern ventana foto[MAX_VENTANAS];
// El array foto guarda la información de todas las imágenes abiertas
// actualmente o que pueden ser abiertas, hasta un máximo de MAX_VENTANAS

extern tipo_herramienta herr_actual;
// Herramienta que se ha seleccionado actualmente

extern int radio_pincel;
// Radio actual para pintar puntos y líneas

extern Scalar color_pincel;
// Color actual del pincel

extern int difum_pincel;
// Difuminado actual del pincel

extern bool preguntar_guardar;
// Al cerrar una imagen modificada, preguntar si se guarda o no



///////////////////////////////////////////////////////////////////
/////////  FUNCIONES DE MANEJO DE VENTANAS           //////////////
///////////////////////////////////////////////////////////////////

void inic_fotos (void);
// Inicializa el array foto, de imágenes abiertas actualmente

void fin_fotos (void);
// Libera las imágenes del array foto

int primera_libre (void);
// Busca la primera entrada libre del array foto

void crear_nueva (int nfoto, int ancho, int alto, Scalar color);
// Crea una nueva entrada, en la posición nfoto, con el ancho y el alto dados,
// de profundidad IPL_DEPTH_8U y 3 canales. color indica el color de fondo

void crear_nueva (int nfoto, Mat img);
// Crea una nueva entrada, en la posición nfoto, usando la imagen dada en img

void crear_nueva (int nfoto, string nombre);
// Crea una nueva entrada, en la posición nfoto, leyéndola del archivo nombre.
// Si no se puede leer, no hace nada

void mostrar (int nfoto);
// Muestra la imagen de la entrada nfoto en la ventana correspondiente

void guardar_foto (int nfoto, string nombre= "");
// Guarda en disco la imagen de nfoto, con el nombre dado. Si el nombre
// es NULL, usa el nombre de la imagen en el array foto. Si cerrar es false,
// se cierra la ventana con la imagen (modo "guardar y cerrar")

void cerrar_foto (int nfoto, bool destruir_ventana= true);
// Elimina la entrada en la posición nfoto. destruir_ventana indica si debe
// destruirse la ventana de HighGUI (ya que si lo ha hecho el usuario, no
// debe volver a hacerse)

int nombre_a_numero (string nombre);
// Dado un nombre de ventana, busca su número en el array foto.
// Si no lo encuentra devuelve -1

int foto_activa (void);
// Devuelve el número de la ventana de imagen activa (la que esté en primer
// lugar). Si no hay ninguna devuelve -1

int num_fotos (int &usadas, int &modificadas);
// Devuelve el número de fotos que hay abiertas actualmente

///////////////////////////////////////////////////////////////////
/////////  FUNCIONES DE PROCESAMIENTO DE IMAGENES    //////////////
///////////////////////////////////////////////////////////////////

void invertir (int nfoto, int nres);
// Invierte la imagen contenida en nfoto y la almacena en nres

void rotar_angulo (int nfoto, Mat &imgRes, double angulo, double escala= 1.0, int modo= 0);
// Rota la imagen contenida en nfoto, en la cantidad grado
// y la almacena el resultado en imgRes. La imagen de salida
// siempre está centrada. modo indica: 0 = el tamaño de la imagen
// de salida es el mismo que la original; 1 = tamaño máximo (caber todo);
// 2 = tamaño mínimo (sin bordes)

void rotar_exacto (int nfoto, int nres, int grado);
// Rota la imagen contenida en nfoto, en la cantidad 90*grado
// y la almacena en nres. Se trata siempre de una rotación exacta

void ver_brillo_contraste (int nfoto, int suma, double prod, double gama, bool guardar = false);
// Modifica la imagen en nfoto, tomando para cada píxel A(x,y) el valor:
// A(x,y)*prod + suma. Si guardar==true se guarda la imagen, y en otro caso
// sólo se visualiza el resultado, pero sin guardar

void ver_suavizado (int nfoto, int ntipo, int tamx, int tamy,
                    bool guardar = false);
// Modifica la imagen en nfoto, aplicando un suavizado de tamaño dado. ntipo
// puede ser CV_GAUSSIAN o CV_BLUR. Si guardar==true se guarda la imagen, y
// en otro caso sólo se visualiza el resultado, pero sin guardar

void media_ponderada (int nf1, int nf2, int nueva, double peso);
// Calcula la media ponderada entre la imagen nf1 y la nf2, según el peso dado,
// y almacena el resultado en la posición nueva. Se aplica la fórmula:
// nueva(x,y):= peso*nf1(x,y)+(1-peso)*nf2(x,y). La imagen nf2 se reescala al
// tamaño de nf1

void espacio_color (int nfoto, int espacio);

void balance_blancos (int nfoto);

void mostrar_histograma (int nfoto, int ncanal);

void ecualiza_histograma (int nfoto);

//Método que activa la cámara por defecto conectada al PC y toma una imagen cuando el usuario pulsa una tecla.
void capturar_desde_camara();

//Método que permite al usuario seleccionar un vídeo, y toma una imagen de dicho vídeo cuando el usuario pulsa una tecla.
void capturar_desde_video();

//Método que copia una ROI de la imagen activa a una imagen nueva. Si no hay ROI seleccionado, la copia entera.
void copiar_a_nueva(int fotoActiva, int fotoNueva);

//Método que muestra el histograma en 2D para los canales seleccionados
void mostrar_histograma2D (int fotoactiva, int canales[], int dimx, int dimy);

//Método para actualizar variables globales relacionadas con la perspectiva de una imagen.
void actualizar_dimperspectiva (int x, int y);

//Método para rotar y reescalar una imagen
void rotar_reescalar (int x, int y, int angulo);

//Método para convertir una imagen a escala de grises
void escala_grises();

//Método para convertir una imagen en escala de grises a una paleta de color falso
void color_falso();

//Método para actualizar variables globales relacionadas con dibujar texto sobre una imagen
void actualizar_variables_texto(string texto, double tam, Scalar color);

//Método para dibujar texto encima de una imagen
void dibujar_texto(int x, int y);

string Lt1(string cadena);
// Convertir una cadena de UTF8 a Latin1

#endif // IMAGENES_H
