#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    _steal=true;
    p=new proxyForm();

    ui->setupUi(this);
    ui->pushButtonUpload->setEnabled(false);

    wp=new wpcontent();
    connect(wp,SIGNAL(wpLoginFinished()),this,SLOT(wpLogined()));
    connect(wp,SIGNAL(wpLoginFailed(QString)),this,SLOT(wpLoginFailed()));
    connect(wp,SIGNAL(categoryGot()),this,SLOT(wpCategoryGot()));
    connect(wp,SIGNAL(finished()),this,SLOT(wpPublished()));
    connect(wp,SIGNAL(status(QString)),this,SLOT(status(QString)));

    tc=new tuchuang();
    connect(tc,SIGNAL(logined(int)),this,SLOT(tuchaungLogined(int)));
    connect(tc,SIGNAL(status(QString)),this,SLOT(status(QString)));
    connect(tc,SIGNAL(finished(QStringList)),this,SLOT(tuchuangFinished(QStringList)));

    tuchuangLogin=new tuchuangLoginForm(tc);

    _picSource=new picSource();
    connect(_picSource,SIGNAL(finished(int,QString,QStringList,QString)),this,SLOT(picSourceFinished(int,QString,QStringList,QString)));

    _picDownloader=new picDownloader();
    connect(_picDownloader,SIGNAL(finished(int,QStringList)),this,SLOT(picDownloaded(int,QStringList)));

    QDir tmpDir(TMPPICDIR);
    if(!tmpDir.exists())
    {
        tmpDir.mkdir(TMPPICDIR);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::wpPublished()
{
    ui->textEditOutPut->clear();
    ui->lineEditUrl->clear();
    ui->lineEditPicPaths->clear();
    ui->pushButtonUpload->setEnabled(true);
}


//图床上传结束
void MainWindow::tuchuangFinished(QStringList inPicLinkList)
{
    wp->publish(title,inPicLinkList,category);
}

//图片下载结束
void MainWindow::picDownloaded(int inNum, QStringList inPicFileList)
{
    if(inNum==0)
    {
        status("没有下载到图片...");
        wpPublished();return;
    }
    status("上传图片到图床...");
    tc->upload(inPicFileList,tuchuangKind);
}

//picsource获取链接中图片信息结束
void MainWindow::picSourceFinished(int inFlag, QString inTitle, QStringList inResultList,QString inReferer)
{
    title=inTitle;
    if(inFlag==0)
    {
        status(ui->lineEditUrl->text()+QString(" failed  :")+inResultList.at(0));
        wpPublished();
        return;
    }
    if(inFlag==1)
    {
        status("图片链接已经获取");
        if(_steal)
        {
            wp->publish(inTitle,inResultList,category);return;
        }else{
            status("下载图片文件中...");
            _picDownloader->doJob(inResultList,inReferer);return;
        }
    }
}

//图床登录成功，comobox添加对应项
void MainWindow::tuchaungLogined(int inLoginedKind)
{
    if(inLoginedKind==1)
    {
        ui->comboBoxTuChuang->addItem(QString("s6tu"));
    }
    else if(inLoginedKind==2)
    {
        ui->comboBoxTuChuang->addItem(QString("pic303"));
    }
    else if(inLoginedKind==3)
    {
        ui->comboBoxTuChuang->addItem(QString("s2tu"));
    }
}


//输出状态信息
void MainWindow::status(QString inString)
{
    ui->textEditOutPut->append(inString);
}


//wordpress登录成功
void MainWindow::wpLogined()
{

    ui->lineEditWPPWD->clear();
    ui->lineEditWPUser->clear();
    ui->textEditOutPut->append("wordpress登录成功，准备获取目录分类...");
}

//wordpress登录失败
void MainWindow::wpLoginFailed()
{
    ui->textEditOutPut->setText("wordpress登录失败");
    ui->pushButtonLogin->setEnabled(true);
}

//获取wordpress目录分类
void MainWindow::wpCategoryGot()
{
    categoryList=wp->getCategoryList();
    QMap<QString, int>::const_iterator i=categoryList.constBegin();
    category=QString::number(i.value());
    while(i!=categoryList.constEnd())
    {
        ui->comboBoxCategory->addItem(i.key());
        i++;
    }
    ui->pushButtonUpload->setEnabled(true);
    ui->textEditOutPut->append("已获取目录分类.");

}

void MainWindow::on_pushButtonLogin_clicked()
{
    if(ui->lineEditWPSite->text().isEmpty() || ui->lineEditWPUser->text().isEmpty() || ui->lineEditWPPWD->text().isEmpty())
    {
        ui->textEditOutPut->setText("请填入网址、用户名、密码");
    }else{
        ui->pushButtonLogin->setEnabled(false);
        wp->init(ui->lineEditWPSite->text(),ui->lineEditWPUser->text(),ui->lineEditWPPWD->text());
    }
}

void MainWindow::on_action_triggered()
{
    p->show();
}

void MainWindow::on_actionTuchuangLogin_triggered()
{
    tuchuangLogin->show();
}

void MainWindow::on_checkBoxUpload_stateChanged(int arg1)
{
    _steal=!_steal;
}

//设置当前上传分类
void MainWindow::on_comboBoxCategory_currentIndexChanged(const QString &arg1)
{
    QMap<QString, int>::const_iterator i=categoryList.constBegin();
    while(i!=categoryList.constEnd())
    {
        if(i.key()==arg1)
        {
            category=QString::number(i.value());return;
        }
        i++;
    }
}

//设置当前图床
void MainWindow::on_comboBoxTuChuang_currentIndexChanged(const QString &arg1)
{
    if(arg1==QString("sm.ms"))
        tuchuangKind=0;
    if(arg1==QString("s6tu"))
        tuchuangKind=1;
    if(arg1==QString("pic303"))
        tuchuangKind=2;
    if(arg1==QString("s2tu"))
        tuchuangKind=3;
}

void MainWindow::on_pushButtonUpload_clicked()
{
    ui->pushButtonUpload->setEnabled(false);
    _picSource->doJobLink(ui->lineEditUrl->text());
}
