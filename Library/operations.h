#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <QObject>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>

class Operations : public QObject
{
    Q_OBJECT
public:
    static void changeSetting();
    static void changePassword();
    static void addAdmin();
    static void deleteAdmin();

    static void addBook();
    static void addReader();
    static void deleteBook();
    static void deleteReader();
    static void modifyBook();
    static void modifyReader();
    static void modifyBookDia(int _id);
    static void modifyReaderDia(int _number);

    static void lendBook();
    static void lendBookDia(int _id, int _number);
    static void renewBook();
    static void returnBook();
};

#endif // OPERATIONS_H
