#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void refreshBook();
    void refreshReader();
    void refreshLent();
    void noSearchResult();
    void SearchResBook();
    void SearchResReader();
    void initMenu();
    static int loginLicense;
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
