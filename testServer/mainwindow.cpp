#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    server = new serverStuff(this);
    connect(server, &serverStuff::gotNewMesssage,
            this, &MainWindow::gotNewMesssage);
    connect(server->tcpServer, &QTcpServer::newConnection,
            this, &MainWindow::smbConnectedToServer);
    connect(server, &serverStuff::smbDisconnected,
            this, &MainWindow::smbDisconnectedFromServer);


}

MainWindow::~MainWindow()
{
    delete server;
    delete ui;
}

void MainWindow::on_pushButton_startServer_clicked()
{
    if (!server->tcpServer->listen(QHostAddress::Any, 25540))
    {
        ui->textEdit_log->append(tr("<font color=\"red\"><b>Ошибка!</b> Порт занят какой-то другой службой.</font>"));
        return;
    }
    connect(server->tcpServer, &QTcpServer::newConnection, server, &serverStuff::newConnection);

    ui->textEdit_log->append(tr("<font color=\"green\"><b>Сервер запущен</b>, порт открыт.</font>"));
    ui->pushButton_startServer->setEnabled(false);
}

void MainWindow::on_pushButton_stopServer_clicked()
{
    if(server->tcpServer->isListening())
    {
        disconnect(server->tcpServer, &QTcpServer::newConnection, server, &serverStuff::newConnection);

        QList<QTcpSocket *> clients = server->getClients();
        for(int i = 0; i < clients.count(); i++)
        {
            server->sendToClient(clients.at(i), QMap<int, QString>{{0, "0"}});
            //server->sendToClient(clients.at(i), "0");
        }

        server->tcpServer->close();
        ui->pushButton_startServer->setEnabled(true);
        ui->textEdit_log->append(tr("<b>Сервер остановлен</b>, порт закрыт."));
    }
    else
    {
        ui->textEdit_log->append(tr("<b>Ошибка!</b> Сервер не запущен."));
    }
}

void MainWindow::on_pushButton_testConn_clicked()
{
    if(server->tcpServer->isListening())
    {
        ui->textEdit_log->append(
                    QString("%1 %2")
                    .arg("Сервер запущен, количество подключенных клиентов:")
                    .arg(QString::number(server->getClients().count()))
                    );
    }
    else
    {
        ui->textEdit_log->append(
                    QString("%1 %2")
                    .arg("Север выключен, количество подключенных клиентов:")
                    .arg(QString::number(server->getClients().count()))
                    );
    }
}

void MainWindow::smbConnectedToServer()
{
    ui->textEdit_log->append(tr("Кто-то подключился"));
}

void MainWindow::smbDisconnectedFromServer()
{
    ui->textEdit_log->append(tr("Кто-то отключился"));
}

void MainWindow::gotNewMesssage(QString msg)
{
    ui->textEdit_log->append(QString("Новое сообщение: %1").arg(msg));
}
