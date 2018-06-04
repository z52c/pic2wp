#include "picsource.h"

picSource::picSource(QObject *parent) : QObject(parent)
{
    caoliu= new t66y();
    connect(caoliu,SIGNAL(finished(bool,QString,QStringList,QString)),this,SLOT(linkFinished(bool,QString,QStringList,QString)));

    mtl=new meitulu();
    connect(mtl,SIGNAL(finished(bool,QString,QStringList,QString)),SLOT(linkFinished(bool,QString,QStringList,QString)));

}

void picSource::linkFinished(bool inIsSucceed, QString inTitle, QStringList inResultList,QString inReferer)
{
    if(inIsSucceed)
    {
        finished(1,inTitle,inResultList,inReferer);
    }
    else
    {
        finished(0,inTitle,inResultList,inReferer);
    }

}

void picSource::doJobLink(QString inUrl)
{
    if(inUrl.contains("t66y"))
        caoliu->doJob(inUrl);
    if(inUrl.contains("meitulu"))
        mtl->doJob(inUrl);
}
