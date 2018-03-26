#include "atheletmain.h"
#include "ui_atheletmain.h"
#include "iconhelper.h"
#include "myhelper.h"
#include "mjson.h"

QString usrname;
int readComplete_,refreshComplete_,colorstyle_;

AtheletMain::AtheletMain(QWidget *parent,QString usrName) :
    QDialog(parent),
    ui(new Ui::AtheletMain)
{
    ui->setupUi(this);

    usrname=usrName;
    myHelper::FormInCenter(this);
    this->InitStyle();
}

AtheletMain::~AtheletMain()
{
    delete ui;
}

void AtheletMain::InitStyle()
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);

    max = false;
    mousePressed = false;
    ui->lab_Title->installEventFilter(this);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Min, QChar(0xf068), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu, QChar(0xf0c9), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf015), 12);
    //设置不可编辑
    ui->table_Event->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table_Candidate->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->readJSONData();
}


bool AtheletMain::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void AtheletMain::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void AtheletMain::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void AtheletMain::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void AtheletMain::on_btnMenu_Max_clicked()
{
    if (max) {
        this->setGeometry(location);
        IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
        ui->btnMenu_Max->setToolTip("最大化");
    } else {
        location = this->geometry();
        this->setGeometry(qApp->desktop()->availableGeometry());
        IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf079), 10);
        ui->btnMenu_Max->setToolTip("还原");
    }
    max = !max;
}

void AtheletMain::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}

void AtheletMain::on_btnMenu_Close_clicked()
{
    qApp->exit();
}

void AtheletMain::readJSONData()
{
    QTableWidget *table_event;
    QComboBox *combobox_event;
    int eventNum,i;
    readJSON();
    readComplete_=0;
    //addMode=0;
    //组合框
    combobox_event=ui->comboBox;
    combobox_event->addItem("All");
    //项目管理
    table_event=ui->table_Event;
    eventNum=getEventNum();
    for(i=0;i<eventNum;i++){
          int row_count = table_event->rowCount(); //获取表单行数
          table_event->insertRow(row_count); //插入新行
          QTableWidgetItem *name = new QTableWidgetItem();
          QTableWidgetItem *time = new QTableWidgetItem();
          QTableWidgetItem *num = new QTableWidgetItem();
          QTableWidgetItem *status = new QTableWidgetItem();
          name->setText(getEventName(i));
          time->setText(getEventPlace(i,QString("").toLatin1().data()));
          num->setText(QString::number(getCandidateNum(i,QString("").toLatin1().data()),10));

          table_event->setItem(row_count, 0, name);
          table_event->setItem(row_count, 1, time);
          table_event->setItem(row_count, 2,num);

          if(getCandidateID(name->text().toLatin1().data(),usrname.toLatin1().data()) != -1){
              status->setText("YES");
          }else{
              status->setText("");
          }

          table_event->setItem(row_count, 3,status);

          combobox_event->addItem(name->text());
    }

    readComplete_=1;
    refreshComplete_=1;
}





void AtheletMain::on_pushButton_Search_clicked()
{
    int row,i;
    if(ui->tabWidget->currentIndex()==0){
        row=ui->table_Event->rowCount();
        for(i=0;i<row;i++){
            if(ui->table_Event->item(i,0)->text().data()==ui->lineEdit->text()){
               ui->table_Event->selectRow(i);
               break;
            }
        }
    }

    if(ui->tabWidget->currentIndex()==1){
        row=ui->table_Candidate->rowCount();
        for(i=0;i<row;i++){
            if(ui->table_Candidate->item(i,1)->text().data()==ui->lineEdit->text()){
               ui->table_Candidate->selectRow(i);
               break;
            }
        }
    }
}


void AtheletMain::on_tabWidget_currentChanged(int index)
{
    QTableWidget *table_event,*table_candidate;
    int eventNum,candidateNum,i,j;

    refreshComplete_=0;

    if(index==0 && readComplete_ == 1){
        table_event=ui->table_Event;
        table_event->setRowCount(0);
        eventNum=getEventNum();
        for(i=0;i<eventNum;i++){
              int row_count = table_event->rowCount(); //获取表单行数
              table_event->insertRow(row_count); //插入新行
              QTableWidgetItem *name = new QTableWidgetItem();
              QTableWidgetItem *time = new QTableWidgetItem();
              QTableWidgetItem *num = new QTableWidgetItem();
              QTableWidgetItem *status = new QTableWidgetItem();
              name->setText(getEventName(i));
              time->setText(getEventPlace(i,QString("").toLatin1().data()));
              num->setText(QString::number(getCandidateNum(i,QString("").toLatin1().data()),10));

              table_event->setItem(row_count, 0,name);
              table_event->setItem(row_count, 1,time);
              table_event->setItem(row_count, 2,num);

              if(getCandidateID(name->text().toLatin1().data(),usrname.toLatin1().data()) != -1){
                  status->setText("YES");
              }else{
                  status->setText("");
              }

              table_event->setItem(row_count, 3,status);
        }
    }

    if(index==1 && readComplete_ == 1){
        table_candidate=ui->table_Candidate;
        table_candidate->setRowCount(0);
        eventNum=getEventNum();
        for(i=0;i<eventNum;i++){
            candidateNum=getCandidateNum(i,QString("").toLatin1().data());
            for(j=0;j<candidateNum;j++){
                int row_count = table_candidate->rowCount(); //获取表单行数
                table_candidate->insertRow(row_count); //插入新行
                QTableWidgetItem *ename = new QTableWidgetItem();
                QTableWidgetItem *cname = new QTableWidgetItem();
                QTableWidgetItem *college = new QTableWidgetItem();
                QTableWidgetItem *score = new QTableWidgetItem();
                QTableWidgetItem *cid = new QTableWidgetItem();
                ename->setText(getEventName(i));
                cname->setText(getCandidateName(i,j));
                college->setText(getCandidateCollege(i,j));
                score->setText(getCandidateScore(i,j));
                cid->setText(QString::number(j,10));
                table_candidate->setItem(row_count, 0, ename);
                table_candidate->setItem(row_count, 1, cname);
                table_candidate->setItem(row_count, 2,college);
                table_candidate->setItem(row_count, 3,score);
                table_candidate->setItem(row_count, 4,cid);
            }
        }
    }

    if(index==2){
        addChart();
    }
    refreshComplete_=1;

}

void AtheletMain::on_table_Event_cellDoubleClicked(int row, int column)
{
    QTableWidgetItem *item;
    QString eventName;
    if(column==3){
        item=ui->table_Event->item(row,3);
        if(item->text()==""){
            eventName=ui->table_Event->item(row,0)->text();
            if(addCandidate(eventName,usrname,QString("Hong Yi").toLatin1().data(),QString("").toLatin1().data())==-1){
                myHelper::ShowMessageBoxError("参加比赛失败，不能重复参加比赛！");
            }else{
                myHelper::ShowMessageBoxInfo(usrname+"成功参加"+eventName+"比赛");
                QTableWidgetItem *status = new QTableWidgetItem();
                QTableWidgetItem *num = new QTableWidgetItem();
                status->setText("YES");
                num->setText(QString::number(ui->table_Event->item(row,2)->text().toInt()+1));
                ui->table_Event->setItem(row,3,status);
                ui->table_Event->setItem(row,2,num);
            }
        }else{
            eventName=ui->table_Event->item(row,0)->text();
            deletCandidate(eventName.toLatin1().data(),usrname.toLatin1().data());
            myHelper::ShowMessageBoxInfo(usrname+"已经退出"+eventName+"比赛");
            QTableWidgetItem *status = new QTableWidgetItem();
            QTableWidgetItem *num = new QTableWidgetItem();
            status->setText("");
            num->setText(QString::number(ui->table_Event->item(row,2)->text().toInt()-1));
            ui->table_Event->setItem(row,3,status);
            ui->table_Event->setItem(row,2,num);
        }
    }
}

void AtheletMain::on_comboBox_currentIndexChanged(const QString &arg1)
{
    int index,eventID,candidateNum,i,j,eventNum;
    QTableWidget *table_candidate;
    index=ui->tabWidget->currentIndex();
    refreshComplete_=0;
    if(index==1 && readComplete_ == 1){
        table_candidate=ui->table_Candidate;
        table_candidate->setRowCount(0);
        if(arg1=="All"){ //ALL 列出全部项目人员
            eventNum=getEventNum();
            table_candidate=ui->table_Candidate;
            for(i=0;i<eventNum;i++){
                candidateNum=getCandidateNum(i,QString("").toLatin1().data());
                for(j=0;j<candidateNum;j++){
                    int row_count = table_candidate->rowCount(); //获取表单行数
                    table_candidate->insertRow(row_count); //插入新行
                    QTableWidgetItem *ename = new QTableWidgetItem();
                    QTableWidgetItem *cname = new QTableWidgetItem();
                    QTableWidgetItem *college = new QTableWidgetItem();
                    QTableWidgetItem *score = new QTableWidgetItem();
                    QTableWidgetItem *cid = new QTableWidgetItem();
                    ename->setText(getEventName(i));
                    cname->setText(getCandidateName(i,j));
                    college->setText(getCandidateCollege(i,j));
                    score->setText(getCandidateScore(i,j));
                    cid->setText(QString::number(j,10));
                    table_candidate->setItem(row_count, 0, ename);
                    table_candidate->setItem(row_count, 1, cname);
                    table_candidate->setItem(row_count, 2,college);
                    table_candidate->setItem(row_count, 3,score);
                    table_candidate->setItem(row_count, 4,cid);
                }
            }
        }else{ //列出指定项目人员
            eventID=getEventID(arg1.toLatin1().data());
            candidateNum=getCandidateNum(eventID,QString("").toLatin1().data());
            for(i=0;i<candidateNum;i++){
                int row_count = table_candidate->rowCount(); //获取表单行数
                table_candidate->insertRow(row_count); //插入新行
                QTableWidgetItem *ename = new QTableWidgetItem();
                QTableWidgetItem *cname = new QTableWidgetItem();
                QTableWidgetItem *college = new QTableWidgetItem();
                QTableWidgetItem *score = new QTableWidgetItem();
                QTableWidgetItem *cid = new QTableWidgetItem();
                ename->setText(getEventName(eventID));
                cname->setText(getCandidateName(eventID,i));
                college->setText(getCandidateCollege(eventID,i));
                score->setText(getCandidateScore(eventID,i));
                cid->setText(QString::number(i,10));
                table_candidate->setItem(row_count, 0, ename);
                table_candidate->setItem(row_count, 1, cname);
                table_candidate->setItem(row_count, 2,college);
                table_candidate->setItem(row_count, 3,score);
                table_candidate->setItem(row_count, 4,cid);
            }
        }
        refreshComplete_=1;
    }

}

int AtheletMain::checkConflict(int eventID1,int eventID2){

    //判断项目地点冲突
    int candidateNum1,candidateNum2,i,j;
    char* candidateName1;



    if(strcmp(getEventPlace(eventID1,QString("").toLatin1().data()),getEventPlace(eventID2,QString("").toLatin1().data())) == 0){
        return 1;
    }

    //判断人员冲突
    candidateNum1=getCandidateNum(eventID1,QString("").toLatin1().data());
    candidateNum2=getCandidateNum(eventID2,QString("").toLatin1().data());
    for(i=0;i<candidateNum1;i++){
        candidateName1=getCandidateName(eventID1,i);
        for(j=0;j<candidateNum2;j++){
            if(strcmp(candidateName1,getCandidateName(eventID2,j)) == 0){
                return 1;
            }
        }
    }

return 0;

}



int AtheletMain::checkList(QList<QString> list,QString arg){
    int i,size;
    size=list.size();
    for(i=0;i<size;i++){
        if(checkConflict(getEventID(list.at(i).toLatin1().data()),getEventID(arg.toLatin1().data()))==1){
           return 1;
        }
    }
    return 0;
}


void AtheletMain::addChart(){
    QTreeWidget *tree;
    int eventNum,i,branches_count;
    QString eventName;
    QList<QString> list,eventList;

    tree=ui->treeWidget;
    tree->clear();

    eventNum=getEventNum();
    for(int i=0;i<eventNum;i++){
        list.append(getEventName(i)); // 末尾添加
    }

    QTreeWidgetItem *event_tree = new QTreeWidgetItem(tree,QStringList(QString("项目场次（每一场次时长两小时）-->  上午8:00-10:00-12:00  下午2:00-4:00-6:00")));

    branches_count=1;
    while (list.size()>0) {
        eventNum=list.size();

        QTreeWidgetItem *event_tree_total = new QTreeWidgetItem(event_tree,QStringList(QString("第"+QString::number(branches_count,10)+"场次")));

        event_tree->addChild(event_tree_total);

        for(i=0;i<eventNum;i++)
        {
          eventName=list.at(i);
          if(checkList(eventList,eventName)==0){
              QTreeWidgetItem *event_tree_object = new QTreeWidgetItem(event_tree_total,QStringList(eventName));
              event_tree_total->addChild(event_tree_object);
              eventList.append(eventName);
          }

        }
        //clear existing projects
        for(i=0;i<eventList.size();i++){
           list.removeOne(eventList.at(i));
        }
        eventList.clear();
        branches_count++;
    }




}




void AtheletMain::on_btnMenu_clicked()
{
    if(colorstyle_==0){
        myHelper::SetStyle("black");//黑色风格
        colorstyle_++;
    }else if(colorstyle_==1){
        myHelper::SetStyle("blue");//蓝色风格
        colorstyle_++;
    }else if(colorstyle_==2){
        myHelper::SetStyle("gray");//灰色风格
        colorstyle_++;
    }else {
        myHelper::SetStyle("navy");//天蓝色风格
        colorstyle_=0;
    }
}
