#ifndef DAYE_H
#define DAYE_H
#include <string.h>
#include <QString>
#include <QList>
int getStringBetweenAandB(const char* a,const char* A,const char* B,char *find);
#define UA " Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.26 Safari/537.36 Core/1.63.5221.400 QQBrowser/10.0.1125.400"
#define TMPPICDIR "tmpFile"

struct categrayNumString
{
    int categrayNum;
    QString categrayString;
};


struct picLinkInfo
{
    QString picFileName;
    QString picLink;
    bool uploadStatus;
};

QString picLinkInfo2WpContent(QList<struct picLinkInfo> inPicLinkInfoList);

#endif // DAYE_H
