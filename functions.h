#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <QString>

QString Proc(QString message);//авторизация и регистрация

QString parser(QString login, QString password);//парсер авторизации

QString parser(QString login, QString password, QString email, QString phone);//парсер регистрации

#endif // FUNCTIONS_H
