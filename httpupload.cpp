#include "httpupload.h"

httpUpload::httpUpload()
{
    a="----WebKitFormBoundaryRZcMJAhkXQinLxmW";
    multiPart=NULL;
    file=NULL;
}

void httpUpload::multiPartClear()
{
    qDebug()<<"multiPartClear";
    multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    multiPart->setBoundary(a.toLatin1());
}


void httpUpload::appendText(QString name, QString content)
{
    QHttpPart textPart;
    QString stringTmp=QString("form-data; name=\"")+name+QString("\"");
    textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(stringTmp));
    textPart.setBody(content.toStdString().c_str());
    multiPart->append(textPart);
}

void httpUpload::closeImageFile()
{
    file->close();
    file=NULL;
}

void httpUpload::appendImg(QString cdh,QString fileName)
{
    qDebug()<<cdh;
    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(cdh));
    if(QFileInfo(fileName).suffix()=="jpg" || QFileInfo(fileName).suffix()=="jpeg")
        imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));
    else if(QFileInfo(fileName).suffix()=="png")
        imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/png"));
    qDebug()<<"imagepart setheader finished";


    file = new QFile(fileName);

    qDebug()<<"7";

    file->open(QIODevice::ReadOnly);
    imagePart.setBodyDevice(file);
    file->setParent(multiPart);
    multiPart->append(imagePart);
}

void httpUpload::doUpload()
{
    request.setHeader(QNetworkRequest::ContentTypeHeader,"multipart/form-data;boundary="+a);
    request.setUrl(url);

    reply = manager.post(request, multiPart);
    multiPart->setParent(reply);

    connect(reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(downloadProgress(qint64,qint64)));
#if QT_CONFIG(ssl)
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)),
            SLOT(sslErrors(QList<QSslError>)));
#endif
}
