#include "meitulu.h"
#include "daye.h"
#include<QtDebug>

meitulu::meitulu(QObject *parent) : QObject(parent)
{
    d=new downloader();
    connect(d,SIGNAL(finished()),this,SLOT(htmlDownloaded()));
    connect(d,SIGNAL(downloadError(QString)),this,SLOT(htmlFailed(QString)));
    connect(d,SIGNAL(redirected(QString)),this,SLOT(htmlFailed(QString)));
}

void meitulu::doJob(QString inUrl)
{
    picLinkList.clear();
    mid=inUrl.split("/").last();
    mid=mid.split(".").first();
    url=inUrl;
    d->init(inUrl,"meitulu.tmp");
    d->setReferer("www.meitulu.com");
    d->doGet();
}

void meitulu::htmlDownloaded()
{
    QString line;
    QFile file("meitulu.tmp");
    char tmp[200];
    if(file.open(QIODevice::ReadOnly))
    {
        while(!file.atEnd())
        {
            line=file.readLine();
            if(line.contains("</span><h1>"))
            {
                getStringBetweenAandB(line.toStdString().c_str(),"</span><h1>","</h1>",tmp);
                title=QString(tmp);
                qDebug()<<title;
                break;
            }
        }
        while(!file.atEnd())
        {
            line=file.readLine();
            if(line.contains("图片数量"))
            {
                getStringBetweenAandB(line.toStdString().c_str(),"图片数量： "," 张",tmp);
                stringPicNum=QString(tmp);
                qDebug()<<stringPicNum;
                iPicNum=stringPicNum.toInt();
                break;
            }
        }
        for(int i=1;i<=iPicNum;i++)
        {
            picLinkList.append(QString("https://mtl.ttsqgs.com/images/img/")+mid+QString("/")+QString::number(i)+QString(".jpg"));
        }
        emit finished(true,title,picLinkList,url);
    }
    else{
        emit finished(false,title,picLinkList,url);
    }

}


void meitulu::htmlFailed(QString inErrorString)
{
    emit finished(false,title,picLinkList,url);
}
