#include "serverstuff.h"

int lobby::getPlayer()
{
  int amount = 0;
  for (int i = 0; i < 9 ; i++) {
      if (player[amount] > 0)
          amount++;
  }
  return amount;
}

QString lobby::getName()
{
    return name;
}

QString lobby::getReadyLobby()
{
    return QString::number(readyLobby);
}

lobby::lobby(QString nameLobby)
{
    name = nameLobby;

    for(int i = 0; i < 9; i++)
    {
       player[i] = -1;
    }
}

serverStuff::serverStuff(QObject *pwgt) : QObject(pwgt), m_nNextBlockSize(0)
{
    tcpServer = new QTcpServer(this);

    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("topgame");
    db.setUserName("buker");
    db.setPassword("201-351");
    dbconnect = db.open();

    lobby* lob1 = new lobby("test1");
    lob1->player[0] = 1234;
    lob1->player[1] = 125435;
    lob1->player[2] = 134;
    lob1->readyLobby = false;
    curLobby[0] = lob1;

    lobby* lob2 = new lobby("test2");
    lob2->player[0] = 1234;
    lob2->player[1] = 125435;
    lob2->player[2] = 134;
    lob2->player[3] = 123;
    lob2->player[4] = 15435;
    lob2->readyLobby = false;
    curLobby[1] = lob2;
}

QList<QTcpSocket *> serverStuff::getClients()
{
    return clients;
}

QMap<int, QString> serverStuff::getLobby()
{
    QMap<int, lobby *>::const_iterator i = curLobby.constBegin();
    QMap<int, QString> listLobby;

    int q = 0;
    int b = 0;

    while (i != curLobby.constEnd())
    {

        if (i.value()->getReadyLobby() != 0)
        {
            QString str = "lobby&" + QString::number(q) + "&" + QString::number(curLobby[q]->getPlayer()) + "&" + curLobby[q]->getReadyLobby() + "&" + curLobby[q]->getName();
            listLobby[b] = str;
            b++;
        }
        i++;
        q++;
    }

    //qDebug() << "QString map " << listLobby;
    return listLobby;
}

void serverStuff::newConnection()
{
    QTcpSocket *clientSocket = tcpServer->nextPendingConnection();

    connect(clientSocket, &QTcpSocket::disconnected, clientSocket, &QTcpSocket::deleteLater);
    connect(clientSocket, &QTcpSocket::readyRead, this, &serverStuff::readClient);
    connect(clientSocket, &QTcpSocket::disconnected, this, &serverStuff::gotDisconnection);

    clients << clientSocket;

    //sendToClient(clientSocket, "Ответ: Соединение установлено.");
}

void serverStuff::readClient()
{
    QTcpSocket *clientSocket = (QTcpSocket*)sender();



    QDataStream in(clientSocket);
    for (;;)
    {
        if (!m_nNextBlockSize) {
                if (clientSocket->bytesAvailable() < sizeof(quint16)) { break; }
            in >> m_nNextBlockSize;
        }

        if (clientSocket->bytesAvailable() < m_nNextBlockSize) { break; }
        QString str;
        in >> str;

        emit gotNewMesssage(str);

        m_nNextBlockSize = 0;

//        if (sendToClient(clientSocket, QString("Ответ: полученный [%1]").arg(str)) == -1)
//        {
//            qDebug() << "Произошла ошибка";
//        }

        QString id;
        QString action = str.section("&", 0, 0);

        if (action == "auth")
        {
            QString login = str.section("&", 1, 1);
            QString password = str.section("&", 2, 2);

            if (dbconnect) {
                QString req = QString("SELECT id_user FROM data_auth WHERE login = '%1' AND password = '%2'").arg(login, password);
                QSqlQuery query(req);

                    if (query.next()) {
                        id = query.value(0).toString();
                    }
            }

            if (id != "") {
                sendToClient(clientSocket, QMap<int, QString>{{0, "auth&" + id}});
                emit gotNewMesssage("auth&" + id);
            }else {
                sendToClient(clientSocket, QMap<int, QString>{{0, "auth&false"}});
            }
        }

        if (action == "reg")
        {

            QString login = str.section("&", 1, 1);
            QString password = str.section("&", 2, 2);

            if (dbconnect) {
                QString req = QString("SELECT id_user FROM data_auth WHERE login = '%1' OR password = '%2'").arg(login, password);
                QSqlQuery query(req);

                if (query.next()) {
                    id = query.value(0).toString();
                }

                if (id == "") {

                    QSqlQuery query;
                    query.prepare("INSERT INTO data_auth (login, password) VALUES (:login, :password) RETURNING id_user");
                       query.bindValue(":login", login);
                       query.bindValue(":password", password);
                       query.exec();

                       if (query.next()) {
                           id = query.value(0).toString();
                       }

                       sendToClient(clientSocket, QMap<int, QString>{{0, "reg&" + id}});
                    emit gotNewMesssage("reg&" + id);
                } else {
                    sendToClient(clientSocket, QMap<int, QString>{{0, "reg&false"}});
                }

            }
        }

        if (action == "listLobby")
        {
            sendToClient(clientSocket, getLobby());
        }

        if (action == "cLobby")
        {
            QString strkey = str.section("&", 1, 1);
            int key = strkey.toInt();
            QString name = str.section("&", 2, 2);

            if(curLobby.size()==0) {
                lobby* newGame = new lobby(name);
                newGame->readyLobby=0;
                newGame->player[0]=(int)clientSocket->socketDescriptor();
                curLobby[0] = newGame;
                sendToClient(clientSocket, QMap<int, QString>{{0, "cLobby&true"}});
                //sendToClient(clientSocket, QString("cLobby&true"));
            }else
            {
               if (curLobby[key]->player[0] == 0) {
                   if (curLobby.size() > 0){
                       lobby* newGame = new lobby(name);
                       newGame->readyLobby=0;
                       newGame->player[0]=(int)clientSocket->socketDescriptor();
                       curLobby[int(curLobby.lastKey())+1] = newGame;
                       sendToClient(clientSocket, QMap<int, QString>{{0, "cLobby&true"}});
                      // sendToClient(clientSocket, QString("cLobby&true"));
                   }
                }else
                   {
                      lobby *Game = curLobby[key];
                      int aPlayer = Game->getPlayer();
                      if (aPlayer != 9) {
                          Game->player[aPlayer]=(int)clientSocket->socketDescriptor();
                         // sendToClient(clientSocket, QString("cLobby&true"));
                            sendToClient(clientSocket, QMap<int, QString>{{0, "cLobby&true"}});
                        } else
                        {
                            sendToClient(clientSocket, QMap<int, QString>{{0, "cLobby&false"}});
                           // sendToClient(clientSocket, QString("cLobby&false"));
                        }
                    }

            }


        }
    }
}

void serverStuff::gotDisconnection()
{
    clients.removeAt(clients.indexOf((QTcpSocket*)sender()));

    emit smbDisconnected();
}

//qint64 serverStuff::sendToClient(QTcpSocket* socket, const QString& str)
qint64 serverStuff::sendToClient(QTcpSocket* socket, QMap<int, QString> str)
{
//    QByteArray arrBlock;
//    QDataStream out(&arrBlock, QIODevice::WriteOnly);
//    out << quint16(0) << str;

//    out.device()->seek(0);
//    out << quint16(arrBlock.size() - sizeof(quint16));

//    return socket->write(arrBlock);

      QByteArray arrblock;
      QDataStream sendStream(&arrblock, QIODevice::ReadWrite);
      sendStream << quint64(0) << str;

      sendStream.device()->seek(0);
      sendStream << (quint64)(arrblock.size() - sizeof(quint64));     

      return socket->write(arrblock);
}
