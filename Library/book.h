#ifndef BOOK_H
#define BOOK_H

#include <QString>

#include "datamodel.h"

class Book : public DataModel
{
public:
    enum BookStatus { IDLE = 0, LENT = 1 };
    Book(QString isbn, QString title,
         QString author, QString press, double price, BookStatus status, int id = 0);
    Book(int id);
    bool addToDatabase();
    bool existDatabase();
    bool modifyDatabase();
    bool deleteFromDB();
    static Book& searchFromDB(int _id);
    static Book& databaseNext(QSqlQuery& query);

    int getId() { return id_; }
    QString getIsbn() { return isbn_; }
    QString getTitle() { return title_; }
    QString getAuthor() { return author_; }
    QString getPress() { return press_; }
    double getPrice() { return price_; }
    BookStatus getStatus() { return status_; }
    void setStatus(BookStatus status) { status_ = status; }
private:
    int id_;
    QString isbn_;
    QString title_;
    QString author_;
    QString press_;
    double price_;
    BookStatus status_;
};

#endif // BOOK_H
