#include <QMessageBox>

#include "booklent.h"

BookLent::BookLent(int id, int number, QDate borrow_time, QDate expire_time) :
    id_(id), number_(number), borrow_time_(borrow_time), expire_time_(expire_time) {}

int BookLent::getId() {
    return id_;
}

int BookLent::getNumber() {
    return number_;
}

QDate BookLent::getBorrowTime() {
    return borrow_time_;
}

QDate BookLent::getExpireTime() {
    return expire_time_;
}

bool BookLent::addToDatabase() {
    QSqlQuery query;
    if (existDatabase() == false) {
        if (!query.exec(QString("INSERT INTO BookLent (id, number, borrow_time, expire_time) "
                                  "VALUES (%1, %2, '%3', '%4')").arg(id_).arg(number_)
                          .arg(borrow_time_.toString()).arg(expire_time_.toString())) ) {
            QMessageBox::critical(0, QObject::tr("Database Error"),
                                  query.lastError().text());
            return false;
        }
        return true;
    }
    return false;
}

bool BookLent::existDatabase() {
    QSqlQuery query;
    query.exec(QString("SELECT * FROM BookLent WHERE id = %1").arg(id_));
    if (query.next() == true) {
        return true;
    }
    return false;
}

BookLent& BookLent::databaseNext(QSqlQuery& query) {
    if (query.next()) {
        int id = query.value(0).toInt();
        int number = query.value(1).toInt();
        QDate borrow_time = QDate::fromString(query.value(2).toString());
        QDate expire_time = QDate::fromString(query.value(3).toString());
        BookLent* bookLent = new BookLent(id, number, borrow_time, expire_time);
        return *bookLent;
    } else {
        BookLent* bookLent = nullptr;
        return *bookLent;
    }
}

bool BookLent::modifyDatabase() {
    QSqlQuery query;
    if (existDatabase() == true) {
        if (!query.exec(QString("UPDATE BookLent SET number = %1, borrow_time = '%2', expire_time = '%3'"
                                "WHERE id = %4")
                        .arg(number_).arg(borrow_time_.toString()).arg(expire_time_.toString()).arg(id_)) ) {
            QMessageBox::critical(0, QObject::tr("Database Error"),
                                  query.lastError().text());
            return false;
        }
        return true;
    }
    return false;
}

bool BookLent::deleteFromDB() {
    QSqlQuery query;
    if (existDatabase() == true) {
        if (!query.exec(QString("DELETE FROM BookLent WHERE id = %1").arg(id_))) {
            QMessageBox::critical(0, QObject::tr("Database Error"),
                                  query.lastError().text());
            return false;
        }
        return true;
    }
    return false;
}

BookLent& BookLent::searchFromDB(int _id) {
    QSqlQuery query;
    query.exec(QString("SELECT * FROM BookLent WHERE id = %1").arg(_id));
    if (query.next()) {
        int id = query.value(0).toInt();
        int number = query.value(1).toInt();
        QDate borrow_time = QDate::fromString(query.value(2).toString());
        QDate expire_time = QDate::fromString(query.value(3).toString());
        BookLent* bookLent = new BookLent(id, number, borrow_time, expire_time);
        return *bookLent;
    } else {
        BookLent* bookLent = nullptr;
        return *bookLent;
    }
}

int BookLent::getBorrowTimes(int _number) {
    QSqlQuery query;
    query.exec(QString("SELECT COUNT(*) FROM BookLent WHERE number = %1").arg(_number));
    query.next();
    int result = query.value(0).toInt();
    return result;
}

int BookLent::getRenewTimes(int _id) {
    BookLent* bookLent = &searchFromDB(_id);
    int days = bookLent->getBorrowTime().daysTo(bookLent->getExpireTime());
    delete bookLent;
    return days / 30;
}
