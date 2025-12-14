#define _CRT_SECURE_NO_WARNINGS
#include "Validator.h"
#include <QDate>

//убрать пробелы в начале и конце строки
QString Validator::trim(const QString& str)
{
    return str.trimmed(); //используем встроенный метод Qt для удаления пробелов
}

//проверка имени, фамилии или отчества
bool Validator::checkName(const QString& name)
{
    QString cleanName = trim(name);//очищаем от пробелов
    if (cleanName.isEmpty()) return false;//если строка пустая

    //первый символ только буква
    if (!cleanName[0].isLetter()) {
        return false;
    }

    //последний символ буква или цифра
    if (!cleanName[cleanName.length() - 1].isLetterOrNumber()) {
        return false;
    }

    //проверка каждого символа строки
    for (int i = 0; i < cleanName.length(); i++) {
        QChar c = cleanName[i];
        // условие: НЕ цифра/буква/дефис/пробел
        if (!c.isLetterOrNumber() && c != '-' && c != ' ') {
            return false;
        }
    }

    return true;
}

// привести телефон к стандартному виду
QString Validator::formatPhone(const QString& phone)
{
    QString result;

    // сохранить плюс в начале, если есть
    if (!phone.isEmpty() && phone[0] == '+') {
        result += '+';
    }

    // оставить только цифры
    for (int i = 0; i < phone.length(); i++) {
        if (phone[i].isDigit()) {
            result += phone[i];
        }
    }

    // преобразовать российские номера
    if (!result.isEmpty()) {
        if (result[0] == '8') {
            // если номер начинается с 8, заменить на +7
            result = "+7" + result.mid(1);
        }
        else if (result[0] == '7' && phone[0] != '+') {
            // если номер начинается с 7 и нет плюса, добавить плюс
            result = "+" + result;
        }
    }

    return result;
}

// проверка телефона
bool Validator::checkPhone(const QString& phone)
{
    QString formatted = formatPhone(phone); // форматируем номер

    // длина номера четко 12 цифр (+7 и 10 цифр)
    if (formatted.length() != 12) {
        return false;
    }

    // проверить начало номера
    if (!formatted.startsWith("+7")) {
        return false;
    }

    // все остальные символы должны быть цифрами
    for (int i = 2; i < formatted.length(); i++) {
        if (!formatted[i].isDigit()) {
            return false;
        }
    }

    return true;
}

// проверка email
bool Validator::checkEmail(const QString& email)
{
    QString cleanEmail = trim(email); // убираем пробелы по краям
    QString noSpaces;

    // убираем пробелы внутри строки
    for (int i = 0; i < cleanEmail.length(); i++) {
        if (!cleanEmail[i].isSpace()) {
            noSpaces += cleanEmail[i];
        }
    }

    // ищем символ @
    int atPos = noSpaces.indexOf('@');
    // проверка позиции @ (НЕТ/ПЕРВЫЙ/ПОСЛЕДНИЙ)
    if (atPos == -1 || atPos == 0 || atPos == noSpaces.length() - 1) {
        return false;
    }

    // позиция точки после позиции @
    int dotPos = noSpaces.indexOf('.', atPos);
    // проверка позиции точки (нет/сразу после собаки)
    if (dotPos == -1 || dotPos == atPos + 1) {
        return false;
    }

    // проверка символов до @
    for (int i = 0; i < atPos; i++) {
        QChar c = noSpaces[i];
        // проверка символов до @, исключаем запрещенные символы
        if (!c.isLetterOrNumber() && c != '.' && c != '_' &&
            c != '%' && c != '+' && c != '-') {
            return false;
        }
    }

    // перебираем символы после @ до конца строки
    for (int i = atPos + 1; i < noSpaces.length(); i++) {
        QChar c = noSpaces[i];
        // если символ - это что-то запрещённое в домене
        if (!c.isLetterOrNumber() && c != '.' && c != '-') {
            return false;
        }
    }

    return true;
}

// проверка даты в формате ДД-ММ-ГГГГ
bool Validator::checkDate(const QString& date)
{
    // проверить формат ДД-ММ-ГГГГ
    if (date.length() != 10) return false;
    if (date[2] != '-' || date[5] != '-') return false;

    // проверить что все символы цифры кроме дефисов
    for (int i = 0; i < 10; i++) {
        if (i != 2 && i != 5 && !date[i].isDigit()) {
            return false;
        }
    }

    // извлечь компоненты даты (ДД-ММ-ГГГГ)
    int day = date.mid(0, 2).toInt();   // день
    int month = date.mid(3, 2).toInt(); // месяц
    int year = date.mid(6, 4).toInt();  // год

    // проверка месяца
    if (month < 1 || month > 12) return false;

    // проверка дня
    if (day < 1 || day > 31) return false;

    // месяцы с 30 днями
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {
        return false;
    }

    // февраль
    if (month == 2) {
        // проверить високосный год
        bool leapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (day > (leapYear ? 29 : 28)) {
            return false;
        }
    }

    // проверить что дата не в будущем
    QDate currentDate = QDate::currentDate(); // получили текущую дату
    QDate inputDate(year, month, day);        // дата из строки

    if (inputDate > currentDate) {
        return false;
    }

    return true;
}
