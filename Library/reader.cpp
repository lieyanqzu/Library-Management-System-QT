#include <QMessageBox>

#include "reader.h"

Reader::Reader(int number, QString name, QString classes) :
    number_(number), name_(name), classes_(classes) {}

Reader::Reader(int number) : number_(number) {}

bool Reader::addToDatabase() {
    QSqlQuery query;
    if (existDatabase() == false) {
        if (!query.exec(QString("INSERT INTO Reader (number, name, classes) VALUES (%1, '%2', '%3')")
                          .arg(number_).arg(name_).arg(classes_)) ) {
            QMessageBox::critical(0, QObject::tr("Database Error"),
                                  query.lastError().text());
            return false;
        }
        return true;
    }
    return false;
}

bool Reader::existDatabase() {
    QSqlQuery query;
    query.exec(QString("SELECT * FROM Reader WHERE number = '%1'").arg(number_));
    if (query.next() == true) {
        return true;
    }
    return false;
}

Reader& Reader::databaseNext(QSqlQuery& query) {
    if (query.next()) {
        int number = query.value(0).toInt();
        QString name = query.value(1).toString();
        QString classes = query.value(2).toString();
        Reader* reader = new Reader(number, name, classes);
        return *reader;
    } else {
        Reader* reader = nullptr;
        return *reader;
    }
}

bool Reader::modifyDatabase() {
    QSqlQuery query;
    if (existDatabase() == true) {
        if (!query.exec(QString("UPDATE Reader SET name = '%1', classes = '%2' "
                                "WHERE number = %3")
                          .arg(name_).arg(classes_).arg(number_)) ) {
            QMessageBox::critical(0, QObject::tr("Database Error"),
                                  query.lastError().text());
            return false;
        }
        return true;
    }
    return false;
}

bool Reader::deleteFromDB() {
    QSqlQuery query;
    if (existDatabase() == true) {
        if (!query.exec(QString("DELETE FROM Reader WHERE number = %1").arg(number_))) {
            QMessageBox::critical(0, QObject::tr("Database Error"),
                                  query.lastError().text());
            return false;
        }
        return true;
    }
    return false;
}

Reader& Reader::searchFromDB(int _number) {
    QSqlQuery query;
    query.exec(QString("SELECT * FROM Reader WHERE number = %1").arg(_number));
    if (query.next()) {
        int number = query.value(0).toInt();
        QString name = query.value(1).toString();
        QString classes = query.value(2).toString();
        Reader* reader = new Reader(number, name, classes);
        return *reader;
    } else {
        Reader* reader = nullptr;
        return *reader;
    }
}
