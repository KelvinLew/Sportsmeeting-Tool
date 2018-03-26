#include "systemmain.h"
#include "ui_systemmain.h"
#include "myhelper.h"


SystemMain::SystemMain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SystemMain)
{
    ui->setupUi(this);

    myHelper::FormInCenter(this);
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
}

SystemMain::~SystemMain()
{
    delete ui;
}


void SystemMain::on_pushButton_clicked()
{
    QString account,password;



    account=ui->lineEdit->text();
    password=ui->lineEdit_2->text();

    if(account=="admin" && password=="admin"){
        this->hide();
        myHelper::ShowMessageBoxInfo("尊敬的管理员，欢迎您！");
        myHelper::ShowAdminMain();
    }else if (password=="usr"){
        this->hide();
        myHelper::ShowMessageBoxInfo("尊敬的"+account+"，欢迎您！");
        myHelper::ShowAtheletMain(account);
    }


}
