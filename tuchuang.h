#ifndef TUCHUANG_H
#define TUCHUANG_H

#include <QObject>
#include "smms.h"
#include "s6tu.h"
#include "s2tu.h"
#include "pic303.h"



/*******************************
 * tuchuang类处理一串图片文件列表（QStringList）
 * 外部调用upload需要先检查列表是否为空
 * 处理完成后发送图片信息列表QList<struct picLinkInfo>
 *
 *
 *
 *
 * ************************/

class tuchuang : public QObject
{
    Q_OBJECT
public:
    explicit tuchuang(QObject *parent = nullptr);
    void upload(QStringList inPicFileList,int inKind=0);
    void s6tuLogin(QString inUserName,QString inPWD);
    void s2tuLogin(QString inUserName,QString inPWD);
    void pic303Login(QString inUserName,QString inPWD);
signals:
    void status(QString);
    void finished(QStringList);
    void logined(int);
public slots:
    void uploadSucess(QString inPicLink);
    void uploadFailed(QString inErrorMsg);
    void s6tuLogined(bool inIsLogin);
    void s2tuLogined(bool inIsLogin);
    void pic303Logined(bool inIsLogin);
private:
    smms *sm;
    s6tu *s6;
    s2tu *s2;
    pic303 *p303;
    int kind;//使用哪类图床，默认0，sm.ms,1,s6tu,2,pic303
    QStringList picFileList;//储存传入的图片文件路径列表
    int picNum;//传入的文件路径数目
    QStringList picLinkList;



    void onePicFinished(bool inStatus,QString inMsg); //一个图片上传完成后执行的操作
};

#endif // TUCHUANG_H
