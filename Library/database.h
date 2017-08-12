#ifndef DATABASE_H
#define DATABASE_H

#include <QString>

class Database {
public:
    static bool connect(const QString &dbName);
    static bool prepare(const QString &dbName);
private:
    static bool createAdminDB();
    static bool createReaderDB();
    static bool createBookDB();
    static bool createBookLentDB();
};

#endif // DATABASE_H
