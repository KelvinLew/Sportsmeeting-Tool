#include "adminmain.h"
#include "ui_adminmain.h"
#include "iconhelper.h"
#include "myhelper.h"
#include "mjson.h"

AdminMain::AdminMain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminMain)
{
    ui->setupUi(this);
    myHelper::FormInCenter(this);
    this->InitStyle();
}

int readComplete,refreshComplete,addMode,colorstyle;

AdminMain::~AdminMain()
{
    delete ui;
}

void AdminMain::InitStyle()
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);

    location = this->geometry();
    max = false;
    mousePressed = false;
    ui->lab_Title->installEventFilter(this);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Min, QChar(0xf068), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu, QChar(0xf0c9), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf015), 12);
    this->readJSONData();
    colorstyle=0;
}

bool AdminMain::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void AdminMain::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void AdminMain::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void AdminMain::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}


void AdminMain::on_btnMenu_Max_clicked()
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

void AdminMain::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}

void AdminMain::on_btnMenu_Close_clicked()
{
    qApp->exit();
}

void AdminMain::readJSONData()
{
    QTableWidget *table_event;
    QComboBox *combobox_event;
    int eventNum,i;
    readJSON();
    readComplete=0;
    addMode=0;
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
          QTableWidgetItem *id = new QTableWidgetItem();
          name->setText(getEventName(i));
          time->setText(getEventPlace(i,QString("").toLatin1().data()));
          num->setText(QString::number(getCandidateNum(i,QString("").toLatin1().data()),10));
          id->setText(QString::number(i,10));
          table_event->setItem(row_count, 0, name);
          table_event->setItem(row_count, 1, time);
          table_event->setItem(row_count, 2,num);
          table_event->setItem(row_count, 3,id);
          combobox_event->addItem(name->text());
    }

    readComplete=1;
    refreshComplete=1;
}



void AdminMain::on_pushButton_Add_clicked()
{
    int index;
    QTableWidget *table_event,*table_candidate;
    index=ui->tabWidget->currentIndex();
    if(index==0){
        table_event=ui->table_Event;
        table_event->insertRow(table_event->rowCount());
        table_event->selectRow(table_event->rowCount()+1);
    }
    if(index==1){
        table_candidate=ui->table_Candidate;
        table_candidate->insertRow(table_candidate->rowCount());
        table_candidate->selectRow(table_candidate->rowCount()+1);
        addMode=1;
    }

}

void AdminMain::on_comboBox_currentIndexChanged(const QString &arg1)
{
    int index,eventID,candidateNum,i,j,eventNum;
    QTableWidget *table_candidate;
    index=ui->tabWidget->currentIndex();
    refreshComplete=0;
    if(index==1 && readComplete==1){
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
        refreshComplete=1;
    }

}

void AdminMain::on_pushButton_Delet_clicked()
{
    int index;
    QTableWidget *table_event,*table_candidate;
    index=ui->tabWidget->currentIndex();
    if(index==0){
        table_event=ui->table_Event;
        deletEvent(table_event->currentRow(),QString("").toLatin1().data());
        table_event->removeRow(table_event->currentRow());
    }
    if(index==1){
        table_candidate=ui->table_Candidate;
        table_candidate->removeRow(table_candidate->currentRow());
        deletCandidate(table_candidate->item(table_candidate->currentRow(),0)->text().toLatin1().data(),table_candidate->item(table_candidate->currentRow(),1)->text().toLatin1().data());
    }
}

void AdminMain::on_table_Event_itemChanged(QTableWidgetItem *item)
{
    int row,column;
    row=item->row();
    column=item->column();

    if(row+1>getEventNum() && column==0 && refreshComplete==1){ //Add Mode
        if(addEvent(item->text().toLatin1().data(),QString("").toLatin1().data()) != -1){
            //QTableWidgetItem *time = new QTableWidgetItem();
            QTableWidgetItem *num = new QTableWidgetItem();
            QTableWidgetItem *id = new QTableWidgetItem();
            //time->setText("");
            num->setText(QString::number(getCandidateNum(row,QString("").toLatin1().data()),10));
            id->setText(QString::number(row,10));
            //ui->table_Event->setItem(row, 1, time);
            ui->table_Event->setItem(row, 2,num);
            ui->table_Event->setItem(row, 3,id);

            ui->comboBox->clear();
            int eventNum,i;
            eventNum=getEventNum();
            ui->comboBox->addItem("All");
            for(i=0;i<eventNum;i++){
                ui->comboBox->addItem(getEventName(i));
            }
        }else{
            myHelper::ShowMessageBoxError("增加项目失败！有重复项目！");
        }

    }else if(readComplete==1 && column==0 && refreshComplete==1){
        if(changeEventName(getEventName(row),item->text().toLatin1().data())==-1){
            myHelper::ShowMessageBoxError("修改项目名称失败！有重复项目！");
        }
    }else if(readComplete==1 && column==1 && refreshComplete==1){
        changeEventPlace(getEventName(row),item->text().toLatin1().data());
        }

}


void AdminMain::on_tabWidget_currentChanged(int index)
{
    QTableWidget *table_event,*table_candidate;
    int eventNum,candidateNum,i,j;

    refreshComplete=0;

    if(index==0 && readComplete==1){
        table_event=ui->table_Event;
        table_event->clearContents();
        table_event->setRowCount(0);
        eventNum=getEventNum();
        for(i=0;i<eventNum;i++){
              int row_count = table_event->rowCount(); //获取表单行数
              table_event->insertRow(row_count); //插入新行
              QTableWidgetItem *name = new QTableWidgetItem();
              QTableWidgetItem *time = new QTableWidgetItem();
              QTableWidgetItem *num = new QTableWidgetItem();
              QTableWidgetItem *id = new QTableWidgetItem();
              name->setText(getEventName(i));
              time->setText(getEventPlace(i,QString("").toLatin1().data()));
              num->setText(QString::number(getCandidateNum(i,QString("").toLatin1().data())));
              id->setText(QString::number(i));
              printf("\n%s\n",name->text().toLatin1().data());
              table_event->setItem(row_count, 0,name);
              table_event->setItem(row_count, 1,time);
              table_event->setItem(row_count, 2,num);
              table_event->setItem(row_count, 3,id);
        }
    }

    if(index==1 && readComplete==1){
        table_candidate=ui->table_Candidate;
        table_candidate->clearContents();
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
    refreshComplete=1;

    if(index==2){
        addChart();
    }


    if(index==3){
        QTreeWidget *tree;
        tree=ui->treeWidget_JSON;
        tree->clear();
        QTreeWidgetItem *event_tree = new QTreeWidgetItem(tree,QStringList(QString("项目")));
        eventNum=getEventNum();
        for(i=0;i<eventNum;i++){

            QTreeWidgetItem *event_tree_child = new QTreeWidgetItem(event_tree,QStringList(getEventName(i)));
            event_tree->addChild(event_tree_child);

            QTreeWidgetItem *event_tree_child_time = new QTreeWidgetItem(event_tree_child,QStringList(QString("place")));
            QTreeWidgetItem *event_tree_child_candidate = new QTreeWidgetItem(event_tree_child,QStringList(QString("candidate")));

            event_tree_child->addChild(event_tree_child_time);
            event_tree_child_time->addChild(new QTreeWidgetItem(event_tree_child_time,QStringList(QString(getEventPlace(i,QString("").toLatin1().data())))));
            event_tree_child->addChild(event_tree_child_candidate);

            candidateNum=getCandidateNum(i,QString("").toLatin1().data());
            for(j=0;j<candidateNum;j++){
                QTreeWidgetItem *event_tree_child_candidate_name = new QTreeWidgetItem(event_tree_child_candidate,QStringList(QString("name")));
                QTreeWidgetItem *event_tree_child_candidate_college = new QTreeWidgetItem(event_tree_child_candidate,QStringList(QString("college")));
                QTreeWidgetItem *event_tree_child_candidate_score = new QTreeWidgetItem(event_tree_child_candidate,QStringList(QString("score")));
                event_tree_child_candidate->addChild(event_tree_child_candidate_name);
                event_tree_child_candidate->addChild(event_tree_child_candidate_college);
                event_tree_child_candidate->addChild(event_tree_child_candidate_score);
                event_tree_child_candidate_name->addChild(new QTreeWidgetItem(event_tree_child_candidate_name,QStringList(QString(getCandidateName(i,j)))));
                event_tree_child_candidate_college->addChild(new QTreeWidgetItem(event_tree_child_candidate_college,QStringList(QString(getCandidateCollege(i,j)))));
                event_tree_child_candidate_score->addChild(new QTreeWidgetItem(event_tree_child_candidate_score,QStringList(QString(getCandidateScore(i,j)))));

            }

        }

    }

}


void AdminMain::on_table_Candidate_cellChanged(int row, int column)
{
    int candidateNum,retn;
    QString eventName,candidateName;
    QTableWidget *table_candidate;
    table_candidate=ui->table_Candidate;


    if(addMode==0 && readComplete==1 && refreshComplete==1 && column==2){
        eventName=table_candidate->item(row,0)->text();
        candidateName=table_candidate->item(row,1)->text();
        changeCandidateCollege(eventName.toLatin1().data(),getCandidateID(eventName.toLatin1().data(),candidateName.toLatin1().data()),table_candidate->item(row,column)->text().toLatin1().data());
    }
    if(addMode==0 && readComplete==1 && refreshComplete==1 && column==3){
        eventName=table_candidate->item(row,0)->text();
        candidateName=table_candidate->item(row,1)->text();
        changeCandidateScore(eventName.toLatin1().data(),getCandidateID(eventName.toLatin1().data(),candidateName.toLatin1().data()),table_candidate->item(row,column)->text().toLatin1().data());
    }



    if(refreshComplete==1 && addMode==1 && column==1 && readComplete==1){
        eventName=table_candidate->item(row,0)->text();
        candidateName=table_candidate->item(row,column)->text();
        retn=addCandidate(eventName.toLatin1().data(),candidateName.toLatin1().data(),QString("").toLatin1().data(),QString("").toLatin1().data());

        addMode++; //不加会有莫名奇妙的BUG

        if(retn != -1){
            candidateNum=getCandidateNum(-1,eventName.toLatin1().data());
            QTableWidgetItem *name = new QTableWidgetItem();
            //QTableWidgetItem *college = new QTableWidgetItem();
            //QTableWidgetItem *score = new QTableWidgetItem();
            QTableWidgetItem *cid = new QTableWidgetItem();
            name->setText(table_candidate->item(row,column)->text());
            //college->setText("");
            //score->setText("");
            cid->setText(QString::number(candidateNum-1,10));
            ui->table_Candidate->setItem(row, 1,name);
            //ui->table_Candidate->setItem(row, 2,college);
            //ui->table_Candidate->setItem(row, 3,score);
            ui->table_Candidate->setItem(row, 4,cid);
            addMode=0;
        }else {
            myHelper::ShowMessageBoxError("增加参赛者失败！有重复参赛者！");
        }
    }
}

void AdminMain::on_pushButton_Search_clicked()
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

int AdminMain::checkConflict(int eventID1,int eventID2){

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


int AdminMain::checkList(QList<QString> list,QString arg){
    int i,size;
    size=list.size();
    for(i=0;i<size;i++){
        if(checkConflict(getEventID(list.at(i).toLatin1().data()),getEventID(arg.toLatin1().data()))==1){
           return 1;
        }
    }
    return 0;
}

void AdminMain::addChart(){
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


void AdminMain::on_pushButton_clicked()
{
    if(ui->tabWidget->currentIndex()==0){
        ui->table_Event->sortItems(2,Qt::DescendingOrder);
    }
    if(ui->tabWidget->currentIndex()==1){
        ui->table_Candidate->sortItems(3,Qt::DescendingOrder);
    }
}

void AdminMain::on_btnMenu_clicked()
{
    if(colorstyle==0){
        myHelper::SetStyle("black");//黑色风格
        colorstyle++;
    }else if(colorstyle==1){
        myHelper::SetStyle("blue");//蓝色风格
        colorstyle++;
    }else if(colorstyle==2){
        myHelper::SetStyle("gray");//灰色风格
        colorstyle++;
    }else {
        myHelper::SetStyle("navy");//天蓝色风格
        colorstyle=0;
    }
}
