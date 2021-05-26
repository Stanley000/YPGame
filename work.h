#ifndef WORK_H
#define WORK_H

#include <QMainWindow>
#include "functions.h"
#include "clientstuff.h"

namespace Ui {
class Work;
}

class Work : public QMainWindow
{
    Q_OBJECT

public:
    explicit Work(clientStuff *getclient, QWidget *parent = nullptr);
    ~Work();

private slots:

    void on_back_button_clicked();
    void getlistLobby();

private:
    Ui::Work *ui;
    clientStuff *client;

signals:
    void signM();
};



#endif // WORK_H

