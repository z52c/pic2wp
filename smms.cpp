#include "smms.h"

smms::smms(QObject *parent) : QObject(parent)
{
    hu=new httpUpload();
    connect(hu,SIGNAL(finished()),this,SLOT(httpFinished()));
    connect(hu,SIGNAL(downloadError(QString)),this,SLOT(httpError(QString)));
    hu->init("https://sm.ms/api/upload");
}

void smms::upload(QString fileName)
{
    hu->multiPartClear();
    QString cdh=QString("form-data; name=\"smfile\"; filename=\"")+QFile(fileName).fileName()+QString("\"");
    hu->appendImg(cdh,fileName);
    hu->appendText("file_id","0");
    hu->doUpload();
}

void smms::httpFinished()
{
    hu->closeImageFile();
    QString dataRead=hu->readData();
    char tmp[100];
    if(dataRead.contains("error"))
    {
        getStringBetweenAandB(dataRead.toStdString().c_str(),"msg: \"","\"",tmp);
        emit uploadError(QString(tmp));
        qDebug()<<"smms error";
        qDebug()<<tmp;
        return;
    }
    if(dataRead.contains("success"))
    {
        getStringBetweenAandB(dataRead.toStdString().c_str(),"url\":\"","\"",tmp);
        emit uploadSucess(QString(tmp));
        qDebug()<<"smms success";
        qDebug()<<tmp;
        return;
    }
}

void smms::httpError(QString inErrorMsg)
{
    emit uploadError(inErrorMsg);
}
