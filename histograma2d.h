#ifndef HISTOGRAMA2D_H
#define HISTOGRAMA2D_H

#include <QDialog>

namespace Ui {
class histograma2d;
}

class histograma2d : public QDialog
{
    Q_OBJECT

public:
    explicit histograma2d(QWidget *parent = 0);
    ~histograma2d();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::histograma2d *ui;
};

#endif // HISTOGRAMA2D_H
