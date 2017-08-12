#include <QMessageBox>

#include "book.h"

Book::Book(QString isbn, QString title, QString author,
           QString press, double price, BookStatus status, int id) :
    id_(id), isbn_(isbn), title_(title), author_(author), press_(press), price_(price), status_(status) {}

Book::Book(int id) : id_(id) {}

bool Book::addToDatabase() {
    QSqlQuery query;
    if (existDatabase() == false) {
        if (!query.exec(QString("INSERT INTO Book (isbn, title, author, press, price, status) "
                                  "VALUES ('%1', '%2', '%3', '%4', %5, %6)").arg(isbn_).arg(title_).arg(author_)
                          .arg(press_).arg(price_).arg(static_cast<int>(status_))) ) {
            QMessageBox::critical(0, QObject::tr("Database Error"),
                                  query.lastError().text());
            return false;
        }
        return true;
    }
    return false;
}

bool Book::existDatabase() {
    QSqlQuery query;
    query.exec(QString("SELECT * FROM Book WHERE id = %1").arg(id_));
    if (query.next() == true) {
        return true;
    }
    return false;
}

Book& Book::databaseNext(QSqlQuery& query) {
    if (query.next()) {
        int id = query.value(0).toInt();
        QString isbn = query.value(1).toString();
        QString title = query.value(2).toString();
        QString author = query.value(3).toString();
        QString press = query.value(4).toString();
        double price = query.value(5).toDouble();
        int status_num = query.value(6).toInt();
        Book::BookStatus status;
        if (status_num == 0) {
            status = Book::IDLE;
        } else {
            status = Book::LENT;
        }
        Book* book = new Book(isbn, title, author, press, price, status, id);
        return *book;
    } else {
        Book* book = nullptr;
        return *book;
    }
}

bool Book::modifyDatabase() {
    QSqlQuery query;
    if (existDatabase() == true) {
        if (!query.exec(QString("UPDATE Book SET isbn = '%1', title = '%2', author = '%3', "
                                "press = '%4', price = %5, status = %6 "
                                "WHERE id = %7").arg(isbn_).arg(title_).arg(author_)
                          .arg(press_).arg(price_).arg(static_cast<int>(status_)).arg(id_)) ) {
            QMessageBox::critical(0, QObject::tr("Database Error"),
                                  query.lastError().text());
            return false;
        }
        return true;
    }
    return false;
}

bool Book::deleteFromDB() {
    QSqlQuery query;
    if (existDatabase() == true) {
        if (!query.exec(QString("DELETE FROM Book WHERE id = %1").arg(id_))) {
            QMessageBox::critical(0, QObject::tr("Database Error"),
                                  query.lastError().text());
            return false;
        }
        return true;
    }
    return false;
}

Book& Book::searchFromDB(int _id) {
    QSqlQuery query;
    query.exec(QString("SELECT * FROM Book WHERE id = %1").arg(_id));
    if (query.next()) {
        int id = query.value(0).toInt();
        QString isbn = query.value(1).toString();
        QString title = query.value(2).toString();
        QString author = query.value(3).toString();
        QString press = query.value(4).toString();
        double price = query.value(5).toDouble();
        int status_num = query.value(6).toInt();
        Book::BookStatus status;
        if (status_num == 0) {
            status = Book::IDLE;
        } else {
            status = Book::LENT;
        }
        Book* book = new Book(isbn, title, author, press, price, status, id);
        return *book;
    } else {
        Book* book = nullptr;
        return *book;
    }
}
