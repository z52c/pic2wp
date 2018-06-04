#ifndef SMMS_H
#define SMMS_H

#include <QObject>
#include "httpupload.h"
#include "daye.h"



class smms : public QObject
{
    Q_OBJECT
public:
    explicit smms(QObject *parent = nullptr);
    void upload(QString fileName);
signals:
    void uploadSucess(QString );
    void uploadError(QString);
public slots:
    void httpFinished();
    void httpError(QString inErrorMsg);
private:
    httpUpload *hu;
};

#endif // SMMS_H
