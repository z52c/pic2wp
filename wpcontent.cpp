#include "wpcontent.h"
#include "daye.h"
#include <QtDebug>

wpcontent::wpcontent(QObject *parent) : QObject(parent)
{
    flag=FLAGFORINIT;
    d=new downloader();
    connect(d,SIGNAL(finished()),this,SLOT(downloaderFinished()));
    connect(d,SIGNAL(redirected(QString)),this,SLOT(redirected(QString)));

    d->setUserAgent(UA);
}


wpcontent::~wpcontent()
{
    delete dGet;
    delete dPost;
}

void wpcontent::init(QString site,QString userName,QString pwd)
{
    siteUrl=site;
    user=userName;
    passWord=pwd;
    login();
}

void wpcontent::login()
{
    d->init(siteUrl+"/wp-login.php");
    d->setPostData("log",user);
    d->setPostData("pwd",passWord);
    d->setPostData("wp-submit","登录");
    d->setPostData("redirect_to",siteUrl+"/wp-admin/");
    //d->setPostData("testcookie","1");
    qDebug()<<"wp post for login";
    d->doPost();
}

void wpcontent::downloaderFinished()
{

    if(flag==FLAGFORCATEGORY)
    {
        qDebug()<<"category downloaded,get category list...";
        QFile file("cate.tmp");
        QString tmpLine;
        struct categrayNumString tmpCate;
        char tmp[100];
        if(file.open(QIODevice::ReadOnly))
        {
            while(!file.atEnd())
            {
                tmpLine=file.readLine();
                if(tmpLine.contains("level-"))
                {
                    getStringBetweenAandB(tmpLine.toStdString().c_str(),"value=\"","\"",tmp);
                    tmpCate.categrayNum=atoi(tmp);
                    getStringBetweenAandB(tmpLine.toStdString().c_str(),">","<",tmp);
                    tmpCate.categrayString=tmp;
                    tmpCate.categrayString.replace("&nbsp;","");
                    categoryList.insert(tmpCate.categrayString,tmpCate.categrayNum);
                    qDebug()<<tmpCate.categrayNum;
                    qDebug()<<tmpCate.categrayString;
                }
            }
            file.close();
        }
        qDebug()<<"emit categorygot";
        emit categoryGot();
    }
    else if(flag==FLAGFORPOSTID)
    {
        getPostId();
        d->init(siteUrl+"/wp-admin/post.php");
        d->clearPostData();
        setPostData();
        qDebug()<<QString("title.post:")+title;
        d->doPost();
    }
    else if(flag==FLAGFORINIT){
        QString tmpFailedString=d->readData();
        qDebug()<<tmpFailedString;
        emit wpLoginFailed(tmpFailedString);
    }
}


void wpcontent::redirected(QString u)
{
    if(!u.contains("post"))
    {
        emit wpLoginFinished();
        getCategoryRequest();
    }
    else
    {
        qDebug()<<"当前postid="<<post_ID;
        emit status("wordpress文章发表成功");
        emit finished();
    }

}

void wpcontent::getCategoryRequest()
{
    flag=FLAGFORCATEGORY;
    d->clearPostData();
    d->init(siteUrl+"/wp-admin/edit-tags.php?taxonomy=category","cate.tmp");
    d->doGet();
}

QMap<QString,int> wpcontent::getCategoryList()
{
    return categoryList;
}

void wpcontent::picLinkList2Content()
{
    content="";
    int c=picLinkList.count();
    QString t;
    for(int i=0;i<c;i++)
    {
         t=QString("<img src=\"")+picLinkList.at(i)+QString("\" />");
         content=content+t;
    }
}

void wpcontent::publish(QString inTitle,QStringList inPicLinkList,QString inCategory)
{
    emit status("准备上传文章到wordPress...");
    qDebug()<<inPicLinkList;
    title=inTitle;
    picLinkList.clear();
    picLinkList=inPicLinkList;
    qDebug()<<"上传wordpress图片数量:";
    qDebug()<<picLinkList.count();
    fifu=inPicLinkList.at(0);
    picLinkList2Content();
    qDebug()<<content;
    post_category=inCategory;
    d->init(siteUrl+"/wp-admin/post-new.php","get.html");
    d->setReferer(siteUrl+"/wp-admin/post-new.php");
    flag=FLAGFORPOSTID;
    d->doGet();
}


void wpcontent::getPostId()
{
    QFile file("get.html");
    QString line;
    char tmp[100];
    if(file.open(QIODevice::ReadOnly))
    {
        while(!file.atEnd())
        {
            line =file.readLine();
            if(line.contains("<form name=\"post\" action=\"post.php\" method=\"post\" id=\"post\">"))
                break;
        }
        line=file.readLine();
        getStringBetweenAandB(line.toStdString().c_str(),"_wpnonce\" value=\"","\"",tmp);
        _wpnonce=QString(tmp);
        line=file.readLine();line=file.readLine();line=file.readLine();line=file.readLine();line=file.readLine();line=file.readLine();line=file.readLine();
        getStringBetweenAandB(line.toStdString().c_str(),"post_ID' value='","'",tmp);
        post_ID=QString(tmp);
        getStringBetweenAandB(line.toStdString().c_str(),"meta-box-order-nonce\" value=\"","\"",tmp);
        meta_box_order_nonce=QString(tmp);
        getStringBetweenAandB(line.toStdString().c_str(),"closedpostboxesnonce\" value=\"","\"",tmp);
        closedpostboxesnonce=QString(tmp);
        while(!file.atEnd())
        {
            line =file.readLine();
            if(line.contains("samplepermalinknonce"))
                break;
        }
        getStringBetweenAandB(line.toStdString().c_str(),"samplepermalinknonce\" value=\"","\"",tmp);
        samplepermalinknonce=QString(tmp);
        while(!file.atEnd())
        {
            line =file.readLine();
            if(line.contains("_ajax_nonce-add-category"))
                break;
        }
        getStringBetweenAandB(line.toStdString().c_str(),"ajax_nonce-add-category\" value=\"","\"",tmp);
        _ajax_nonceaddcategory=tmp;
        while(!file.atEnd())
        {
            line =file.readLine();
            if(line.contains("_ajax_nonce-add-meta"))
                break;
        }
        getStringBetweenAandB(line.toStdString().c_str(),"ajax_nonce-add-meta\" value=\"","\"",tmp);
        _ajax_nonceaddmeta=QString(tmp);
        file.close();
    }
}

void wpcontent::setPostData()
{
    d->setPostData(QString("_wpnonce"),_wpnonce);
    d->setPostData(QString("_wp_http_referer"),QString("/wp-admin/post-new.php"));
    d->setPostData(QString("user_ID"),QString("1"));
    d->setPostData(QString("action"),QString("editpost"));
    d->setPostData(QString("post_author"),QString("1"));
    d->setPostData(QString("post_type"),QString("post"));
    d->setPostData(QString("original_post_status"),QString("auto-draft"));
    d->setPostData(QString("referredby"),QString(siteUrl+"/wp-admin/edit.php"));
    d->setPostData(QString("_wp_original_http_referer"),QString(siteUrl+"/wp-admin/edit.php"));
    d->setPostData(QString("auto_draft"),QString("1"));
    d->setPostData(QString("post_ID"),post_ID);
    d->setPostData(QString("meta-box-order-nonce"),meta_box_order_nonce);
    d->setPostData(QString("closedpostboxesnonce"),closedpostboxesnonce);
    d->setPostData(QString("post_title"),title);
    d->setPostData(QString("samplepermalinknonce"),samplepermalinknonce);
    d->setPostData(QString("content"),content);
    d->setPostData(QString("wp-preview"),QString(""));
    d->setPostData(QString("hidden_post_status"),QString("draft"));
    d->setPostData(QString("post_status"),QString("draft"));
    d->setPostData(QString("hidden_post_password"),QString(""));
    d->setPostData(QString("hidden_post_visibility"),QString("public"));
    d->setPostData(QString("post_password"),QString(""));
    d->setPostData(QString("aa"),QString(QDate::currentDate().year()));
    d->setPostData(QString("mm"),QString(QDate::currentDate().month()));
    d->setPostData(QString("jj"),QString(QDate::currentDate().day()));
    d->setPostData(QString("hh"),QString(QTime::currentTime().hour()));
    d->setPostData(QString("mn"),QString(QTime::currentTime().minute()));
    d->setPostData(QString("ss"),QString("00"));
    d->setPostData(QString("hidden_mm"),QString(QDate::currentDate().month()));
    d->setPostData(QString("cur_mm"),QString(QDate::currentDate().month()));
    d->setPostData(QString("hidden_jj"),QString(QDate::currentDate().day()));
    d->setPostData(QString("cur_jj"),QString(QDate::currentDate().day()));
    d->setPostData(QString("hidden_aa"),QString(QDate::currentDate().year()));
    d->setPostData(QString("cur_aa"),QString(QDate::currentDate().year()));
    d->setPostData(QString("hidden_hh"),QString(QTime::currentTime().hour()));
    d->setPostData(QString("cur_hh"),QString(QTime::currentTime().hour()));
    d->setPostData(QString("hidden_mn"),QString(QTime::currentTime().minute()));
    d->setPostData(QString("cur_mn"),QString(QTime::currentTime().minute()));
    d->setPostData(QString("original_publish"),QString("发布"));
    d->setPostData(QString("publish"),QString("发布"));
    d->setPostData(QString("post_format"),QString("0"));
    d->setPostData(QString("post_category[]"),post_category);
    d->setPostData(QString("newcategory"),QString("新分类目录名"));
    d->setPostData(QString("newcategory_parent"),QString("-1"));
    d->setPostData(QString("_ajax_nonce-add-category"),_ajax_nonceaddcategory);
    d->setPostData(QString("tax_input[post_tag]"),QString(""));
    d->setPostData(QString("newtag[post_tag]"),QString(""));
    d->setPostData(QString("_thumbnail_id"),QString("-1"));
    d->setPostData(QString("fifu_input_alt"),QString(""));
    d->setPostData(QString("fifu_input_url"),fifu);
    d->setPostData(QString("excerpt"),QString(""));
    d->setPostData(QString("trackback_url"),QString(""));
    d->setPostData(QString("metakeyselect"),QString("#NONE#"));
    d->setPostData(QString("metakeyinput"),QString(""));
    d->setPostData(QString("metavalue"),QString(""));
    d->setPostData(QString("_ajax_nonce-add-meta"),_ajax_nonceaddmeta);
    d->setPostData(QString("advanced_view"),QString("1"));
    d->setPostData(QString("comment_status"),QString("open"));
    d->setPostData(QString("ping_status"),QString("open"));
    d->setPostData(QString("post_name"),QString(""));
    d->setPostData(QString("post_author_override"),QString("1"));
}


