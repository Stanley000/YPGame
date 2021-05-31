#ifndef SERVERSTUFF_H
#define SERVERSTUFF_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QList>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>

class lobby {
private:
    QString name;
public:

    int player[9];
    bool readyLobby;
    lobby(QString nameLobby);
    int getPlayer();
    QString getName();
    QString getReadyLobby();
};

class serverStuff : public QObject
{
    Q_OBJECT

public:
    serverStuff(QObject *pwgt);
    QTcpServer *tcpServer;
    QList<QTcpSocket *> getClients();
    QMap<int, QString> getLobby();
    QSqlDatabase db;
    bool dbconnect;

public slots:
    virtual void newConnection();
    void readClient();
    void gotDisconnection();
    //qint64 sendToClient(QTcpSocket *socket, const QString &str);
    qint64 sendToClient(QTcpSocket *socket, QMap<int, QString> str);

signals:
    void gotNewMesssage(QString msg);
    void smbDisconnected();

private:
    quint16 m_nNextBlockSize;
    QMap<int, lobby *> curLobby;
    QList<QTcpSocket*> clients;
};

#endif // SERVERSTUFF_H
