#ifndef PIC303_H
#define PIC303_H

#include <QObject>
#include "httpupload.h"

class pic303 : public QObject
{
    Q_OBJECT
public:
    explicit pic303(QObject *parent = nullptr);
    void login(QString inUserName, QString inPwd);
    void upload(QString fileName);
signals:
    void uploadSucess(QString );
    void uploadError(QString);
    void loginFinished(bool);
private slots:
    void httpFinished();
    void logined();
private:
    void loginPost();

    httpUpload *hu;
    QString picLink;
    QString cookie;
    QString auth_token;
    QString userName;
    QString passWord;
    int flag;
    bool isLogin;
};

#endif // PIC303_H
