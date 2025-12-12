#define _CRT_SECURE_NO_WARNINGS
#include "Validator.h"
#include <cctype>
#include <regex>
#include <ctime>

//убрать пробелы в начале и конце строки
string Validator::trim(const string& str)
{
    if (str.empty()) return "";//если строка пустая
    size_t start = 0;
    size_t end = str.length() - 1;
    while (start <= end && isspace(static_cast<unsigned char>(str[start]))) {//ищем первый пробел сначала
        start++;
    }
    while (end >= start && isspace(static_cast<unsigned char>(str[end]))) {//ищем где заканчиваются символы и начинаются пробелы
        end--;
    }
    if (start > end) return "";//если в строке только пробелы
    return str.substr(start, end - start + 1);//вырезаем кусок строки бе пробелов
}

//проверка имени
bool Validator::checkName(const string& name)
{
    string cleanName = trim(name);
    if (cleanName.empty()) return false;
    if (!isalpha(static_cast<unsigned char>(cleanName[0]))) { //первый символ только буква
        return false;
    }
    if (!isalnum(static_cast<unsigned char>(cleanName.back()))) {//последний символ буква или цифра
        return false;
    }
    for (char c : cleanName) {//проверка каждого символа строки, 
        if (!isalnum(static_cast<unsigned char>(c)) && c != '-' && c != ' ') {//условие: НЕ цифра/буква/дефис/пробел
            return false;
        }
    }

    return true;
}

//привести телефон к стандартному виду
string Validator::formatPhone(const string& phone)
{
    string result;
    if (!phone.empty() && phone[0] == '+') {
        result += '+';//сохранить плюс в начале, если есть
    }
    for (char c : phone) {
        if (isdigit(static_cast<unsigned char>(c))) {
            result += c;//оставить только цифры
        }
    }
    if (!result.empty() && result[0] == '8') {
        result = "+7" + result.substr(1);//если номер начинается с 8, заменить на +7
    }
    else if (!result.empty() && result[0] == '7' && phone[0] != '+') {//если номер начинается с 7 и нет плюса, добавить плюс
        result = "+" + result;
    }
    return result;
}

//проверка телефона
bool Validator::checkPhone(const string& phone)
{
    string formatted = formatPhone(phone);
    if (formatted.length() != 12) {//длина номера четко 12 цифр
        return false;
    }

    if (formatted.substr(0, 2) != "+7") {//проверить начало номера
        return false;
    }
    for (size_t i = 2; i < formatted.length(); i++) {
        if (!isdigit(static_cast<unsigned char>(formatted[i]))) {//все остальные символы должны быть цифрами
            return false;
        }
    }
    return true;
}

//проверка email
bool Validator::checkEmail(const string& email)
{
    string cleanEmail = trim(email);
    string noSpaces;
    for (char c : cleanEmail) {
        if (!isspace(static_cast<unsigned char>(c))) {//убираем пробелы внутри строки
            noSpaces += c;
        }
    }
    size_t atPos = noSpaces.find('@');
    if (atPos == string::npos || atPos == 0 || atPos == noSpaces.length() - 1) {//проверка позиции @(НЕТ/ПЕРВЫЙ/ПОСЛЕДНИЙ)
        return false;
    }
    size_t dotPos = noSpaces.find('.', atPos);//позиция точки после позиции @
    if (dotPos == string::npos || dotPos == atPos + 1) { //проверка позиции точки(нет/сразу после собаки)
        return false;
    }
    for (size_t i = 0; i < atPos; i++) {
        char c = noSpaces[i];
        if (!isalnum(static_cast<unsigned char>(c)) && c != '.' && c != '_' && c != '%' && c != '+' && c != '-') {
            return false;//проверка символов до @,исключаем запрещенные стмволы
        }
    }

    for (size_t i = atPos + 1; i < noSpaces.length(); i++) {//перебираем символы после @ до конца строки
        char c = noSpaces[i];//
        if (!isalnum(static_cast<unsigned char>(c)) && c != '.' && c != '-') {//если символ - это что-то запрещённое в домене
            return false;
        }
    }

    return true;
}

//проверка даты в формате ДД-ММ-ГГГГ
bool Validator::checkDate(const string& date)
{
    if (date.length() != 10) return false;//проверить формат ДД-ММ-ГГГГ
    if (date[2] != '-' || date[5] != '-') return false;
    for (int i = 0; i < 10; i++) {
        if (i != 2 && i != 5 && !isdigit(static_cast<unsigned char>(date[i]))) {//проверить что все символы цифры кроме дефисов
            return false;
        }
    }
    int day = stoi(date.substr(0, 2));//извлечь компоненты даты (ДД-ММ-ГГГГ) stoi-string to integer
    int month = stoi(date.substr(3, 2));
    int year = stoi(date.substr(6, 4));
    if (month < 1 || month > 12) return false;

    if (day < 1 || day > 31) return false;

    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {//месяцы с 30 днями
        return false;
    }

    if (month == 2) {//февраль
        bool leapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);//проверить високосный год
        if (day > (leapYear ? 29 : 28)) {
            return false;
        }
    }

    //проверить что дата не в будущем
    time_t now = time(nullptr);
    tm currentTime;
    localtime_s(&currentTime, &now);//получили текущую дату

    int currentYear = currentTime.tm_year + 1900;//tm_year хранит с 1900
    int currentMonth = currentTime.tm_mon + 1;//хранит месяцы с 0
    int currentDay = currentTime.tm_mday;

    if (year > currentYear) return false;
    if (year == currentYear && month > currentMonth) return false;
    if (year == currentYear && month == currentMonth && day > currentDay) return false;

    return true;
}