#ifndef ADMIN_H
#define ADMIN_H

#include <QString>

#include "datamodel.h"

class Admin : public DataModel
{
public:
    Admin(int license, QString password);
    Admin(int license);
    bool addToDatabase();
    bool loginValidate();
    bool existDatabase();
    bool modifyDatabase();
    bool deleteFromDB();
private:
    QString passwdToMd5(QString & password);
    int license_;
    QString password_;
};

#endif // ADMIN_H
