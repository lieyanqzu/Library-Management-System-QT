#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QStandardItemModel>
#include <QInputDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "book.h"
#include "reader.h"
#include "booklent.h"
#include "operations.h"
#include "setting.h"

int MainWindow::loginLicense = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("图书管理系统"));

    Setting::initSetting();
    initMenu();
    refreshBook();
    refreshReader();
    refreshLent();
    noSearchResult();

    statusBar() ;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshBook() {
    QSqlQuery query;
    query.exec("SELECT * FROM Book");
    QStandardItemModel* bookModel = new QStandardItemModel(ui->bookView);
    bookModel->clear();
    bookModel->setHorizontalHeaderLabels(QStringList() << QStringLiteral("编号") << QStringLiteral("ISBN")
                                     << QStringLiteral("标题") << QStringLiteral("作者")
                                     << QStringLiteral("出版社") << QStringLiteral("价格")
                                     << QStringLiteral("是否可借"));
    QFont font = ui->bookView->horizontalHeader()->font();
    font.setBold(true);
    ui->bookView->horizontalHeader()->setFont(font);

    Book* book;
    int row = 0;
    while (book = &Book::databaseNext(query)) {
        bookModel->setItem(row, 0, new QStandardItem(QString::number(book->getId())));
        bookModel->setItem(row, 1, new QStandardItem(book->getIsbn()));
        bookModel->setItem(row, 2, new QStandardItem(book->getTitle()));
        bookModel->setItem(row, 3, new QStandardItem(book->getAuthor()));
        bookModel->setItem(row, 4, new QStandardItem(book->getPress()));
        bookModel->setItem(row, 5, new QStandardItem(QString::number(book->getPrice())));
        if (book->getStatus() == Book::IDLE) {
            bookModel->setItem(row, 6, new QStandardItem(QStringLiteral("可借")));
        } else {
            bookModel->setItem(row, 6, new QStandardItem(QStringLiteral("已借出")));
        }
        row ++;
        delete book;
    }
    ui->bookView->setModel(bookModel);
    ui->bookView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->bookView->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::refreshReader() {
    QSqlQuery query;    
    query.exec("SELECT * FROM Reader");
    QStandardItemModel* readerModel = new QStandardItemModel(ui->readerView);
    readerModel->clear();
    readerModel->setHorizontalHeaderLabels(QStringList() << QStringLiteral("学号") << QStringLiteral("姓名")
                                     << QStringLiteral("班级"));
    QFont font = ui->readerView->horizontalHeader()->font();
    font.setBold(true);
    ui->readerView->horizontalHeader()->setFont(font);

    Reader* reader;
    int row = 0;
    while (reader = &Reader::databaseNext(query)) {
        readerModel->setItem(row, 0, new QStandardItem(QString::number(reader->getNumber())));
        readerModel->setItem(row, 1, new QStandardItem(reader->getName()));
        readerModel->setItem(row, 2, new QStandardItem(reader->getClasses()));
        row ++;
        delete reader;
    }
    ui->readerView->setModel(readerModel);
    ui->readerView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}


void MainWindow::refreshLent() {
    QSqlQuery query;
    query.exec("SELECT * FROM BookLent");
    QStandardItemModel* lentModel = new QStandardItemModel(ui->lentView);
    lentModel->clear();
    lentModel->setHorizontalHeaderLabels(QStringList() << QStringLiteral("书籍编号")
                                         << QStringLiteral("读者学号") << QStringLiteral("借出时间") << QStringLiteral("过期时间"));
    QFont font = ui->lentView->horizontalHeader()->font();
    font.setBold(true);
    ui->lentView->horizontalHeader()->setFont(font);

    BookLent* bookLent;
    int row = 0;
    while (bookLent = &BookLent::databaseNext(query)) {
        lentModel->setItem(row, 0, new QStandardItem(QString::number(bookLent->getId())));
        lentModel->setItem(row, 1, new QStandardItem(QString::number(bookLent->getNumber())));
        lentModel->setItem(row, 2, new QStandardItem(bookLent->getBorrowTime().toString()));
        lentModel->setItem(row, 3, new QStandardItem(bookLent->getExpireTime().toString()));
        row ++;
        delete bookLent;
    }
    ui->lentView->setModel(lentModel);
    ui->lentView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::noSearchResult() {
    QStandardItemModel* SearchModel = new QStandardItemModel(ui->searchView);
    SearchModel->clear();
    SearchModel->setHorizontalHeaderLabels(QStringList() << QStringLiteral("没有搜索结果"));
    ui->searchView->setModel(SearchModel);
    ui->searchView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::SearchResBook() {
    bool ok;
    QString title = QInputDialog::getText(nullptr, QStringLiteral("查询书籍"),
                                        QStringLiteral("请输入要查询的书籍标题："), QLineEdit::Normal, "", &ok);
    if (ok) {
        if (title.isEmpty()) {
            return;
        }
        ui->tabWidget->setCurrentIndex(3);
        QSqlQuery query;
        query.exec(QString("SELECT * FROM Book WHERE title LIKE '%%1%'").arg(title));
        QStandardItemModel* bookModel = new QStandardItemModel(ui->searchView);
        bookModel->clear();
        bookModel->setHorizontalHeaderLabels(QStringList() << QStringLiteral("编号") << QStringLiteral("ISBN")
                                         << QStringLiteral("标题") << QStringLiteral("作者")
                                         << QStringLiteral("出版社") << QStringLiteral("价格")
                                         << QStringLiteral("是否可借"));
        QFont font = ui->searchView->horizontalHeader()->font();
        font.setBold(true);
        ui->searchView->horizontalHeader()->setFont(font);

        Book* book;
        int row = 0;
        while (book = &Book::databaseNext(query)) {
            bookModel->setItem(row, 0, new QStandardItem(QString::number(book->getId())));
            bookModel->setItem(row, 1, new QStandardItem(book->getIsbn()));
            bookModel->setItem(row, 2, new QStandardItem(book->getTitle()));
            bookModel->setItem(row, 3, new QStandardItem(book->getAuthor()));
            bookModel->setItem(row, 4, new QStandardItem(book->getPress()));
            bookModel->setItem(row, 5, new QStandardItem(QString::number(book->getPrice())));
            if (book->getStatus() == Book::IDLE) {
                bookModel->setItem(row, 6, new QStandardItem(QStringLiteral("可借")));
            } else {
                bookModel->setItem(row, 6, new QStandardItem(QStringLiteral("已借出")));
            }
            row ++;
            delete book;
        }
        if (row == 0) {
            noSearchResult();
            return;
        }
        ui->searchView->setModel(bookModel);
        ui->searchView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    }
}

void MainWindow::SearchResReader() {
    bool ok;
    QString name = QInputDialog::getText(nullptr, QStringLiteral("查询读者"),
                                        QStringLiteral("请输入要查询的读者姓名："), QLineEdit::Normal, "", &ok);
    if (ok) {
        if (name.isEmpty()) {
            return;
        }
        ui->tabWidget->setCurrentIndex(3);
        QSqlQuery query;
        query.exec(QString("SELECT * FROM Reader WHERE name LIKE '%%1%'").arg(name));
        QStandardItemModel* readerModel = new QStandardItemModel(ui->searchView);
        readerModel->clear();
        readerModel->setHorizontalHeaderLabels(QStringList() << QStringLiteral("学号") << QStringLiteral("姓名")
                                         << QStringLiteral("班级"));
        QFont font = ui->searchView->horizontalHeader()->font();
        font.setBold(true);
        ui->searchView->horizontalHeader()->setFont(font);

        Reader* reader;
        int row = 0;
        while (reader = &Reader::databaseNext(query)) {
            readerModel->setItem(row, 0, new QStandardItem(QString::number(reader->getNumber())));
            readerModel->setItem(row, 1, new QStandardItem(reader->getName()));
            readerModel->setItem(row, 2, new QStandardItem(reader->getClasses()));
            row ++;
            delete reader;
        }
        if (row == 0) {
            noSearchResult();
            return;
        }
        ui->searchView->setModel(readerModel);
        ui->searchView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    }
}

void MainWindow::initMenu() {
    QAction* freshBookAction = new QAction(QStringLiteral("刷新书籍列表(&B)"), this);
    freshBookAction->setStatusTip(QStringLiteral("刷新书籍一览列表"));
    freshBookAction->setShortcut(tr("Ctrl+B"));
    connect(freshBookAction, &QAction::triggered, this, &MainWindow::refreshBook);

    QAction* freshReaderAction = new QAction(QStringLiteral("刷新读者列表(&R)"), this);
    freshReaderAction->setStatusTip(QStringLiteral("刷新读者一览列表"));
    freshReaderAction->setShortcut(tr("Ctrl+R"));
    connect(freshReaderAction, &QAction::triggered, this, &MainWindow::refreshReader);

    QAction* freshLentAction = new QAction(QStringLiteral("刷新借出列表(&L)"), this);
    freshLentAction->setStatusTip(QStringLiteral("刷新借出一览列表"));
    freshLentAction->setShortcut(tr("Ctrl+L"));
    connect(freshLentAction, &QAction::triggered, this, &MainWindow::refreshLent);

    QAction* exitProgramAction = new QAction(QStringLiteral("退出(&E)"), this);
    exitProgramAction->setStatusTip(QStringLiteral("退出程序"));
    connect(exitProgramAction, &QAction::triggered, this, &MainWindow::close);

    QMenu *file = menuBar()->addMenu(QStringLiteral("文件(&F)"));
    file->addAction(freshBookAction);
    file->addAction(freshReaderAction);
    file->addAction(freshLentAction);
    file->addSeparator();
    file->addAction(exitProgramAction);

    QAction* lendAction = new QAction(QStringLiteral("借出书籍(&L)"), this);
    lendAction->setStatusTip(QStringLiteral("借出书籍"));
    connect(lendAction, &QAction::triggered, &Operations::lendBook);

    QAction* returnAction = new QAction(QStringLiteral("归还书籍(&R)"), this);
    returnAction->setStatusTip(QStringLiteral("归还书籍入库"));
    connect(returnAction, &QAction::triggered, &Operations::returnBook);

    QAction* renewAction = new QAction(QStringLiteral("续借书籍(&N)"), this);
    renewAction->setStatusTip(QStringLiteral("续借书籍一个月"));
    connect(renewAction, &QAction::triggered, &Operations::renewBook);

    QMenu *oper = menuBar()->addMenu(QStringLiteral("操作(&O)"));
    oper->addAction(lendAction);
    oper->addAction(returnAction);
    oper->addAction(renewAction);

    QAction* findBookAction = new QAction(QStringLiteral("查找书籍(&B)"), this);
    findBookAction->setStatusTip(QStringLiteral("查找书籍信息"));
    connect(findBookAction, &QAction::triggered, this, &MainWindow::SearchResBook);
    QAction* findReaderAction = new QAction(QStringLiteral("查找读者(&R)"), this);
    findReaderAction->setStatusTip(QStringLiteral("查找读者信息"));
    connect(findReaderAction, &QAction::triggered, this, &MainWindow::SearchResReader);

    QAction* addBookAction = new QAction(QStringLiteral("增加书籍(&B)"), this);
    addBookAction->setStatusTip(QStringLiteral("增加书籍信息"));
    connect(addBookAction, &QAction::triggered, &Operations::addBook);
    QAction* addReaderAction = new QAction(QStringLiteral("增加读者(&R)"), this);
    addReaderAction->setStatusTip(QStringLiteral("增加读者信息"));
    connect(addReaderAction, &QAction::triggered, &Operations::addReader);

    QAction* deleteBookAction = new QAction(QStringLiteral("删除书籍(&B)"), this);
    deleteBookAction->setStatusTip(QStringLiteral("删除书籍信息"));
    connect(deleteBookAction, &QAction::triggered, &Operations::deleteBook);
    QAction* deleteReaderAction = new QAction(QStringLiteral("删除读者(&R)"), this);
    deleteReaderAction->setStatusTip(QStringLiteral("删除读者信息"));
    connect(deleteReaderAction, &QAction::triggered, &Operations::deleteReader);

    QAction* changeBookAction = new QAction(QStringLiteral("修改书籍(&B)"), this);
    changeBookAction->setStatusTip(QStringLiteral("修改书籍信息"));
    connect(changeBookAction, &QAction::triggered, &Operations::modifyBook);
    QAction* changeReaderAction = new QAction(QStringLiteral("修改读者(&R)"), this);
    changeReaderAction->setStatusTip(QStringLiteral("修改读者信息"));
    connect(changeReaderAction, &QAction::triggered, &Operations::modifyReader);

    QAction* optionsAction = new QAction(QStringLiteral("修改设置(&O)"), this);
    optionsAction->setStatusTip(QStringLiteral("修改借阅相关设置"));
    connect(optionsAction, &QAction::triggered, &Operations::changeSetting);

    QMenu *edit = menuBar()->addMenu(QStringLiteral("编辑(&E)"));
    QMenu *find = edit->addMenu(QStringLiteral("查找(&F)"));
    find->addAction(findBookAction);
    find->addAction(findReaderAction);
    QMenu *add = edit->addMenu(QStringLiteral("增加(&A)"));
    add->addAction(addBookAction);
    add->addAction(addReaderAction);
    QMenu *_delete = edit->addMenu(QStringLiteral("删除(&D)"));
    _delete->addAction(deleteBookAction);
    _delete->addAction(deleteReaderAction);
    QMenu *change = edit->addMenu(QStringLiteral("修改(&C)"));
    change->addAction(changeBookAction);
    change->addAction(changeReaderAction);
    edit->addSeparator();
    edit->addAction(optionsAction);

    QAction* changePwdAction = new QAction(QStringLiteral("修改密码(&C)"), this);
    changePwdAction->setStatusTip(QStringLiteral("修改当前管理员账户密码"));
    connect(changePwdAction, &QAction::triggered, &Operations::changePassword);

    QAction* addAccountAction = new QAction(QStringLiteral("增加管理员(&A)"), this);
    addAccountAction->setStatusTip(QStringLiteral("增加管理员账户"));
    connect(addAccountAction, &QAction::triggered, &Operations::addAdmin);

    QAction* deleteAction = new QAction(QStringLiteral("删除管理员(&D)"), this);
    deleteAction->setStatusTip(QStringLiteral("删除管理员账户"));
    connect(deleteAction, &QAction::triggered, &Operations::deleteAdmin);

    QMenu *account = menuBar()->addMenu(QStringLiteral("账户(&A)"));
    account->addAction(changePwdAction);
    account->addAction(addAccountAction);
    account->addAction(deleteAction);

    QAction* aboutAction = new QAction(QStringLiteral("关于(&A)"), this);
    aboutAction->setStatusTip(QStringLiteral("关于"));
    connect(aboutAction, &QAction::triggered,
            [=] {
        QMessageBox about(this);
        about.setWindowTitle(QStringLiteral("关于"));
        about.setText(QStringLiteral("图书馆管理系统 Version:1.0\n\n作者： 网142 张玉清"));
        about.exec();
    });

    QMenu *help = menuBar()->addMenu(QStringLiteral("帮助(&H)"));
    help->addAction(aboutAction);
}
