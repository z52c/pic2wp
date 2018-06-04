#ifndef WPCONTENT_H
#define WPCONTENT_H

#include <QObject>
#include "downloader.h"
#include "daye.h"

/*默认作者1号
 *网址格式http://.....com
 *特色图使用外链，插件
 *
 *
 *
 */

#define FLAGFORINIT 0
#define FLAGFORCATEGORY 1
#define FLAGFORPOSTID 2


class wpcontent : public QObject
{
    Q_OBJECT
public:
    explicit wpcontent(QObject *parent = nullptr);
    ~wpcontent();
    void init(QString site,QString userName,QString pwd);

    void setPostData();
    void publish(QString inTitle,QStringList inPicLinkList,QString inCategory);

    QMap<QString,int> getCategoryList();

signals:
    void finished();
    void status(QString);

    void wpLoginFinished();
    void wpLoginFailed(QString);
    void categoryGot();
private slots:
    void redirected(QString u);
    void downloaderFinished();

private:
    QString siteUrl;
    QString user;
    QString passWord;
    QString post_category;   //文章分类编号
    QString title;
    QString content;
    QString fifu;   //特色图链接

    downloader *dGet;
    downloader *dPost;

    downloader *d;
    int flag;
    QMap<QString,int> categoryList;
    QStringList picLinkList;


    QString _wpnonce;
    QString post_ID;
    QString meta_box_order_nonce;
    QString closedpostboxesnonce;
    QString samplepermalinknonce;
    QString _ajax_nonceaddcategory;
    QString _ajax_nonceaddmeta;


    void login();
    void getCategoryRequest();
    void picLinkList2Content();
    void getPostId();

};

#endif // WPCONTENT_H
