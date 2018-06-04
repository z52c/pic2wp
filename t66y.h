#ifndef T66Y_H
#define T66Y_H

#include <QObject>
#include "downloader.h"

class t66y : public QObject
{
    Q_OBJECT
public:
    explicit t66y(QObject *parent = nullptr);
    void doJob(QString inUrl);
signals:
    void finished(bool,QString,QStringList,QString);
private slots:
    void htmlFileDownloadFailed(QString inErrorString);
    void htmlFileDownloaded();
private:
    downloader *d;
    QStringList picList;
    QString title;
};

#endif // T66Y_H
