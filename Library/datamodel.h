#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QtSql/QtSql>

class DataModel
{
public:
    virtual bool addToDatabase() = 0;
    virtual bool existDatabase() = 0;
    virtual bool modifyDatabase() = 0;
    virtual bool deleteFromDB() = 0;
};

#endif // DATAMODEL_H
