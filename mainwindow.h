#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "wpcontent.h"
#include "tuchuang.h"
#include "daye.h"
#include "proxyform.h"
#include "tuchuangloginform.h"
#include "picsource.h"
#include "picdownloader.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void wpLogined();
    void status(QString inString);
    void wpLoginFailed();
    void wpCategoryGot();
    void tuchaungLogined(int);
    void wpPublished();
    void picDownloaded(int inNum,QStringList inPicFileList);
    void tuchuangFinished(QStringList inPicLinkList);

    void on_pushButtonLogin_clicked();
    void on_action_triggered();
    void on_actionTuchuangLogin_triggered();

    void picSourceFinished(int inFlag,QString inTitle,QStringList inResultList,QString inReferer);

    void on_checkBoxUpload_stateChanged(int arg1);

    void on_comboBoxCategory_currentIndexChanged(const QString &arg1);

    void on_comboBoxTuChuang_currentIndexChanged(const QString &arg1);

    void on_pushButtonUpload_clicked();

private:
    Ui::MainWindow *ui;
    tuchuang *tc;
    wpcontent *wp;
    proxyForm *p;
    tuchuangLoginForm *tuchuangLogin;
    picSource *_picSource;
    picDownloader *_picDownloader;

    QString title;
    bool _steal;
    QString category;
    int tuchuangKind=0;

    QMap<QString,int> categoryList;

};

#endif // MAINWINDOW_H
