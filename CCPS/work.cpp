#include "work.h"
#include "ui_work.h"
#include "mainwindow.h"
#include <QScreen>

///

/*!
 * \brief Work::Work Обьявление класса Work(Интерфейс с выбором лобби, Личным кабинетом и кнопкой назад)
 * \param getclient Переменная для передачи данных клиента для обработки
 * \param parent Параметры ui
 */

Work::Work(clientStuff *getclient, QWidget *parent) : QMainWindow(parent), ui(new Ui::Work)
{
    client = getclient;
    ui->setupUi(this);

    this->setWindowTitle("QTableWidget Example");

    this->createUI(QStringList() << trUtf8("") << trUtf8("") << trUtf8("") << trUtf8(""));

    on_pushButton_Reload_clicked();

    connect(client, &clientStuff::hasReadMap, this, &Work::getlistLobby);
    connect(client, &clientStuff::hasReadSome, this, &Work::receivedSomething);
}

/*!
 * \brief Work::~Work Диструктор
 * Удаляет ui и отключает сигналы для получения сообщений
 */

Work::~Work()
{
    disconnect(client, &clientStuff::hasReadMap, this, &Work::getlistLobby);
    disconnect(client, &clientStuff::hasReadSome, this, &Work::receivedSomething);
    delete ui;
}

/*!
 * \brief Work::receivedSomething
 * \param msg
 * Сообщение которое обрабатывается после ответа сервера на запрос на вход
 */

void Work::receivedSomething(QString msg)
{
    qDebug() << "Вошел - " + msg;
}

/*!
 * \brief Work::connectLobby
 * \param table
 * \param row
 * Обрабатывается запрос к серверу для проверки подключения к лобби
 */

void Work::connectLobby(QTableWidget* table, int row)
{
    //qDebug() << table->item(row, 0)->text();

    QString key = table->item(row, 0)->text(); //Вытаскиваем из таблицы пункт ID
    QString name = table->item(row, 1)->text(); //Вытаскиваем из таблицы пункт Name

    //Подготовка запроса
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << quint16(0) << ("cLobby&" + key + "&" + name); //Добавление к запросу строки

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    //Отправка данных на сервер
    client->tcpSocket->write(arrBlock);
}

/*!
 * \brief Work::createUI
 * \param headers
 * Создание и настройка таблицы tableWidget
 */

void Work::createUI(const QStringList &headers)
{
    ui->tableWidget->setColumnCount(4); //число колонок
    ui->tableWidget->setShowGrid(true); //включаем сетку
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); //разрешаем выделять только один элемент
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //разрешаем выделять строки
    ui->tableWidget->setHorizontalHeaderLabels(headers); //устанавливаем заголовки
    ui->tableWidget->setEditTriggers(0); //запрещаем изменение строк
    ui->tableWidget->horizontalHeader()->setStretchLastSection(QHeaderView::Stretch); //растягиваем последнюю колонку на все доступное территорию
    ui->tableWidget->hideColumn(0); //скрываем колонку под номером 0
}

/*!
 * \brief Work::getlistLobby
 * \param map Данные полученные после парсинга данных с сервера QMap<int, QString>
 * Заполнение таблицы данными полученные с сервера
 */

void Work::getlistLobby(QMap<int, QString> map)
{

    foreach(QString val, map)
    {

        QString idLobby = val.section("&", 1, 1); //Получаем из строки ID
        QString countPlayer = val.section("&", 2, 2); //Получаем из строки Количество Игроков
        QString name = val.section("&", 4, 4); //Получаем из строки название лобби

        countPlayer += "/10"; // Добавляем доп. строку

        //Обьявления переменных для создания кнопки "Connect"
        QWidget* pWidget = new QWidget();
        QHBoxLayout* pLayout = new QHBoxLayout(pWidget);
        QPushButton* btn_connect = new QPushButton("Connect");

        //Настраиваем интерфейс кнопки
        pLayout->addWidget(btn_connect);
        pLayout->setAlignment(Qt::AlignCenter);
        pLayout->setContentsMargins(0, 0, 0, 0);
        pWidget->setLayout(pLayout);

        //Вставляем данные в таблицу
        int lastRow = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(lastRow); //вставляем строку
        //Установка информации в таблицу
        ui->tableWidget->setItem(lastRow, 0, new QTableWidgetItem(idLobby));
        ui->tableWidget->setItem(lastRow, 1, new QTableWidgetItem(name));
        ui->tableWidget->setItem(lastRow, 2, new QTableWidgetItem(countPlayer));
        ui->tableWidget->setItem(lastRow, 3, new QTableWidgetItem(""));
        ui->tableWidget->setCellWidget(lastRow, 3, pWidget);
        //ui->tableWidget->setRowHeight(0, 20);

        //Подключаем сигнал который будет реагировать на нажатие на кнопку "Connect"
        QObject::connect(btn_connect, &QPushButton::clicked, this, [=](){ this->connectLobby(ui->tableWidget, lastRow); });
    }

    ui->tableWidget->resizeColumnsToContents();
}

/*!
 * \brief Work::on_pushButton_Close_clicked
 * Действия выполняемые после нажатия на кнопку "Назад"
 */

void Work::on_pushButton_Close_clicked()
{
    //hide();
   // emit signM();
}

/*!
 * \brief Work::on_pushButton_Reload_clicked
 * Действия выполняемые после нажатияя на кнопку "Обновить"
 */

void Work::on_pushButton_Reload_clicked()
{
    ui->tableWidget->setRowCount(0); //Очищаем таблицу

    //Подготавливаем запрос к серверу
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    //Добавляем к запросу строку
    out << quint16(0) << QString::fromStdString("listLobby");

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    //Отправка данных на сервер
    client->tcpSocket->write(arrBlock);
}
