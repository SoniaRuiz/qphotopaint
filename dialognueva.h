#ifndef DIALOGNUEVA_H
#define DIALOGNUEVA_H

#include <QDialog>

namespace Ui {
    class DialogNueva;
}

class DialogNueva : public QDialog {
    Q_OBJECT
public:
    DialogNueva(QWidget *parent = 0);
    ~DialogNueva();
    int red();
    int green();
    int blue();
    int getWidth();
    int getHeight();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogNueva *ui;
    QColor color;

private slots:
    void on_pushButton_clicked();
};

#endif // DIALOGNUEVA_H
