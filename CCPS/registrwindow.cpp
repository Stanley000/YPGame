#include "registrwindow.h"
#include "ui_registrwindow.h"

/*!
 * \brief RegistrWindow::RegistrWindow Обьявление класса Registr(Интерфейс для регистрации нового пользователя)
 * \param getclient Переменная для передачи данных клиента для обработки
 * \param parent Параметры ui
 */

RegistrWindow::RegistrWindow(clientStuff *getclient, QWidget *parent) : QMainWindow(parent), ui(new Ui::RegistrWindow)
{
    client = getclient;
    ui->setupUi(this);

    work = new Work(client);

    connect(client, &clientStuff::hasReadSome, this, &RegistrWindow::receivedSomething);
}

/*!
 * \brief RegistrWindow::~RegistrWindow Диструктор
 */

RegistrWindow::~RegistrWindow()
{
    delete ui;
}

/*!
 * \brief RegistrWindow::receivedSomething
 * \param msg
 * Сообщение которое обрабатывается после ответа сервера на запрос о регистрации
 */

void RegistrWindow::receivedSomething(QString msg)
{
    QString getMesProc = msg.section("&", 0, 0);

    if (getMesProc == "reg")
    {
       if (Proc(msg) != "reg&false") {
           open_workWindow();
           qDebug() << "Зашел";
       }
    }
}

/*!
 * \brief RegistrWindow::on_pushButton_Close_clicked
 * Действия выполняемые после нажатия на кнопку "Назад"
 */

void RegistrWindow::on_pushButton_Close_clicked()
{
    disconnect(client, &clientStuff::hasReadSome, this, &RegistrWindow::receivedSomething);
    this->close();
    emit signReg();
}

/*!
 * \brief RegistrWindow::on_pushButton_Reg_clicked
 * Действия выполняемые после нажатия на кнопку "Регистрация"
 */

void RegistrWindow::on_pushButton_Reg_clicked()
{
    //Подготавливаем запрос к серверу
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    //Добавляем к запросу строку с помощью парсера functions.cpp
    out << quint16(0) << parser(ui->lineEdit_Login->text(), ui->lineEdit_Password->text(), ui->lineEdit_Email->text(), ui->lineEdit_Phone->text());

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    //Отправка данных на сервер
    client->tcpSocket->write(arrBlock);
}

/*!
 * \brief RegistrWindow::open_workWindow
 * Открытие формы Work
 */

void RegistrWindow::open_workWindow()
{
    work->show();
    this->close();
}
