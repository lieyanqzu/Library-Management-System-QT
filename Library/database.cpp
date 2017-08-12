#include <QtSql/QtSql>
#include <QMessageBox>
#include <QString>

#include "database.h"
#include "admin.h"

bool Database::connect(const QString &dbName) {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);
    if (!db.open()) {
        QMessageBox::critical(0, QObject::tr("Database Error"),
                              db.lastError().text());
        return false;
    }
    return true;
}

bool Database::prepare(const QString &dbName) {
    if (!Database::connect(dbName)) {
        return false;
    }

    createAdminDB();
    createReaderDB();
    createBookDB();
    createBookLentDB();

    return true;
}

bool Database::createAdminDB() {
    QSqlQuery query;
    bool success = query.exec("CREATE TABLE IF NOT EXISTS Admin ("
                              "license INTEGER PRIMARY KEY,"
                              "password VARCHAR)");
    if (success == false) {
        QMessageBox::critical(0, QObject::tr("Database Error"),
                              query.lastError().text());
        return false;
    }

    Admin admin(10000, "10000");
    admin.addToDatabase();

    return true;
}

bool Database::createReaderDB() {

    QSqlQuery query;
    bool success = query.exec("CREATE TABLE IF NOT EXISTS Reader ("
                              "number INTEGER PRIMARY KEY,"
                              "name VARCHAR,"
                              "classes VARCHAR)");
    if (success == false) {
        QMessageBox::critical(0, QObject::tr("Database Error"),
                              query.lastError().text());
        return false;
    }
    return true;
}

bool Database::createBookDB() {
    QSqlQuery query;
    bool success = query.exec("CREATE TABLE IF NOT EXISTS Book ("
                              "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                              "isbn VARVHAR,"
                              "title VARCHAR,"
                              "author VARCHAR,"
                              "press VARCHAR,"
                              "price REAL,"
                              "status INTEGER)");
    if (success == false) {
        QMessageBox::critical(0, QObject::tr("Database Error"),
                              query.lastError().text());
        return false;
    }
    return true;
}

bool Database::createBookLentDB() {
    QSqlQuery query;
    bool success = query.exec("CREATE TABLE IF NOT EXISTS BookLent ("
                              "id INTEGER PRIMARY KEY,"
                              "number INTEGER,"
                              "borrow_time VARCHAR,"
                              "expire_time VARCHAR)");
    if (success == false) {
        QMessageBox::critical(0, QObject::tr("Database Error"),
                              query.lastError().text());
        return false;
    }
    return true;
}
