#ifndef VIMCN_H
#define VIMCN_H

#include <QObject>
#include "httpupload.h"

class vimcn : public QObject
{
    Q_OBJECT
public:
    explicit vimcn(QObject *parent = nullptr);
    void upload(QString fileName);
signals:
    void uploadSucess(QString );
    void uploadError(QString);
private slots:
    void httpFinished();
    void httpError(QString inErrorString);

private:
    httpUpload *hu;
    QString picLink;
};

#endif // VIMCN_H
