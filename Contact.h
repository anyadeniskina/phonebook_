#ifndef CONTACT_H
#define CONTACT_H

#include <QString>
#include <QStringList>
#include <QDate>

class Contact
{
public:
    Contact();
    Contact(const QString& fname, const QString& lname, const QString& patron,
            const QString& addr, const QDate& date, const QString& mail,
            const QStringList& ph);
    QString getFullName() const;
    bool hasPhone(const QString& phone) const;
    QString firstName;
    QString lastName;
    QString patronymic;
    QString address;
    QDate dateOfBirth;//используем Qdate вместо string
    QString email;
    QStringList phones;//вместо vector<>
};
#endif // CONTACT_H
