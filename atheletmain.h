#ifndef ATHELETMAIN_H
#define ATHELETMAIN_H

#include <QDialog>

namespace Ui {
class AtheletMain;
}

class AtheletMain : public QDialog
{
    Q_OBJECT

public:
    explicit AtheletMain(QWidget *parent = 0,QString usrName="");
    ~AtheletMain();
    QString usrname;

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);

private slots:

    void on_btnMenu_Min_clicked();

    void on_btnMenu_Max_clicked();

    void on_btnMenu_Close_clicked();

    void readJSONData();

private slots:
    void on_pushButton_Search_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_table_Event_cellDoubleClicked(int row, int column);

    int checkConflict(int eventID1,int eventID2);

    int checkList(QList<QString> list,QString arg);

    void addChart();
    void on_btnMenu_clicked();

private:
    Ui::AtheletMain *ui;

    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
    int readComplete_,refreshComplete_;
    void InitStyle();
};

#endif // ATHELETMAIN_H
