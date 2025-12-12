#ifndef PHONEBOOK_H
#define PHONEBOOK_H

#include "Contact.h"
#include <map>
#include <vector>

using namespace std;

class PhoneBook
{
private:
    map<int, Contact> contacts;//хранилище контактов
    int nextId;//следующий свободный ID

public:
    PhoneBook();
    //основные операции
    bool addContact(const Contact& contact);
    bool removeContact(int id);
    bool updateContact(int id, const Contact& newContact);
    //поиск
    void findByName(const string& name) const;
    void findByPhone(const string& phone) const;
    //показать контакты
    void showAll() const;
    void showContact(int id) const;
    //работа с файлами
    bool saveToFile(const string& filename) const;
    bool loadFromFile(const string& filename);
    //информация
    int getCount() const;
    bool isEmpty() const;
    bool containsId(int id) const;

private:
    void printContact(const Contact& contact, int id) const;
};

#endif