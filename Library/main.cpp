#include <QApplication>
#include <QTextCodec>

#include "database.h"
#include "logindialog.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icon/library"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    if (!Database::prepare("library.db")) {
        return 0;
    }

    MainWindow mw;
    LoginDialog logDlg;
    if (logDlg.exec() == QDialog::Accepted) {
        mw.show();
        return a.exec();
    } else {
        return 0;
    }
}
