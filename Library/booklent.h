#ifndef BOOKLENT_H
#define BOOKLENT_H

#include <QDate>

#include "datamodel.h"

class BookLent : public DataModel
{
public:
    BookLent(int id, int number, QDate borrow_time, QDate expire_time);
    bool addToDatabase();
    bool existDatabase();
    bool modifyDatabase();
    bool deleteFromDB();
    static BookLent& searchFromDB(int _id);
    static BookLent& databaseNext(QSqlQuery& query);
    static int getBorrowTimes(int _number);
    static int getRenewTimes(int _id);

    int getId();
    int getNumber();
    QDate getBorrowTime();
    QDate getExpireTime();
private:
    int id_;
    int number_;
    QDate borrow_time_;
    QDate expire_time_;
};

#endif // BOOKLENT_H
