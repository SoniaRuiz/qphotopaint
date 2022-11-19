#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

extern QString FiltroImagen;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setStatusBarText(QString cadena);
    int comprobar_primera_libre (void);
    void show();
protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;

private slots:
    void on_actionImagen_media_triggered();
    void on_actionAcerca_de_triggered();
    void on_actionRotar_imagen_triggered();
    void on_actionMedia_ponderada_triggered();
    void on_actionMedia_triggered();
    void on_actionGausiano_triggered();
    void on_actionBrillo_contraste_triggered();
    void on_actionRotar_91_triggered();
    void on_actionRotar_180_triggered();
    void on_actionRotar_90_triggered();
    void on_actionInvertir_triggered();
    void on_toolButton_6_clicked();
    void on_toolButton_5_clicked();
    void on_toolButton_4_clicked();
    void on_actionSeleccionar_todo_triggered();
    void on_actionColor_del_pincel_triggered();
    void on_actionPreguntar_si_guardar_triggered();
    void on_actionCerrar_triggered();
    void on_actionGuardar_como_triggered();
    void on_actionGuardar_triggered();
    void on_toolButton_clicked();
    void on_horizontalSlider_2_valueChanged(int value);
    void on_horizontalSlider_valueChanged(int value);
    void on_pushButton_clicked();
    void on_toolButton_2_clicked();
    void on_actionSeleccionar_triggered();
    void on_actionLinea_triggered();
    void on_actionPunto_triggered();
    void on_actionAbrir_imagen_triggered();
    void on_actionNueva_imagen_triggered();
    void on_actionSalir_triggered();

    void on_actionRect_ngulo_triggered();
    void on_toolButton_7_clicked();
    void on_toolButton_8_clicked();
    void on_actionElipses_triggered();
    void on_actionTri_ngulo_triggered();
    void on_toolButton_9_clicked();
    void on_actionMediana_triggered();
    void on_actionAleatorio_triggered();
    void on_actionAcristalado_triggered();
    void on_actionXYZ_triggered();
    void on_actionHLS_triggered();
    void on_actionYCrCb_triggered();
    void on_actionLab_triggered();
    void on_actionRGB_triggered();
    void on_actionBalance_de_blancos_triggered();
    void on_actionR_triggered();
    void on_actionG_triggered();
    void on_actionB_triggered();
    void on_actionEcualizado_triggered();
    void on_actionCapturar_de_c_mara_triggered();
    void on_actionCapturar_de_v_deo_triggered();
    void on_actionCopiar_a_nueva_triggered();

    void on_actionVer_histograma_2D_triggered();
    void on_actionArco_irirs_triggered();
    void on_toolButton_10_clicked();
    void on_actionPerspectiva_triggered();
    void on_actionRotar_imagen_y_reescalar_triggered();
    void on_actionConvertir_a_escala_de_grises_triggered();
    void on_actionColor_falso_triggered();
    void on_actionTexto_triggered();
};

extern MainWindow *w;
extern QApplication *a;

#endif // MAINWINDOW_H
