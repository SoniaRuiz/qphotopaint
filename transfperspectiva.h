#ifndef TRANSFPERSPECTIVA_H
#define TRANSFPERSPECTIVA_H

#include <QDialog>

namespace Ui {
class transfperspectiva;
}

class transfperspectiva : public QDialog
{
    Q_OBJECT

public:
    explicit transfperspectiva(QWidget *parent = 0);
    ~transfperspectiva();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::transfperspectiva *ui;
};



#endif // TRANSFPERSPECTIVA_H
