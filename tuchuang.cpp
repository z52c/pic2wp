#include "tuchuang.h"
#include <QtDebug>

/**********************
 * 构造函数中对所有需要登陆的图床
 * 进行登录操作，登录成功后发送相应
 * 的signal，主界面接收到相应的
 * signal后允许用户选择对应的图床类型
 * ********************/
tuchuang::tuchuang(QObject *parent) : QObject(parent)
{
    picNum=0;
    sm=new smms();
    connect(sm,SIGNAL(uploadError(QString)),this,SLOT(uploadFailed(QString)));
    connect(sm,SIGNAL(uploadSucess(QString)),this,SLOT(uploadSucess(QString)));

    s6=new s6tu();
    connect(s6,SIGNAL(uploadError(QString)),this,SLOT(uploadFailed(QString)));
    connect(s6,SIGNAL(uploadSucess(QString)),this,SLOT(uploadSucess(QString)));
    connect(s6,SIGNAL(loginFinished(bool)),this,SLOT(s6tuLogined(bool)));

    s2=new s2tu();
    connect(s2,SIGNAL(uploadError(QString)),this,SLOT(uploadFailed(QString)));
    connect(s2,SIGNAL(uploadSucess(QString)),this,SLOT(uploadSucess(QString)));
    connect(s2,SIGNAL(loginFinished(bool)),this,SLOT(s2tuLogined(bool)));

    p303=new pic303();
    connect(p303,SIGNAL(uploadError(QString)),this,SLOT(uploadFailed(QString)));
    connect(p303,SIGNAL(uploadSucess(QString)),this,SLOT(uploadSucess(QString)));
    connect(p303,SIGNAL(loginFinished(bool)),this,SLOT(pic303Logined(bool)));
}

void tuchuang::s6tuLogin(QString inUserName, QString inPWD)
{
    s6->login(inUserName,inPWD);
}

void tuchuang::s2tuLogin(QString inUserName, QString inPWD)
{
    s2->login(inUserName,inPWD);
}

void tuchuang::pic303Login(QString inUserName, QString inPWD)
{
    p303->login(inUserName,inPWD);
}

void tuchuang::upload(QStringList inPicFileList,int inKind)
{
    if(picNum==0)
    {
        picLinkList.clear();
        picFileList.clear();
        picFileList=inPicFileList;
        kind=inKind;
        picNum=picFileList.count();
    }
    switch(kind)
    {
    case 0:
        sm->upload(picFileList.at(0));
        break;
    case 1:
        s6->upload(picFileList.at(0));
        break;
    case 2:
        p303->upload(picFileList.at(0));
        break;
    case 3:
        s2->upload(picFileList.at(0));
        break;
    }
}


void tuchuang::uploadFailed(QString inErrorMsg)
{
    onePicFinished(false,inErrorMsg);
}

void tuchuang::uploadSucess(QString inPicLink)
{
    onePicFinished(true,inPicLink);
}


void tuchuang::onePicFinished(bool inStatus,QString inMsg)
{
    picFileList.removeAt(0);
    QString tmpInfo;
    if(inStatus)
    {
        picLinkList.append(inMsg);
        tmpInfo=QString("第")+QString::number(picNum - picFileList.count())+QString("/")+QString::number(picNum)+QString("张上传成功,链接：")+inMsg;
        emit status(tmpInfo);
        qDebug()<<tmpInfo;
    }else{
        tmpInfo=QString("第")+QString::number(picNum - picFileList.count())+QString("/")+QString::number(picNum)+QString("张上传失败,信息：")+inMsg;
        emit status(tmpInfo);
        qDebug()<<tmpInfo;
    }
    if(picFileList.count()>0)
    {
        upload(picFileList,kind);
    }
    else{
        picNum=0;
        emit finished(picLinkList);
    }
}

void tuchuang::s6tuLogined(bool inIsLogin)
{
    if(inIsLogin)
    {
        emit status("s6tu已经登录");
        emit logined(1);
    }
}

void tuchuang::s2tuLogined(bool inIsLogin)
{
    if(inIsLogin)
    {
        emit status("s2tu已经登录");
        emit logined(3);
    }
}

void tuchuang::pic303Logined(bool inIsLogin)
{
    if(inIsLogin)
    {
        emit status("pic303已经登录");
        emit logined(2);
    }
}
