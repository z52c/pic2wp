#include "pic303.h"
#include "daye.h"
#include <QtDebug>

pic303::pic303(QObject *parent) : QObject(parent)
{
    hu=new httpUpload();
    connect(hu,SIGNAL(finished()),this,SLOT(httpFinished()));
    connect(hu,SIGNAL(redirected(QString)),this,SLOT(logined()));
}

void pic303::login(QString inUserName, QString inPwd)
{
    flag=0;
    isLogin=false;
    userName=inUserName;
    passWord=inPwd;
    hu->init("http://pic303.com");
    hu->setUserAgent(UA);
    hu->doGet();

}

void pic303::logined()
{
    auth_token="";
    isLogin=true;
    hu->init("http://pic303.com/json");
    hu->setReferer("http://pic303.com/daye");
    hu->setAccept("application/json");
    hu->setOrigin("http://pic303.com");
    emit loginFinished(true);
}

void pic303::httpFinished()
{
    qDebug()<<flag;
    if(flag==0)
    {
        flag=1;
        QString data=hu->readData();
        QStringList dataLineList=data.split("\r\n");
        QString line;
        char tmp[100];
        int n=dataLineList.count();
        qDebug()<<"n:"<<n;
        qDebug()<<data;
        qDebug()<<flag;
        for(int i=0;i<=n;i++)
        {

            line=dataLineList.at(i);
            qDebug()<<line;
            if(line.contains("auth_token"))
            {
                getStringBetweenAandB(line.toStdString().c_str(),"value=\"","\"",tmp);
                auth_token=tmp;
                qDebug()<<auth_token;
                break;
            }
        }
        if(auth_token==QString(""))
        {
            emit loginFinished(false);
            return;
        }
        loginPost();
        return;
    }
    if(flag==1)
    {
        if(isLogin)
        {
            char tmp[100];
            int tmprtn;
            QString line=hu->readData();
            qDebug()<<line;
            tmprtn=getStringBetweenAandB(line.toStdString().c_str(),"\"mime\":\"image\\/jpeg\",\"url\":\"","\"",tmp);
            picLink=tmp;
            hu->closeImageFile();
            if(tmprtn)
            {
                emit uploadError(line);
            }else {
                emit uploadSucess(picLink);
            }
        }else{
            emit loginFinished(false);
        }
    }
}

void pic303::loginPost()
{
    hu->init("http://pic303.com/login");
    hu->setReferer("http://pic303.com");
    hu->setPostData("auth_token",auth_token);
    hu->setPostData("login-subject",userName);
    hu->setPostData("password",passWord);
    hu->doPost();
    qDebug()<<"pic303 login...";
}

void pic303::upload(QString fileName)
{
    QString cdh=QString("form-data; name=\"source\"; filename=\"")+fileName+QString("\"");
    hu->multiPartClear();
    hu->appendImg(cdh,fileName);
    hu->appendText("type","file");
    hu->appendText("action","upload");
    hu->appendText("timestamp",QString::number(QDateTime::currentDateTime().toTime_t()));
    hu->appendText("auth_token",auth_token);
    hu->appendText("nsfw","0");
    picLink="";
    hu->doUpload();
}
