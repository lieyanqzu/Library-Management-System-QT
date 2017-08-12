#ifndef READER_H
#define READER_H

#include <QString>

#include "datamodel.h"

class Reader : public DataModel
{
public:
    Reader(int number, QString name, QString classes);
    Reader(int number);
    bool addToDatabase();
    bool existDatabase();
    bool modifyDatabase();
    bool deleteFromDB();
    static Reader& searchFromDB(int _number);
    static Reader& databaseNext(QSqlQuery& query);

    int getNumber() { return number_; }
    QString getName() { return name_; }
    QString getClasses() { return classes_; }
private:
    int number_;
    QString name_;
    QString classes_;
};

#endif // READER_H
