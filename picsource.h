#ifndef PICSOURCE_H
#define PICSOURCE_H

#include <QObject>
#include "t66y.h"
#include "meitulu.h"

class picSource : public QObject
{
    Q_OBJECT
public:
    explicit picSource(QObject *parent = nullptr);
    void doJobLink(QString inUrl);
    void doJobFile(QString inPicDir,QString inTitle);
signals:
    void finished(int,QString,QStringList,QString);  //0,failed  1,picLink 2,file     QString title,referer
private slots:
    void linkFinished(bool inIsSucceed,QString inTitle,QStringList inResultList,QString inReferer);
private:
    t66y *caoliu;
    meitulu *mtl;
};

#endif // PICSOURCE_H
