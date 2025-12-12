#ifndef CONTACT_H
#define CONTACT_H

#include <string>
#include <vector>
using namespace std;
class Contact //класс для хранения данных одного контакта
{
public:
    string firstName;      //имя
    string lastName;       //фамилия  
    string patronymic;     //отчество,может быть пустым
    string address;        //адрес
    string dateOfBirth;    //дата рождения в формате ГГГГ-ММ-ДД
    string email;          //email адрес
    vector<string> phones;  //cписок телефонных номеров

   
    Contact();  //конструктор по умолчанию
    Contact(const string& fname, const string& lname,
        const string& patron, const string& addr,
        const string& date, const string& mail,
        const vector<string>& ph);

    //методы для удобства
    string getFullName() const;
    bool hasPhone(const string& phone) const;
};

#endif