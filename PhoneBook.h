#ifndef PHONEBOOK_H
#define PHONEBOOK_H

#include "Contact.h"
#include "Validator.h"
#include <QMap>
#include <QString>
#include <QFile>
#include <QTextStream>

class PhoneBook
{
public:
    PhoneBook();
    int getCount() const;
    bool isEmpty() const;
    bool addContact(const Contact& contact);
    bool removeContact(int id);
    bool containsId(int id) const;
    bool updateContact(int id, const Contact& newContact);
    QMap<int, Contact> getAllContacts() const { return contacts; }
    Contact getContact(int id) const;
    void findByName(const QString& name) const;
    void findByPhone(const QString& phone) const;
    void showContact(int id) const;
    void showAll() const;
    bool saveToFile(const QString& filename) const;
    bool loadFromFile(const QString& filename);
    bool saveToDatabase();
    bool loadFromDatabase();
    void setDatabaseCredentials(const QString& host, int port, const QString& dbName, const QString& user, const QString& password);
    bool isDatabaseConnected() const;

private:
    //вспомогательный метод для вывода контакта
    void printContact(const Contact& contact, int id) const;

    QMap<int, Contact> contacts;//хранилище контактов
    int nextId;//следующий свободный ID
    QString dbHost;
    int dbPort;
    QString dbName;
    QString dbUser;
    QString dbPassword;
    bool dbInitialized;
};

#endif
