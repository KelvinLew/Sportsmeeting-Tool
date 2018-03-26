#ifndef ADMINMAIN_H
#define ADMINMAIN_H

#include <QDialog>
#include <QTableWidget>

namespace Ui {
class AdminMain;
}

class AdminMain : public QDialog
{
    Q_OBJECT

public:
    explicit AdminMain(QWidget *parent = 0);
    ~AdminMain();

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

    void on_pushButton_Add_clicked();

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_pushButton_Delet_clicked();

    void on_table_Event_itemChanged(QTableWidgetItem *item);

    void on_tabWidget_currentChanged(int index);

    void on_table_Candidate_cellChanged(int row, int column);

    void on_pushButton_Search_clicked();

    int checkConflict(int eventID1,int eventID2);

    int checkList(QList<QString> list,QString arg);

    void addChart();

    void on_pushButton_clicked();

    void on_btnMenu_clicked();

private:
    Ui::AdminMain *ui;

    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;

    void InitStyle();
};

#endif // ADMINMAIN_H
