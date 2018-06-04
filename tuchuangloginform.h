#ifndef TUCHUANGLOGINFORM_H
#define TUCHUANGLOGINFORM_H

#include <QWidget>
#include "tuchuang.h"

namespace Ui {
class tuchuangLoginForm;
}

class tuchuangLoginForm : public QWidget
{
    Q_OBJECT

public:
    explicit tuchuangLoginForm(tuchuang *inT,QWidget *parent = 0);
    ~tuchuangLoginForm();

private slots:
    void on_pushButtonLogin_clicked();

    void on_pushButtonCancle_clicked();

private:
    Ui::tuchuangLoginForm *ui;
    tuchuang *t;
};

#endif // TUCHUANGLOGINFORM_H
