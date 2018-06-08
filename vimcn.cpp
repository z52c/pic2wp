#include "vimcn.h"

vimcn::vimcn(QObject *parent) : QObject(parent)
{
    hu=new httpUpload();
    connect(hu,SIGNAL(downloadError(QString)),this,SLOT(httpError(QString)));
    connect(hu,SIGNAL(redirected(QString)),this,SLOT(httpError(QString)));
    connect(hu,SIGNAL(finished()),this,SLOT(httpFinished()));

    hu->init("https://img.vim-cn.com/");
}

void vimcn::upload(QString fileName)
{
    QString cdh=QString("form-data; name=\"image\"; filename=\"")+fileName+QString("\"");
    hu->multiPartClear();
    hu->appendImg(cdh,fileName);
    picLink="";
    hu->doUpload();
}

void vimcn::httpFinished()
{
    picLink=hu->readData();
    emit uploadSucess(picLink);
}

void vimcn::httpError(QString inErrorString)
{
    emit uploadError(inErrorString);
}
