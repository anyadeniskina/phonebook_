#include "Contact.h"

//конструктор по умолчанию, все поля пустые
Contact::Contact(): firstName(""), lastName(""), patronymic(""),
                    address(""), email(""), phones() {}

//конструктор с параметрами
Contact::Contact(const QString& fname, const QString& lname, const QString& patron,
                 const QString& addr, const QDate& date, const QString& mail,
                 const QStringList& ph):
    firstName(fname), lastName(lname), patronymic(patron), address(addr),
    dateOfBirth(date), email(mail), phones(ph) {}

//получить полное имя
QString Contact::getFullName() const
{
    QString result = lastName + " " + firstName;
    if (!patronymic.isEmpty()) {
        result += " " + patronymic;
    }
    return result;
}

//проверить есть ли такой телефон
bool Contact::hasPhone(const QString& phone) const
{
    for (const auto& p : phones) {
        if (p == phone) {
            return true;
        }
    }
    return false;
}
