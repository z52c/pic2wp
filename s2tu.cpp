#include "s2tu.h"
#include "daye.h"

s2tu::s2tu(QObject *parent) : QObject(parent)
{
    hu=new httpUpload();
    connect(hu,SIGNAL(finished()),this,SLOT(httpFinished()));
    connect(hu,SIGNAL(redirected(QString)),this,SLOT(logined()));
    connect(hu,SIGNAL(downloadError(QString)),this,SLOT(httpError(QString)));
}

void s2tu::login(QString inUserName, QString inPwd)
{
    flag=0;
    isLogin=false;
    userName=inUserName;
    passWord=inPwd;
    hu->init("http://s2tu.com");
    hu->setUserAgent(UA);
    hu->doGet();
}

void s2tu::logined()
{
    isLogin=true;
    hu->init("http://s2tu.com/json");
    hu->setReferer("http://s2tu.com/daye");
    hu->setAccept("application/json");
    hu->setOrigin("http://s2tu.com");
    emit loginFinished(true);
}

void s2tu::httpFinished()
{
    if(flag==0)
    {
        flag=1;
        QString data=hu->readData();
        QStringList dataLineList=data.split("\r\n");
        QString line;
        char tmp[100];
        int n=dataLineList.count();
        for(int i=0;i<=n;i++)
        {
            line=dataLineList.at(i);
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

void s2tu::httpError(QString inErrorString)
{
    emit uploadError(picFileName+inErrorString);
}

void s2tu::loginPost()
{
    hu->init("http://s2tu.com/login");
    hu->setReferer("http://s2tu.com");
    hu->setPostData("auth_token",auth_token);
    hu->setPostData("login-subject",userName);
    hu->setPostData("password",passWord);
    hu->doPost();
}

void s2tu::upload(QString fileName)
{
    picFileName=fileName;
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
