#ifndef ROTARYREESCALAR_H
#define ROTARYREESCALAR_H

#include <QDialog>

namespace Ui {
class rotaryreescalar;
}

class rotaryreescalar : public QDialog
{
    Q_OBJECT

public:
    explicit rotaryreescalar(QWidget *parent = 0);
    ~rotaryreescalar();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::rotaryreescalar *ui;
};

#endif // ROTARYREESCALAR_H
