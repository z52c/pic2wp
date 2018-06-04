#include "t66y.h"
#include "daye.h"

t66y::t66y(QObject *parent) : QObject(parent)
{
    d=new downloader();
    connect(d,SIGNAL(downloadError(QString)),this,SLOT(htmlFileDownloadFailed(QString)));
    connect(d,SIGNAL(redirected(QString)),this,SLOT(htmlFileDownloadFailed(QString)));
    connect(d,SIGNAL(finished()),this,SLOT(htmlFileDownloaded()));

}

void t66y::doJob(QString inUrl)
{
    picList.clear();
    d->init(inUrl,"t66y.tmp");
    d->doGet();
}

void t66y::htmlFileDownloaded()
{
    QString line;
    QStringList l;
    QFile file("t66y.tmp");
    char tmp[20000];
    char picLink[200];
    char* index;
    int i=0;
    int count;
    if(file.open(QIODevice::ReadOnly))
    {
       QTextStream stream(&file);
       stream.setCodec(QTextCodec::codecForLocale());
       line=stream.readAll();
       l=line.split("\r\n");
       count=l.count();
       for(i=0;i<count;i++)
       {
           line=l.at(i);
           if(line.contains("title"))
           {
               getStringBetweenAandB(line.toStdString().c_str(),"<title>","-",tmp);
               title=QString(tmp);
               qDebug()<<title;
               break;
           }
       }

       for(;i<count;i++)
       {
           line=l.at(i);
           if(line.contains("<div class=\"tpc_content do_not_catch\">"))
           {
                qDebug()<<line;
                strcpy(tmp,line.toStdString().c_str());
                index=tmp;
                while(index=strstr(index,"data-src='"))
                {
                    getStringBetweenAandB(index,"data-src='","'",picLink);
                    picList.append(QString(picLink));
                    index=index+10;
                }
                break;
           }
       }
        file.close();
        finished(true,title,picList,"");
    }
    else
    {
        picList.append(QString("t66y.tmp open failed"));
        finished(false,title,picList,"");
    }
}

void t66y::htmlFileDownloadFailed(QString inErrorString)
{
    picList.append(inErrorString);
    finished(false,title,picList,"");
}

