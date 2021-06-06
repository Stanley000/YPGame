#ifndef CLIENTSTUFF_H
#define CLIENTSTUFF_H

#include <QString>
#include <QTcpSocket>
#include <QDataStream>
#include <QTimer>
#include "functions.h"

class clientStuff : public QObject
{
    Q_OBJECT

public:
    clientStuff(const QString hostAddress, int portVal, QObject *parent = 0);

    QTcpSocket *tcpSocket;
    bool getStatus();

public slots:
    void closeConnection();
    void connect2host();

signals:
    void statusChanged(bool);/// Статус подключения к серверу
    void hasReadSome(QString msg);/// Получение сообщения с сервера
    void hasReadMap(QMap<int, QString> map);/// Получение QMap лобби с сервера

private slots:
    void readyRead();
    void connected();
    void connectionTimeout();

private:
    QString host;
    int port;
    bool status;
    quint16 m_nNextBlockSize;
    QTimer *timeoutTimer;
};

#endif // CLIENTSTUFF_H
