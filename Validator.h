#ifndef VALIDATOR_H
#define VALIDATOR_H

//вместо #include <string>
#include <QString>
#include <QDate>

using namespace std;

class Validator
{
public:
    static bool checkName(const QString& name);//проверка имени, фамилии или отчества
    static bool checkPhone(const QString& phone);//проверка номера телефона
    static bool checkEmail(const QString& email);//проверка email
    static bool checkDate(const QString& date);//проверка даты рождения в формате ДД-ММ-ГГГГ
    static QString trim(const QString& str);//очистка строки от пробелов в начале и конце
    static QString formatPhone(const QString& phone);//привести телефон к стандартному формату
};

#endif // VALIDATOR_H
