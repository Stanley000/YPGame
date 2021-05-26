#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->info->setVisible(false);

    client = new clientStuff("localhost", 25540);

    work = new Work(client);
    registr = new RegistrWindow(client);

    connect(client, &clientStuff::hasReadSome, this, &MainWindow::receivedSomething);
    connect(work, &Work::signM, this, &MainWindow::showAgain);
    connect(registr, &RegistrWindow::signReg, this, &MainWindow::showAgain);

    client->connect2host();
}

MainWindow::~MainWindow()
{
    delete client;
    delete ui;
}

void MainWindow::receivedSomething(QString msg)
{
    if (Proc(msg) != "false")
    {
       open_workWindow();
       qDebug() << "Зашел";
    }
}

void MainWindow::on_pushButton_Auth_clicked()  // кнопка авторизация
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << quint16(0) << parser(ui->lineEdit_Login->text(), ui->lineEdit_Password->text());

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    client->tcpSocket->write(arrBlock);
}

void MainWindow::open_workWindow()
{
    work->show();
    this->close();
}

void MainWindow::showAgain()
{
    connect(client, &clientStuff::hasReadSome, this, &MainWindow::receivedSomething);
    this->show();
}

void MainWindow::on_pushButton_Reg_clicked()
{
    registr->show();
    disconnect(client, &clientStuff::hasReadSome, this, &MainWindow::receivedSomething);
    this->close();
}

