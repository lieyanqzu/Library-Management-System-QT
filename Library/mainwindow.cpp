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
    setWindowTitle(QStringLiteral("ͼ�����ϵͳ"));

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
    bookModel->setHorizontalHeaderLabels(QStringList() << QStringLiteral("���") << QStringLiteral("ISBN")
                                     << QStringLiteral("����") << QStringLiteral("����")
                                     << QStringLiteral("������") << QStringLiteral("�۸�")
                                     << QStringLiteral("�Ƿ�ɽ�"));
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
            bookModel->setItem(row, 6, new QStandardItem(QStringLiteral("�ɽ�")));
        } else {
            bookModel->setItem(row, 6, new QStandardItem(QStringLiteral("�ѽ��")));
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
    readerModel->setHorizontalHeaderLabels(QStringList() << QStringLiteral("ѧ��") << QStringLiteral("����")
                                     << QStringLiteral("�༶"));
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
    lentModel->setHorizontalHeaderLabels(QStringList() << QStringLiteral("�鼮���")
                                         << QStringLiteral("����ѧ��") << QStringLiteral("���ʱ��") << QStringLiteral("����ʱ��"));
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
    SearchModel->setHorizontalHeaderLabels(QStringList() << QStringLiteral("û���������"));
    ui->searchView->setModel(SearchModel);
    ui->searchView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::SearchResBook() {
    bool ok;
    QString title = QInputDialog::getText(nullptr, QStringLiteral("��ѯ�鼮"),
                                        QStringLiteral("������Ҫ��ѯ���鼮���⣺"), QLineEdit::Normal, "", &ok);
    if (ok) {
        if (title.isEmpty()) {
            return;
        }
        ui->tabWidget->setCurrentIndex(3);
        QSqlQuery query;
        query.exec(QString("SELECT * FROM Book WHERE title LIKE '%%1%'").arg(title));
        QStandardItemModel* bookModel = new QStandardItemModel(ui->searchView);
        bookModel->clear();
        bookModel->setHorizontalHeaderLabels(QStringList() << QStringLiteral("���") << QStringLiteral("ISBN")
                                         << QStringLiteral("����") << QStringLiteral("����")
                                         << QStringLiteral("������") << QStringLiteral("�۸�")
                                         << QStringLiteral("�Ƿ�ɽ�"));
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
                bookModel->setItem(row, 6, new QStandardItem(QStringLiteral("�ɽ�")));
            } else {
                bookModel->setItem(row, 6, new QStandardItem(QStringLiteral("�ѽ��")));
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
    QString name = QInputDialog::getText(nullptr, QStringLiteral("��ѯ����"),
                                        QStringLiteral("������Ҫ��ѯ�Ķ���������"), QLineEdit::Normal, "", &ok);
    if (ok) {
        if (name.isEmpty()) {
            return;
        }
        ui->tabWidget->setCurrentIndex(3);
        QSqlQuery query;
        query.exec(QString("SELECT * FROM Reader WHERE name LIKE '%%1%'").arg(name));
        QStandardItemModel* readerModel = new QStandardItemModel(ui->searchView);
        readerModel->clear();
        readerModel->setHorizontalHeaderLabels(QStringList() << QStringLiteral("ѧ��") << QStringLiteral("����")
                                         << QStringLiteral("�༶"));
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
    QAction* freshBookAction = new QAction(QStringLiteral("ˢ���鼮�б�(&B)"), this);
    freshBookAction->setStatusTip(QStringLiteral("ˢ���鼮һ���б�"));
    freshBookAction->setShortcut(tr("Ctrl+B"));
    connect(freshBookAction, &QAction::triggered, this, &MainWindow::refreshBook);

    QAction* freshReaderAction = new QAction(QStringLiteral("ˢ�¶����б�(&R)"), this);
    freshReaderAction->setStatusTip(QStringLiteral("ˢ�¶���һ���б�"));
    freshReaderAction->setShortcut(tr("Ctrl+R"));
    connect(freshReaderAction, &QAction::triggered, this, &MainWindow::refreshReader);

    QAction* freshLentAction = new QAction(QStringLiteral("ˢ�½���б�(&L)"), this);
    freshLentAction->setStatusTip(QStringLiteral("ˢ�½��һ���б�"));
    freshLentAction->setShortcut(tr("Ctrl+L"));
    connect(freshLentAction, &QAction::triggered, this, &MainWindow::refreshLent);

    QAction* exitProgramAction = new QAction(QStringLiteral("�˳�(&E)"), this);
    exitProgramAction->setStatusTip(QStringLiteral("�˳�����"));
    connect(exitProgramAction, &QAction::triggered, this, &MainWindow::close);

    QMenu *file = menuBar()->addMenu(QStringLiteral("�ļ�(&F)"));
    file->addAction(freshBookAction);
    file->addAction(freshReaderAction);
    file->addAction(freshLentAction);
    file->addSeparator();
    file->addAction(exitProgramAction);

    QAction* lendAction = new QAction(QStringLiteral("����鼮(&L)"), this);
    lendAction->setStatusTip(QStringLiteral("����鼮"));
    connect(lendAction, &QAction::triggered, &Operations::lendBook);

    QAction* returnAction = new QAction(QStringLiteral("�黹�鼮(&R)"), this);
    returnAction->setStatusTip(QStringLiteral("�黹�鼮���"));
    connect(returnAction, &QAction::triggered, &Operations::returnBook);

    QAction* renewAction = new QAction(QStringLiteral("�����鼮(&N)"), this);
    renewAction->setStatusTip(QStringLiteral("�����鼮һ����"));
    connect(renewAction, &QAction::triggered, &Operations::renewBook);

    QMenu *oper = menuBar()->addMenu(QStringLiteral("����(&O)"));
    oper->addAction(lendAction);
    oper->addAction(returnAction);
    oper->addAction(renewAction);

    QAction* findBookAction = new QAction(QStringLiteral("�����鼮(&B)"), this);
    findBookAction->setStatusTip(QStringLiteral("�����鼮��Ϣ"));
    connect(findBookAction, &QAction::triggered, this, &MainWindow::SearchResBook);
    QAction* findReaderAction = new QAction(QStringLiteral("���Ҷ���(&R)"), this);
    findReaderAction->setStatusTip(QStringLiteral("���Ҷ�����Ϣ"));
    connect(findReaderAction, &QAction::triggered, this, &MainWindow::SearchResReader);

    QAction* addBookAction = new QAction(QStringLiteral("�����鼮(&B)"), this);
    addBookAction->setStatusTip(QStringLiteral("�����鼮��Ϣ"));
    connect(addBookAction, &QAction::triggered, &Operations::addBook);
    QAction* addReaderAction = new QAction(QStringLiteral("���Ӷ���(&R)"), this);
    addReaderAction->setStatusTip(QStringLiteral("���Ӷ�����Ϣ"));
    connect(addReaderAction, &QAction::triggered, &Operations::addReader);

    QAction* deleteBookAction = new QAction(QStringLiteral("ɾ���鼮(&B)"), this);
    deleteBookAction->setStatusTip(QStringLiteral("ɾ���鼮��Ϣ"));
    connect(deleteBookAction, &QAction::triggered, &Operations::deleteBook);
    QAction* deleteReaderAction = new QAction(QStringLiteral("ɾ������(&R)"), this);
    deleteReaderAction->setStatusTip(QStringLiteral("ɾ��������Ϣ"));
    connect(deleteReaderAction, &QAction::triggered, &Operations::deleteReader);

    QAction* changeBookAction = new QAction(QStringLiteral("�޸��鼮(&B)"), this);
    changeBookAction->setStatusTip(QStringLiteral("�޸��鼮��Ϣ"));
    connect(changeBookAction, &QAction::triggered, &Operations::modifyBook);
    QAction* changeReaderAction = new QAction(QStringLiteral("�޸Ķ���(&R)"), this);
    changeReaderAction->setStatusTip(QStringLiteral("�޸Ķ�����Ϣ"));
    connect(changeReaderAction, &QAction::triggered, &Operations::modifyReader);

    QAction* optionsAction = new QAction(QStringLiteral("�޸�����(&O)"), this);
    optionsAction->setStatusTip(QStringLiteral("�޸Ľ����������"));
    connect(optionsAction, &QAction::triggered, &Operations::changeSetting);

    QMenu *edit = menuBar()->addMenu(QStringLiteral("�༭(&E)"));
    QMenu *find = edit->addMenu(QStringLiteral("����(&F)"));
    find->addAction(findBookAction);
    find->addAction(findReaderAction);
    QMenu *add = edit->addMenu(QStringLiteral("����(&A)"));
    add->addAction(addBookAction);
    add->addAction(addReaderAction);
    QMenu *_delete = edit->addMenu(QStringLiteral("ɾ��(&D)"));
    _delete->addAction(deleteBookAction);
    _delete->addAction(deleteReaderAction);
    QMenu *change = edit->addMenu(QStringLiteral("�޸�(&C)"));
    change->addAction(changeBookAction);
    change->addAction(changeReaderAction);
    edit->addSeparator();
    edit->addAction(optionsAction);

    QAction* changePwdAction = new QAction(QStringLiteral("�޸�����(&C)"), this);
    changePwdAction->setStatusTip(QStringLiteral("�޸ĵ�ǰ����Ա�˻�����"));
    connect(changePwdAction, &QAction::triggered, &Operations::changePassword);

    QAction* addAccountAction = new QAction(QStringLiteral("���ӹ���Ա(&A)"), this);
    addAccountAction->setStatusTip(QStringLiteral("���ӹ���Ա�˻�"));
    connect(addAccountAction, &QAction::triggered, &Operations::addAdmin);

    QAction* deleteAction = new QAction(QStringLiteral("ɾ������Ա(&D)"), this);
    deleteAction->setStatusTip(QStringLiteral("ɾ������Ա�˻�"));
    connect(deleteAction, &QAction::triggered, &Operations::deleteAdmin);

    QMenu *account = menuBar()->addMenu(QStringLiteral("�˻�(&A)"));
    account->addAction(changePwdAction);
    account->addAction(addAccountAction);
    account->addAction(deleteAction);

    QAction* aboutAction = new QAction(QStringLiteral("����(&A)"), this);
    aboutAction->setStatusTip(QStringLiteral("����"));
    connect(aboutAction, &QAction::triggered,
            [=] {
        QMessageBox about(this);
        about.setWindowTitle(QStringLiteral("����"));
        about.setText(QStringLiteral("ͼ��ݹ���ϵͳ Version:1.0\n\n���ߣ� ��142 ������"));
        about.exec();
    });

    QMenu *help = menuBar()->addMenu(QStringLiteral("����(&H)"));
    help->addAction(aboutAction);
}
