#include "tuchuangloginform.h"
#include "ui_tuchuangloginform.h"
#include <QtDebug>

tuchuangLoginForm::tuchuangLoginForm(tuchuang *inT,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tuchuangLoginForm)
{
    t=inT;
    ui->setupUi(this);
}

tuchuangLoginForm::~tuchuangLoginForm()
{
    delete ui;
}

void tuchuangLoginForm::on_pushButtonLogin_clicked()
{
    if(ui->comboBox->currentText()==(QString("s6tu.com")))
    {
        t->s6tuLogin(ui->lineEditUser->text(),ui->lineEditpwd->text());
        this->close();
    }
    if(ui->comboBox->currentText()==(QString("pic303.com")))
    {
        t->pic303Login(ui->lineEditUser->text(),ui->lineEditpwd->text());
        this->close();
    }
    if(ui->comboBox->currentText()==(QString("s2tu.com")))
    {
        t->s2tuLogin(ui->lineEditUser->text(),ui->lineEditpwd->text());
        this->close();
    }
}

void tuchuangLoginForm::on_pushButtonCancle_clicked()
{
    this->close();
}
