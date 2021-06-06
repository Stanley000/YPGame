#include "functions.h"

QString Proc(QString message)
{
    QString getMesProc = message.section("&", 0, 0);
    QString getMesUUID = message.section("&", 1, 1);

    if (getMesProc == "auth" || getMesProc == "reg")
        if (getMesUUID != "false")
        {
            return getMesUUID;
        }

    if (getMesProc == "error")
    {
        QString getMesError = message.section("&", 2, 2);

        if (getMesError == "1")
        {
            //return
        }
    }

    return "false";
}

/*!
 * \brief parser
 * \param login
 * \param password
 * \return
 * Парсер для подготовки сообщения авторизации
 */

QString parser(QString login, QString password)
{
    return "auth&" + login + "&" + password;
}

/*!
 * \brief parser
 * \param login
 * \param password
 * \param email
 * \param phone
 * \return
 * Парсер для подготовки сообщения регистрации
 */

QString parser(QString login, QString password, QString email, QString phone)
{
    return "reg&" + login + "&" + password + "&" + email + "&" + phone;
}

/*!
 * \brief parser
 * \param map
 * \return
 * Парсер для обработки полученных сообщений от сервера и перевода их в QString или QMap<int,QString>
 */

QString parser(QMap<int, QString> map)
{
    QString str;
    QString flag = "";
    foreach(QString value, map)
    {
        if (flag != "lobbyList"){
            str = value;
            QString action = str.section("&", 0, 0);
            if (action == "lobby")
            {
                flag = "lobbyList";
            }
        }
    }

    if (flag == "lobbyList")
    {
        return "lobby";

    } else
    {
        return str;
    }
}
