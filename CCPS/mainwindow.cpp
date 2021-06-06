#include "mainwindow.h"
#include "ui_mainwindow.h"

/*!
 * \brief MainWindow::MainWindow Обьявление класса MainWindow(Интерфейс для авторизации пользователя)
 * \param parent
 */

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->info->setVisible(false);

    client = new clientStuff("localhost", 25540); //Подключение к серверу

    connect(client, &clientStuff::hasReadSome, this, &MainWindow::receivedSomething);
    //connect(work, &Work::signM, this, &MainWindow::showAgain);
    //connect(registr, &RegistrWindow::signReg, this, &MainWindow::showAgain);

    client->connect2host();
}

/*!
 * \brief MainWindow::~MainWindow Диструктор
 */

MainWindow::~MainWindow()
{
    delete client;
    delete ui;
}

/*!
 * \brief MainWindow::receivedSomething
 * \param msg
 * Сообщение которое обрабатывается после ответа сервера на запрос о авторизации
 */

void MainWindow::receivedSomething(QString msg)
{
    QString getMesProc = msg.section("&", 0, 0);

    if (getMesProc == "auth")
    {
        if (Proc(msg) != "auth&false")
        {
           open_workWindow();
           qDebug() << "Зашел";
        }
    }
}

/*!
 * \brief MainWindow::on_pushButton_Auth_clicked
 * Действия выполняемые после нажатия на кнопку "Авторизация"
 */

void MainWindow::on_pushButton_Auth_clicked()
{
    //Подготавливаем запрос к серверу
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    //Добавляем к запросу строку с помощью парсера functions.cpp
    out << quint16(0) << parser(ui->lineEdit_Login->text(), ui->lineEdit_Password->text());

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    //Отправка данных на сервер
    client->tcpSocket->write(arrBlock);
}

/*!
 * \brief MainWindow::open_workWindow
 * Открытие формы Work
 */

void MainWindow::open_workWindow()
{
    work = new Work(client);
    work->show();
    disconnect(client, &clientStuff::hasReadSome, this, &MainWindow::receivedSomething);
    //disconnect(work, &Work::signM, this, &MainWindow::showAgain);
    //disconnect(registr, &RegistrWindow::signReg, this, &MainWindow::showAgain);
    this->close();
}

/*!
 * \brief MainWindow::showAgain
 */

void MainWindow::showAgain()
{
    connect(client, &clientStuff::hasReadSome, this, &MainWindow::receivedSomething);
    this->show();
}

/*!
 * \brief MainWindow::on_pushButton_Reg_clicked
 * Открытие формы Registr
 */

void MainWindow::on_pushButton_Reg_clicked()
{
    registr = new RegistrWindow(client);
    registr->show();
    disconnect(client, &clientStuff::hasReadSome, this, &MainWindow::receivedSomething);
    //disconnect(work, &Work::signM, this, &MainWindow::showAgain);
    //disconnect(registr, &RegistrWindow::signReg, this, &MainWindow::showAgain);
    this->close();
}

