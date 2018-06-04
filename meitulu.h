#ifndef MEITULU_H
#define MEITULU_H

#include <QObject>
#include "downloader.h"

class meitulu : public QObject
{
    Q_OBJECT
public:
    explicit meitulu(QObject *parent = nullptr);
    void doJob(QString inUrl);
signals:
    finished(bool,QString,QStringList,QString);
private slots:
    void htmlDownloaded();
    void htmlFailed(QString inErrorString);
private:
    QString url;
    QString title;
    QString stringPicNum;
    int iPicNum;
    QStringList picLinkList;
    QString mid;


    downloader *d;
};

#endif // MEITULU_H
