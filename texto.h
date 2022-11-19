#ifndef TEXTO_H
#define TEXTO_H

#include <QDialog>

namespace Ui {
class Texto;
}

class Texto : public QDialog
{
    Q_OBJECT

public:
    explicit Texto(QWidget *parent = 0);
    ~Texto();

private slots:
    void on_buttonBox_accepted();

    void on_pushButton_clicked();

private:
    Ui::Texto *ui;
};

#endif // TEXTO_H
