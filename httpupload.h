#ifndef HTTPUPLOAD_H
#define HTTPUPLOAD_H

#include <QObject>
#include "downloader.h"

class httpUpload : public downloader
{
    Q_OBJECT
public:
    httpUpload();
    void doUpload();
    void appendText(QString name,QString content);
    void appendImg(QString cdh,QString fileName);
    void multiPartClear();
    void closeImageFile();
private:
    QHttpMultiPart *multiPart;
    QFile *file;
    QString a;
};

#endif // HTTPUPLOAD_H
