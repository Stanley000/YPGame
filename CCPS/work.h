#ifndef WORK_H
#define WORK_H

#include <QMainWindow>
#include "functions.h"
#include "clientstuff.h"
#include <QMap>
#include <QTableWidget>
#include <QVBoxLayout>

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

    void getlistLobby(QMap<int, QString> map);
    void createUI(const QStringList &headers);
    void connectLobby(QTableWidget* table, int row);
    void receivedSomething(QString msg);
    void on_pushButton_Close_clicked();

    void on_pushButton_Reload_clicked();

private:
    Ui::Work *ui;
    clientStuff *client;
    QWidget *mainWidget;
    QTableWidget *tableWidget;
    QVBoxLayout *vlayout;

signals:
    void signM();
};



#endif // WORK_H

