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
    setWindowTitle(QStringLiteral("ͼ�����ϵͳ"));
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
        QMessageBox::warning(this,QStringLiteral("��¼ʧ��"),
                             QStringLiteral("�˺Ż����벻��ȷ,���������룡"),QMessageBox::Ok);
        ui->pwdLineEdit->clear();
        ui->licLineEdit->clear();
        ui->licLineEdit->setFocus();
    }
}

void LoginDialog::on_quitBtn_clicked() {
    QDialog::reject();
}
