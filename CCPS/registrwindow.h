#ifndef REGISTRWINDOW_H
#define REGISTRWINDOW_H

#include <QMainWindow>
#include "functions.h"
#include "clientstuff.h"
#include "work.h"

namespace Ui {
class RegistrWindow;
}

class RegistrWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegistrWindow(clientStuff *getclient, QWidget *parent = nullptr);
    ~RegistrWindow();

private slots:
    void on_pushButton_Close_clicked();
    void on_pushButton_Reg_clicked();
    void open_workWindow();
    void receivedSomething(QString msg);

private:
    Ui::RegistrWindow *ui;
    clientStuff *client;
    Work *work;

signals:
    void signReg();
};

#endif // REGISTRWINDOW_H
