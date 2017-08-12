#include <QMessageBox>

#include "logindialog.h"
#include "ui_logindialog.h"
#include "admin.h"
#include "mainwindow.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    setFixedSize(350, 200);
    setWindowTitle(QStringLiteral("图书管理系统"));
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    ui->licLineEdit->setFocus();
    ui->loginBtn->setDefault(true);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_loginBtn_clicked() {
    Admin admin(ui->licLineEdit->text().toInt(), ui->pwdLineEdit->text());
    if (admin.loginValidate()) {
        QDialog::accept();
        MainWindow::loginLicense = ui->licLineEdit->text().toInt();
    } else {
        QMessageBox::warning(this,QStringLiteral("登录失败"),
                             QStringLiteral("账号或密码不正确,请重新输入！"),QMessageBox::Ok);
        ui->pwdLineEdit->clear();
        ui->licLineEdit->clear();
        ui->licLineEdit->setFocus();
    }
}

void LoginDialog::on_quitBtn_clicked() {
    QDialog::reject();
}
