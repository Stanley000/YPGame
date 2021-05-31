#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "functions.h"
#include "clientstuff.h"
#include "work.h"
#include "registrwindow.h"

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_Auth_clicked();
    void on_pushButton_Reg_clicked();
    void open_workWindow();
    void receivedSomething(QString msg);
    void showAgain();

private:
    Ui::MainWindow *ui;
    clientStuff *client;
    Work *work;
    RegistrWindow *registr;
};
#endif // MAINWINDOW_H
