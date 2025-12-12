#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>

using namespace std;

class Validator
{
public:
    static bool checkName(const string& name);//проверка имени, фамилии или отчества
    static bool checkPhone(const string& phone);//проверка номера телефона
    static bool checkEmail(const string& email);//проверка email
    static bool checkDate(const string& date);//проверка даты рождения в формате ДД-ММ-ГГГГ
    static string trim(const string& str);//очистка строки от пробелов в начале и конце
    static string formatPhone(const string& phone);//привести телефон к стандартному формату
};

#endif