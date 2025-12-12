#include "Contact.h"
#include <algorithm>
//конструктор по умолчанию,все поля пустые
Contact::Contact(): firstName(""), lastName(""), patronymic(""), address(""), dateOfBirth(""), email(""), phones(){}
//конструктор с параметрами
Contact::Contact(const string& fname, const string& lname,const string& patron, 
const string& addr,const string& date, const string& mail,const vector<string>& ph): 
firstName(fname), lastName(lname), patronymic(patron), address(addr), dateOfBirth(date), email(mail), phones(ph){}

//получить полное имя
string Contact::getFullName() const
{
    string result = lastName + " " + firstName;
    if (!patronymic.empty()) {
        result += " " + patronymic;
    }
    return result;
}

//проверить есть ли такой телефон
bool Contact::hasPhone(const string& phone) const
{
 
    for (const auto& p : phones) { //
        if (p == phone) {
            return true;
        }
    }
    return false;
}