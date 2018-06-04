#ifndef S6TU_H
#define S6TU_H

#include <QObject>
#include "httpupload.h"

class s6tu : public QObject
{
    Q_OBJECT
public:
    explicit s6tu(QObject *parent = nullptr);
    void login(QString inUserName, QString inPwd);
    void upload(QString fileName);
signals:
    void uploadSucess(QString );
    void uploadError(QString);
    void loginFinished(bool);

private slots:
    void httpFinished();
    void logined();
    void httpError(QString inErrorString);
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
    QString picFileName;
};

#endif // S6TU_H
