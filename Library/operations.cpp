#include <QDialog>
#include <QLabel>
#include <QtGui>
#include <QLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QDate>

#include "operations.h"
#include "setting.h"
#include "admin.h"
#include "mainwindow.h"
#include "book.h"
#include "reader.h"
#include "booklent.h"

void Operations::changeSetting() {
    QDialog dialog;
    dialog.setFixedSize(400, 250);
    dialog.setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    dialog.setWindowTitle(QStringLiteral("修改设置"));

    QGridLayout layout;
    dialog.setLayout(&layout);

    QLabel borrowNumberLabel;
    borrowNumberLabel.setText(QStringLiteral("每位同学可借书籍本数："));
    QLabel renewTimesLabel;
    renewTimesLabel.setText(QStringLiteral("每位同学可续借次数："));
    QLabel finePerDayLabel;
    finePerDayLabel.setText(QStringLiteral("过期书籍每过期一天罚金："));

    QSpinBox borrowNumberBox;
    borrowNumberBox.setValue(Setting::getBorrowNumber());
    borrowNumberBox.setRange(0, 50);
    QSpinBox renewTimesBox;
    renewTimesBox.setValue(Setting::getRenewTimes());
    renewTimesBox.setRange(0, 20);
    QDoubleSpinBox finePerDayBox;
    finePerDayBox.setValue(Setting::getFinePerDay());
    finePerDayBox.setRange(0.0, 5.0);
    finePerDayBox.setDecimals(1);
    finePerDayBox.setSingleStep(0.1);

    QPushButton okBtn(QStringLiteral("确认"));
    QPushButton cancelBtn(QStringLiteral("取消"));
    QHBoxLayout buttomHBLayout;
    buttomHBLayout.addWidget(&okBtn);
    buttomHBLayout.addWidget(&cancelBtn);

    connect(&okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    layout.addWidget(&borrowNumberLabel, 0, 0);
    layout.addWidget(&borrowNumberBox, 0, 1);
    layout.addWidget(&renewTimesLabel, 1, 0);
    layout.addWidget(&renewTimesBox, 1, 1);
    layout.addWidget(&finePerDayLabel, 2, 0);
    layout.addWidget(&finePerDayBox, 2, 1);
    layout.addLayout(&buttomHBLayout, 3, 1);

    if (dialog.exec() == QDialog::Accepted) {
        Setting::setBorrowNumber(borrowNumberBox.value());
        Setting::setRenewTimes(renewTimesBox.value());
        Setting::setFinePerDay(finePerDayBox.value());
        Setting::saveSetting();
    }
}

void Operations::changePassword() {
    bool ok;
    QString password = QInputDialog::getText(nullptr, QStringLiteral("修改密码"),
                                             QStringLiteral("请输入新密码："), QLineEdit::Normal, "", &ok);

    if (ok) {
        Admin admin(MainWindow::loginLicense, password);
        admin.modifyDatabase();
    }
}

void Operations::addAdmin() {
    QDialog dialog;
    dialog.setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    dialog.setWindowTitle(QStringLiteral("新增管理员"));

    QGridLayout layout;
    dialog.setLayout(&layout);

    QLabel licenseLabel;
    licenseLabel.setText(QStringLiteral("账号："));
    QLabel passwordLabel;
    passwordLabel.setText(QStringLiteral("密码："));

    QLineEdit licenseEdit;
    QLineEdit passwordEdit;

    QPushButton okBtn(QStringLiteral("确认"));
    QPushButton cancelBtn(QStringLiteral("取消"));
    QHBoxLayout buttomHBLayout;
    buttomHBLayout.addWidget(&okBtn);
    buttomHBLayout.addWidget(&cancelBtn);
    connect(&okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    layout.addWidget(&licenseLabel, 0, 0);
    layout.addWidget(&licenseEdit, 0, 1);
    layout.addWidget(&passwordLabel, 1, 0);
    layout.addWidget(&passwordEdit, 1, 1);
    layout.addLayout(&buttomHBLayout, 2, 1);


    if (dialog.exec() == QDialog::Accepted) {
        Admin admin(licenseEdit.text().toInt(), passwordEdit.text());
        admin.addToDatabase();
    }
}

void Operations::deleteAdmin() {
    bool ok;
    QString license = QInputDialog::getText(nullptr, QStringLiteral("删除管理员"),
                                        QStringLiteral("请输入要删除的账号："), QLineEdit::Normal, "", &ok);
    if (ok) {
        if (license.isEmpty()) {
            return;
        }
        Admin admin(license.toInt());
        if (admin.deleteFromDB() == false) {
            QMessageBox::critical(0, QStringLiteral("删除失败"),
                                  QStringLiteral("无法成功删除，请确认输入信息！"));
        }
    }
}

void Operations::addBook() {
    QDialog dialog;
    dialog.setFixedSize(400, 250);
    dialog.setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    dialog.setWindowTitle(QStringLiteral("新增书籍"));

    QGridLayout layout;
    dialog.setLayout(&layout);

    QLabel isbnLabel;
    isbnLabel.setText(QStringLiteral("ISBN："));
    QLabel titleLabel;
    titleLabel.setText(QStringLiteral("标题："));
    QLabel authorLabel;
    authorLabel.setText(QStringLiteral("作者："));
    QLabel pressLabel;
    pressLabel.setText(QStringLiteral("出版社："));
    QLabel priceLabel;
    priceLabel.setText(QStringLiteral("价格："));

    QLineEdit isbnEdit;
    QLineEdit titleEdit;
    QLineEdit authorEdit;
    QLineEdit pressEdit;
    QDoubleSpinBox priceBox;
    priceBox.setRange(0.0, 10000.0);
    priceBox.setDecimals(2);
    priceBox.setSingleStep(1.0);

    QPushButton okBtn(QStringLiteral("确认"));
    QPushButton cancelBtn(QStringLiteral("取消"));
    QHBoxLayout buttomHBLayout;
    buttomHBLayout.addWidget(&okBtn);
    buttomHBLayout.addWidget(&cancelBtn);

    connect(&okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    layout.addWidget(&isbnLabel, 0, 0);
    layout.addWidget(&isbnEdit, 0, 1);
    layout.addWidget(&titleLabel, 1, 0);
    layout.addWidget(&titleEdit, 1, 1);
    layout.addWidget(&authorLabel, 2, 0);
    layout.addWidget(&authorEdit, 2, 1);
    layout.addWidget(&pressLabel, 3, 0);
    layout.addWidget(&pressEdit, 3, 1);
    layout.addWidget(&priceLabel, 4, 0);
    layout.addWidget(&priceBox, 4, 1);
    layout.addLayout(&buttomHBLayout, 5, 1);

    if (dialog.exec() == QDialog::Accepted) {
        Book book(isbnEdit.text(), titleEdit.text(), authorEdit.text(),
                  pressEdit.text(), priceBox.value(), Book::IDLE);
        book.addToDatabase();
    }
}

void Operations::addReader() {
    QDialog dialog;
    dialog.setFixedSize(400, 250);
    dialog.setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    dialog.setWindowTitle(QStringLiteral("新增读者"));

    QGridLayout layout;
    dialog.setLayout(&layout);

    QLabel numberLabel;
    numberLabel.setText(QStringLiteral("学号："));
    QLabel nameLabel;
    nameLabel.setText(QStringLiteral("姓名："));
    QLabel classesLabel;
    classesLabel.setText(QStringLiteral("班级："));

    QLineEdit numberEdit;
    QLineEdit nameEdit;
    QLineEdit classesEdit;

    QPushButton okBtn(QStringLiteral("确认"));
    QPushButton cancelBtn(QStringLiteral("取消"));
    QHBoxLayout buttomHBLayout;
    buttomHBLayout.addWidget(&okBtn);
    buttomHBLayout.addWidget(&cancelBtn);

    connect(&okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    layout.addWidget(&numberLabel, 0, 0);
    layout.addWidget(&numberEdit, 0, 1);
    layout.addWidget(&nameLabel, 1, 0);
    layout.addWidget(&nameEdit, 1, 1);
    layout.addWidget(&classesLabel, 2, 0);
    layout.addWidget(&classesEdit, 2, 1);
    layout.addLayout(&buttomHBLayout, 3, 1);

    if (dialog.exec() == QDialog::Accepted) {
        Reader reader(numberEdit.text().toInt(), nameEdit.text(), classesEdit.text());
        reader.addToDatabase();
    }
}

void Operations::deleteBook() {
    bool ok;
    QString id = QInputDialog::getText(nullptr, QStringLiteral("删除书籍"),
                                        QStringLiteral("请输入要删除的书籍编号："), QLineEdit::Normal, "", &ok);
    if (ok) {
        if (id.isEmpty()) {
            return;
        }
        Book book(id.toInt());
        if (book.deleteFromDB() == false) {
            QMessageBox::critical(0, QStringLiteral("删除失败"),
                                  QStringLiteral("无法成功删除，请确认输入信息！"));
        }
    }
}

void Operations::deleteReader() {
    bool ok;
    QString number = QInputDialog::getText(nullptr, QStringLiteral("删除读者"),
                                        QStringLiteral("请输入要删除的读者学号："), QLineEdit::Normal, "", &ok);
    if (ok) {
        if (number.isEmpty()) {
            return;
        }
        Reader reader(number.toInt());
        if (reader.deleteFromDB() == false) {
            QMessageBox::critical(0, QStringLiteral("删除失败"),
                                  QStringLiteral("无法成功删除，请确认输入信息！"));
        }
    }
}

void Operations::modifyBook() {
    bool ok;
    QString id = QInputDialog::getText(nullptr, QStringLiteral("修改书籍信息"),
                                        QStringLiteral("请输入要修改的书籍编号："), QLineEdit::Normal, "", &ok);
    if (ok) {
        if (id.isEmpty()) {
            return;
        }
        modifyBookDia(id.toInt());
    }
}

void Operations::modifyReader() {
    bool ok;
    QString number = QInputDialog::getText(nullptr, QStringLiteral("修改读者信息"),
                                        QStringLiteral("请输入要修改的读者学号："), QLineEdit::Normal, "", &ok);
    if (ok) {
        if (number.isEmpty()) {
            return;
        }
        modifyReaderDia(number.toInt());
    }
}

void Operations::modifyBookDia(int _id) {
    Book* book;
    if ((book = &Book::searchFromDB(_id)) != nullptr) {
        QDialog dialog;
        dialog.setFixedSize(400, 250);
        dialog.setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
        dialog.setWindowTitle(QStringLiteral("修改书籍信息"));

        QGridLayout layout;
        dialog.setLayout(&layout);

        QLabel isbnLabel;
        isbnLabel.setText(QStringLiteral("ISBN："));
        QLabel titleLabel;
        titleLabel.setText(QStringLiteral("标题："));
        QLabel authorLabel;
        authorLabel.setText(QStringLiteral("作者："));
        QLabel pressLabel;
        pressLabel.setText(QStringLiteral("出版社："));
        QLabel priceLabel;
        priceLabel.setText(QStringLiteral("价格："));

        QLineEdit isbnEdit;
        isbnEdit.setText(book->getIsbn());
        QLineEdit titleEdit;
        titleEdit.setText(book->getTitle());
        QLineEdit authorEdit;
        authorEdit.setText(book->getAuthor());
        QLineEdit pressEdit;
        pressEdit.setText(book->getPress());
        QDoubleSpinBox priceBox;
        priceBox.setValue(book->getPrice());
        priceBox.setRange(0.0, 10000.0);
        priceBox.setDecimals(2);
        priceBox.setSingleStep(1.0);

        QPushButton okBtn(QStringLiteral("确认"));
        QPushButton cancelBtn(QStringLiteral("取消"));
        QHBoxLayout buttomHBLayout;
        buttomHBLayout.addWidget(&okBtn);
        buttomHBLayout.addWidget(&cancelBtn);

        connect(&okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
        connect(&cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

        layout.addWidget(&isbnLabel, 0, 0);
        layout.addWidget(&isbnEdit, 0, 1);
        layout.addWidget(&titleLabel, 1, 0);
        layout.addWidget(&titleEdit, 1, 1);
        layout.addWidget(&authorLabel, 2, 0);
        layout.addWidget(&authorEdit, 2, 1);
        layout.addWidget(&pressLabel, 3, 0);
        layout.addWidget(&pressEdit, 3, 1);
        layout.addWidget(&priceLabel, 4, 0);
        layout.addWidget(&priceBox, 4, 1);
        layout.addLayout(&buttomHBLayout, 5, 1);

        if (dialog.exec() == QDialog::Accepted) {
            Book booktemp(isbnEdit.text(), titleEdit.text(), authorEdit.text(),
                      pressEdit.text(), priceBox.value(), Book::IDLE, _id);
            booktemp.modifyDatabase();
        }
        delete book;
    }
}

void Operations::modifyReaderDia(int _number) {
    Reader* reader;
    if ((reader = &Reader::searchFromDB(_number)) == nullptr) {
        return;
    }
    QDialog dialog;
    dialog.setFixedSize(400, 250);
    dialog.setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    dialog.setWindowTitle(QStringLiteral("修改读者信息"));

    QGridLayout layout;
    dialog.setLayout(&layout);

    QLabel numberLabel;
    numberLabel.setText(QStringLiteral("学号："));
    QLabel nameLabel;
    nameLabel.setText(QStringLiteral("姓名："));
    QLabel classesLabel;
    classesLabel.setText(QStringLiteral("班级："));

    QLineEdit numberEdit;
    numberEdit.setText(QString("%1").arg(reader->getNumber()));
    numberEdit.setEnabled(false);
    QLineEdit nameEdit;
    nameEdit.setText(reader->getName());
    QLineEdit classesEdit;
    classesEdit.setText(reader->getClasses());

    QPushButton okBtn(QStringLiteral("确认"));
    QPushButton cancelBtn(QStringLiteral("取消"));
    QHBoxLayout buttomHBLayout;
    buttomHBLayout.addWidget(&okBtn);
    buttomHBLayout.addWidget(&cancelBtn);

    connect(&okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    layout.addWidget(&numberLabel, 0, 0);
    layout.addWidget(&numberEdit, 0, 1);
    layout.addWidget(&nameLabel, 1, 0);
    layout.addWidget(&nameEdit, 1, 1);
    layout.addWidget(&classesLabel, 2, 0);
    layout.addWidget(&classesEdit, 2, 1);
    layout.addLayout(&buttomHBLayout, 3, 1);

    if (dialog.exec() == QDialog::Accepted) {
        Reader readertemp(_number, nameEdit.text(), classesEdit.text());
        readertemp.modifyDatabase();
    }
    delete reader;
}

void Operations::lendBook() {
    QDialog dialog;
    dialog.setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    dialog.setWindowTitle(QStringLiteral("借出书籍"));

    QGridLayout layout;
    dialog.setLayout(&layout);

    QLabel idLabel;
    idLabel.setText(QStringLiteral("被借书籍编号："));
    QLabel numberLabel;
    numberLabel.setText(QStringLiteral("借书读者学号："));

    QLineEdit idEdit;
    QLineEdit numberEdit;

    QPushButton okBtn(QStringLiteral("确认"));
    QPushButton cancelBtn(QStringLiteral("取消"));
    QHBoxLayout buttomHBLayout;
    buttomHBLayout.addWidget(&okBtn);
    buttomHBLayout.addWidget(&cancelBtn);
    connect(&okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    layout.addWidget(&idLabel, 0, 0);
    layout.addWidget(&idEdit, 0, 1);
    layout.addWidget(&numberLabel, 1, 0);
    layout.addWidget(&numberEdit, 1, 1);
    layout.addLayout(&buttomHBLayout, 2, 1);


    if (dialog.exec() == QDialog::Accepted) {
        lendBookDia(idEdit.text().toInt(), numberEdit.text().toUInt());
    }
}

void Operations::lendBookDia(int _id, int _number) {
    Book* book = &Book::searchFromDB(_id);
    Reader* reader = &Reader::searchFromDB(_number);

    if (book == nullptr || reader == nullptr) {
        QMessageBox::critical(0, QObject::tr("Error"), QStringLiteral("编号或学号输入错误！"));
        return;
    }

    if (BookLent::getBorrowTimes(_number) >= Setting::getBorrowNumber()) {
        QMessageBox::critical(0, QObject::tr("Error"), QStringLiteral("超过最大借阅数量！"));
        delete book;
        delete reader;
        return;
    }

    if (book->getStatus() == Book::LENT) {
        QMessageBox::critical(0, QObject::tr("Error"), QStringLiteral("本书已被借出！"));
        delete book;
        delete reader;
        return;
    }

    QDialog dialog;
    dialog.setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    dialog.setWindowTitle(QStringLiteral("确认借书书籍"));

    QGridLayout layout;
    dialog.setLayout(&layout);

    QVBoxLayout bookLayout;
    QLabel idLabel;
    idLabel.setText(QString(QStringLiteral("编号：%1")).arg(book->getId()));
    QLabel isbnLabel;
    isbnLabel.setText(QString(QStringLiteral("ISBN：%1")).arg(book->getIsbn()));
    QLabel titleLabel;
    titleLabel.setText(QString(QStringLiteral("标题：%1")).arg(book->getTitle()));
    QLabel authorLabel;
    authorLabel.setText(QString(QStringLiteral("作者：%1")).arg(book->getAuthor()));
    QLabel pressLabel;
    pressLabel.setText(QString(QStringLiteral("出版社：%1")).arg(book->getPress()));
    QLabel priceLabel;
    priceLabel.setText(QString(QStringLiteral("价格：%1")).arg(book->getPrice()));
    bookLayout.addWidget(&idLabel);
    bookLayout.addWidget(&isbnLabel);
    bookLayout.addWidget(&titleLabel);
    bookLayout.addWidget(&authorLabel);
    bookLayout.addWidget(&pressLabel);
    bookLayout.addWidget(&priceLabel);

    QLabel textLabel;
    textLabel.setText(QStringLiteral("   借给->   "));

    QVBoxLayout readerLayout;
    QLabel numberLabel;
    numberLabel.setText(QString(QStringLiteral("学号：%1")).arg(reader->getNumber()));
    QLabel nameLabel;
    nameLabel.setText(QString(QStringLiteral("姓名：%1")).arg(reader->getName()));
    QLabel classesLabel;
    classesLabel.setText(QString(QStringLiteral("班级：%1")).arg(reader->getClasses()));
    readerLayout.addWidget(&numberLabel);
    readerLayout.addWidget(&nameLabel);
    readerLayout.addWidget(&classesLabel);

    QPushButton okBtn(QStringLiteral("确认"));
    QPushButton cancelBtn(QStringLiteral("取消"));
    QHBoxLayout buttomHBLayout;
    buttomHBLayout.addWidget(&okBtn);
    buttomHBLayout.addWidget(&cancelBtn);
    connect(&okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    layout.addLayout(&bookLayout, 0, 0);
    layout.addWidget(&textLabel, 0, 1);
    layout.addLayout(&readerLayout, 0, 2);
    layout.addLayout(&buttomHBLayout, 1, 2);


    if (dialog.exec() == QDialog::Accepted) {
        BookLent bookLent(_id, _number, QDate::currentDate(), QDate::currentDate().addDays(30));
        book->setStatus(Book::LENT);
        bookLent.addToDatabase();
        book->modifyDatabase();
        delete book;
        delete reader;
    }
}

void Operations::renewBook() {
    bool ok;
    int id = QInputDialog::getText(nullptr, QStringLiteral("续借书籍"),
                                        QStringLiteral("请输入要续借的书籍编号："), QLineEdit::Normal, "", &ok).toInt();
    if (ok) {
        if (id == 0) {
            return;
        }
        BookLent* bookLent = &BookLent::searchFromDB(id);
        if (bookLent == nullptr) {
            return;
        }
        int renewtimes = BookLent::getRenewTimes(id);
        if (renewtimes >= Setting::getRenewTimes() + 1) {
            QMessageBox::critical(0, QObject::tr("Error"), QStringLiteral("超过最大续借次数！"));
            return;
        }
        BookLent bookLentTemp(bookLent->getId(), bookLent->getNumber(), bookLent->getBorrowTime(),
                              bookLent->getExpireTime().addDays(30));
        bookLentTemp.modifyDatabase();
    }
}

void Operations::returnBook() {
    bool ok;
    int id = QInputDialog::getText(nullptr, QStringLiteral("归还书籍"),
                                        QStringLiteral("请输入要归还的书籍编号："), QLineEdit::Normal, "", &ok).toInt();
    if (ok) {
        if (id == 0) {
            return;
        }
        BookLent* bookLent = &BookLent::searchFromDB(id);
        if (bookLent == nullptr) {
            return;
        }
        Book* book = &Book::searchFromDB(id);
        book->setStatus(Book::IDLE);
        book->modifyDatabase();
        if (bookLent->getExpireTime() < QDate::currentDate()) {
            QMessageBox::critical(0, QStringLiteral("延期罚金"),
                                  QString(QStringLiteral("因为延期还书，需要缴纳罚金 %1 元！"))
                                  .arg(Setting::getFinePerDay()*bookLent->getExpireTime().daysTo(QDate::currentDate())));
        }
        bookLent->deleteFromDB();
        delete bookLent;
        delete book;
    }
}
