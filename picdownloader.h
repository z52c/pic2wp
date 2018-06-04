#ifndef PICDOWNLOADER_H
#define PICDOWNLOADER_H

#include <QObject>
#include "downloader.h"
#include "daye.h"

class picDownloader : public QObject
{
    Q_OBJECT
public:
    explicit picDownloader(QObject *parent = nullptr);
    void doJob(QStringList inPicLinkList,QString inReferer="");
signals:
    void finished(int,QStringList); //返回下载的图片数量
private slots:
    void onePicDownloaded();
    void downloadError();
    void redirected(QString inRedirectUrl);
private:
    downloader *d;
    QStringList picLinkList;
    int num;
    QStringList picFileList;
    bool redirectFlag;
    QString currentFileName;
};

#endif // PICDOWNLOADER_H
