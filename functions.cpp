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

QString parser(QString login, QString password)
{
    return "auth&" + login + "&" + password;
}

QString parser(QString login, QString password, QString email, QString phone)
{
    return "reg&" + login + "&" + password + "&" + email + "&" + phone;
}
