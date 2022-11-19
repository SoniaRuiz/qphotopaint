#-------------------------------------------------
#
# Project created by QtCreator 2015-10-18T21:16:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QFotoPaint3

TEMPLATE = app

SOURCES += main.cpp\
    mainwindow.cpp \
    imagenes.cpp \
    dialognueva.cpp \
    brillocontraste.cpp \
    suavizados.cpp \
    video.cpp \
    rotaravideo.cpp \
    mediaponderada.cpp \
    acercade.cpp \
    mediadevideo.cpp \
    histograma2d.cpp \
    transfperspectiva.cpp \
    rotaryreescalar.cpp \
    texto.cpp

HEADERS  += mainwindow.h \
    imagenes.h \
    dialognueva.h \
    brillocontraste.h \
    suavizados.h \
    video.h \
    rotaravideo.h \
    mediaponderada.h \
    acercade.h \
    mediadevideo.h \
    histograma2d.h \
    transfperspectiva.h \
    rotaryreescalar.h \
    texto.h

FORMS    += mainwindow.ui \
    dialognueva.ui \
    brillocontraste.ui \
    suavizados.ui \
    rotaravideo.ui \
    mediaponderada.ui \
    acercade.ui \
    mediadevideo.ui \
    histograma2d.ui \
    transfperspectiva.ui \
    rotaryreescalar.ui \
    texto.ui

INCLUDEPATH += "C:\OpenCV\OpenCV3.0.0\include"

LIBS += -L"C:\OpenCV\OpenCV3.0.0\lib"\
        -llibopencv_core300\
        -llibopencv_imgproc300\
        -llibopencv_imgcodecs300\
        -llibopencv_highgui300\
        -llibopencv_videoio300

RESOURCES += recursos.qrc
