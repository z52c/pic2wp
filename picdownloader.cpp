#include "picdownloader.h"
#include <QString>

picDownloader::picDownloader(QObject *parent) : QObject(parent)
{
    d=new downloader();
    connect(d,SIGNAL(finished()),this,SLOT(onePicDownloaded()));
    connect(d,SIGNAL(downloadError(QString)),this,SLOT(downloadError()));
    connect(d,SIGNAL(redirected(QString)),this,SLOT(redirected(QString)));
}

void picDownloader::doJob(QStringList inPicLinkList,QString inReferer)
{
    redirectFlag=false;
    picFileList.clear();
    picLinkList.clear();
    num=0;
    picLinkList=inPicLinkList;
    if(inReferer!="")
        d->setReferer(inReferer);
    d->setUserAgent(UA);
    if(picLinkList.count()==0)
    {
        emit finished(0,picFileList);
        return;
    }else
    {
        QString tmp=picLinkList.at(0);
        currentFileName=QString(TMPPICDIR)+QString("/")+QString::number(num)+tmp.split("/").last();
        d->init(tmp,currentFileName);
        d->doGet();
    }
}

//部分图链接有重定向到其他图床的情况
void picDownloader::redirected(QString inRedirectUrl)
{
    if(redirectFlag)
    {
        downloadError();
    }
    else
    {
        redirectFlag=true;
        d->init(inRedirectUrl,currentFileName);
        d->doGet();
    }

}

void picDownloader::onePicDownloaded()
{
    redirectFlag=false;
    num++;

    picFileList.append(currentFileName);
    picLinkList.removeAt(0);
    if(picLinkList.count()>0)
    {
        QString tmp=picLinkList.at(0);
        currentFileName=QString(TMPPICDIR)+QString("/")+QString::number(num)+tmp.split("/").last();
        d->init(tmp,currentFileName);
        d->doGet();
    }
    else{
        emit finished(num,picFileList);
    }
}

void picDownloader::downloadError()
{
    picLinkList.removeAt(0);
    if(picLinkList.count()>0)
    {
        QString tmp=picLinkList.at(0);
        d->init(tmp,QString(TMPPICDIR)+tmp.split("/").last());
        d->doGet();
    }
    else{
        emit finished(num,picFileList);
    }
}
