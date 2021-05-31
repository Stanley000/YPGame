#include "registrwindow.h"
#include "ui_registrwindow.h"

RegistrWindow::RegistrWindow(clientStuff *getclient, QWidget *parent) : QMainWindow(parent), ui(new Ui::RegistrWindow)
{
    client = getclient;
    ui->setupUi(this);

    work = new Work(client);

    connect(client, &clientStuff::hasReadSome, this, &RegistrWindow::receivedSomething);
}

RegistrWindow::~RegistrWindow()
{
    delete ui;
}

void RegistrWindow::receivedSomething(QString msg)
{
    if (Proc(msg) != "false")
    {
       open_workWindow();
       qDebug() << "Зашел";
    }
}

void RegistrWindow::on_pushButton_Close_clicked()
{
    disconnect(client, &clientStuff::hasReadSome, this, &RegistrWindow::receivedSomething);
    this->close();
    emit signReg();
}

void RegistrWindow::on_pushButton_Reg_clicked()
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << quint16(0) << parser(ui->lineEdit_Login->text(), ui->lineEdit_Password->text(), ui->lineEdit_Email->text(), ui->lineEdit_Phone->text());

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    client->tcpSocket->write(arrBlock);
}

void RegistrWindow::open_workWindow()
{
    work->show();
    this->close();
}
