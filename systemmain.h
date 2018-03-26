#ifndef SYSTEMMAIN_H
#define SYSTEMMAIN_H

#include <QDialog>

namespace Ui {
class SystemMain;
}

class SystemMain : public QDialog
{
    Q_OBJECT

public:
    explicit SystemMain(QWidget *parent = 0);
    ~SystemMain();

private slots:
    void on_pushButton_clicked();

private:
    Ui::SystemMain *ui;
};

#endif // SYSTEMMAIN_H
