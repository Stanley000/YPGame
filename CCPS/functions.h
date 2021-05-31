#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <QString>
#include <QMap>

QString parser(QMap<int, QString> map); //парсер ответа от сервера

QString Proc(QString message);//авторизация и регистрация

QString parser(QString login, QString password);//парсер авторизации

QString parser(QString login, QString password, QString email, QString phone);//парсер регистрации

#endif // FUNCTIONS_H
