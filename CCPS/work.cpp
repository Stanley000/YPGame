#include "work.h"
#include "ui_work.h"
#include "mainwindow.h"
#include <QScreen>

Work::Work(clientStuff *getclient, QWidget *parent) : QMainWindow(parent), ui(new Ui::Work)
{
    client = getclient;
    ui->setupUi(this);


    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << quint16(0) << QString::fromStdString("listLobby");

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    client->tcpSocket->write(arrBlock);

    connect(client, &clientStuff::hasReadMap, this, &Work::getlistLobby);
}

Work::~Work()
{
    disconnect(client, &clientStuff::hasReadMap, this, &Work::getlistLobby);
    delete ui;
}

void Work::getlistLobby(QMap<int, QString> map)
{
    qDebug() << "QString map " << map;
}

void Work::on_back_button_clicked()
{
    hide();
    emit signM();
}
