#include <QCryptographicHash>
#include <QMessageBox>

#include "admin.h"
#include "mainwindow.h"

Admin::Admin(int license, QString password)
    : license_(license) {
    this->password_ = passwdToMd5(password);
}

Admin::Admin(int license) {
    this->license_ = license;
}

QString Admin::passwdToMd5(QString & password) {
    QByteArray b;
    b = QCryptographicHash::hash (password.toLatin1(), QCryptographicHash::Md5);
    password.clear();
    password.append(b.toHex());
    return password;
}

bool Admin::addToDatabase() {
    QSqlQuery query;
    if (existDatabase() == false) {
        if (!query.exec(QString("INSERT INTO Admin (license, password) VALUES (%1, '%2')").arg(license_).arg(password_))) {
            QMessageBox::critical(0, QObject::tr("Database Error"),
                                  query.lastError().text());
            return false;
        }
        return true;
    }
    return false;
}

bool Admin::existDatabase() {
    QSqlQuery query;
    query.exec(QString("SELECT * FROM Admin WHERE license = %1").arg(license_));
    if (query.next() == true) {
        return true;
    }
    return false;
}

bool Admin::loginValidate() {
    QSqlQuery query;
    query.exec(QString("SELECT password FROM Admin WHERE license = %1").arg(license_));
    while (query.next()) {
        QString pwd = query.value(0).toString();
        if (pwd == password_) {
            return true;
        } else {
            return false;
        }
    }
    return false;
}

bool Admin::modifyDatabase() {
    QSqlQuery query;
    if (existDatabase() == true) {
        if (!query.exec(QString("UPDATE Admin SET password = '%1' WHERE license = %2").arg(password_).arg(license_))) {
            QMessageBox::critical(0, QObject::tr("Database Error"),
                                  query.lastError().text());
            return false;
        }
        return true;
    }
    return false;
}

bool Admin::deleteFromDB() {
    QSqlQuery query;
    if (existDatabase() == true) {
        if (!query.exec(QString("DELETE FROM Admin WHERE license = %1").arg(license_))) {
            QMessageBox::critical(0, QObject::tr("Database Error"),
                                  query.lastError().text());
            return false;
        }
        return true;
    }
    return false;
}
