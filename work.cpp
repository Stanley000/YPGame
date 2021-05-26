#include "work.h"
#include "ui_work.h"
#include "mainwindow.h"
#include <QScreen>

Work::Work(clientStuff *getclient, QWidget *parent) : QMainWindow(parent), ui(new Ui::Work)
{
    client = getclient;
    ui->setupUi(this);

    //connect(client, &clientStuff::hasReadSome, this, &RegistrWindow::receivedSomething);
}

Work::~Work()
{
    delete ui;
}

void Work::getlistLobby()
{

}

void Work::on_back_button_clicked()
{
    hide();
    emit signM();
}
