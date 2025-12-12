#include "PhoneBook.h"
#include "Validator.h"
#include <iostream>
#include <fstream>
#include <sstream>

//конструктор
PhoneBook::PhoneBook(): nextId(1) {}

int PhoneBook::getCount() const {
    return static_cast<int>(contacts.size());
}

bool PhoneBook::isEmpty() const {
    return contacts.empty();
}
bool PhoneBook::addContact(const Contact& contact)
{
    //проверяем обязательные поля
    if (!Validator::checkName(contact.firstName)) {
        cout << "Ошибка: неверное имя" << endl;
        return false;
    }
    if (!Validator::checkName(contact.lastName)) {
        cout << "Ошибка: неверная фамилия" << endl;
        return false;
    }
    if (!Validator::checkDate(contact.dateOfBirth)) {
        cout << "Ошибка: неверная дата рождения" << endl;
        return false;
    }
    if (!Validator::checkEmail(contact.email)) {
        cout << "Ошибка: неверный email" << endl;
        return false;
    }
    if (!contact.patronymic.empty() && !Validator::checkName(contact.patronymic)) {//проеряем отчество если оно не пустое
        cout << "Ошибка: неверное отчество" << endl;
        return false;
    }

    if (contact.phones.empty()) {//проверить что есть хотя бы один телефон
        cout << "Ошибка: должен быть хотя бы один телефон" << endl;
        return false;
    }

    for (const auto& phone : contact.phones) {//проверка всех телефонов контакта
        if (!Validator::checkPhone(phone)) {
            cout << "Ошибка: неверный номер телефона: " << phone << endl;
            return false;
        }
    }

    contacts[nextId] = contact;//добавляем контакт
    cout << "Контакт добавлен с ID: " << nextId << endl;
    nextId++;

    return true;
}

//удалить контакт
bool PhoneBook::removeContact(int id)
{
    auto it = contacts.find(id);
    if (it != contacts.end()) {
        contacts.erase(it);
        cout << "Контакт с ID " << id << " удален" << endl;
        return true;
    }

    cout << "Ошибка: контакт с ID " << id << " не найден" << endl;
    return false;
}
bool PhoneBook::containsId(int id) const {
    return contacts.find(id) != contacts.end();
}
//обновить контакт
bool PhoneBook::updateContact(int id, const Contact& newContact)
{   
    if (!containsId(id)) {//обязательно проверим есть ли обновляемый контакт
        cout << "Ошибка: контакт с ID " << id << " не найден" << endl;
        return false;
    }
    //проверяем данные контакта
    if (!Validator::checkName(newContact.firstName) || !Validator::checkName(newContact.lastName) || 
        !Validator::checkDate(newContact.dateOfBirth) ||!Validator::checkEmail(newContact.email)) 
    {
        cout << "Ошибка: неверные данные контакта" << endl;
        return false;
    }
    //проверяем есть ли телефон и правильный ли формат
    if (newContact.phones.empty()) { 
        cout << "Ошибка: должен быть хотя бы один телефон" << endl;
        return false;
    }

    for (const auto& phone : newContact.phones) {
        if (!Validator::checkPhone(phone)) {
            cout << "Ошибка: неверный номер телефона: " << phone << endl;
            return false;
        }
    }

    contacts[id] = newContact;
    cout << "Контакт с ID " << id << " обновлен" << endl;
    return true;
}
//вспомогательный метод для вывода контакта
void PhoneBook::printContact(const Contact& contact, int id) const
{
    cout << "ID: " << id << endl;
    cout << "ФИО: " << contact.lastName << " " << contact.firstName;
    if (!contact.patronymic.empty()) {
        cout << " " << contact.patronymic;
    }
    cout << endl;
    cout << "Адрес: " << contact.address << endl;
    cout << "Дата рождения: " << contact.dateOfBirth << endl;
    cout << "Email: " << contact.email << endl;
    cout << "Телефоны: ";
    for (size_t i = 0; i < contact.phones.size(); i++) {
        cout << contact.phones[i];
        if (i < contact.phones.size() - 1) {
            cout << ", ";//чтобы красиво вывести телефоны
        }
    }
    cout << endl;
}
//показать все контакты
void PhoneBook::showAll() const
{
    if (contacts.empty()) {
        cout << "Телефонная книга пуста" << endl;
        return;
    }
    cout << " Все контакты " << endl;
    for (const auto& pair : contacts) {
        printContact(pair.second, pair.first);//пара - ключ-значение
    }
}

//поиск по имени
void PhoneBook::findByName(const string& name) const {
    cout << " Результаты поиска по имени: " << name << endl;
    bool found = false;
    for (const auto& pair : contacts) {
        const Contact& contact = pair.second;
        //искать в имени, фамилии и отчестве
        if (contact.firstName.find(name) != string::npos || 
            contact.lastName.find(name) != string::npos ||contact.patronymic.find(name) != string::npos) {
            printContact(contact, pair.first);
            found = true;
        }
    }
    if (!found) {
        cout << "Контакты не найдены" << endl;
    }
}

//поиск по телефону
void PhoneBook::findByPhone(const string& phone) const {
    cout << " Результаты поиска по телефону: " << phone <<  endl;
    bool found = false;
    for (const auto& pair : contacts) {
        const Contact& contact = pair.second;
        for (const auto& contactPhone : contact.phones) {
            if (contactPhone.find(phone) != string::npos) {
                printContact(contact, pair.first);
                found = true;
                break;
            }
        }
    }
    if (!found) {
        cout << "Контакты не найдены" << endl;
    }
}

//показать конкретный контакт по ID
void PhoneBook::showContact(int id) const {
    auto it = contacts.find(id);
    if (it != contacts.end()) {
        printContact(it->second, id);
    }
    else {
        cout << "Контакт с ID " << id << " не найден" << endl;
    }
}

//cохранить в файл
bool PhoneBook::saveToFile(const string& filename) const {
    ofstream file(filename);//создали и открываем файл для записи
    if (!file.is_open()) { //проверяем открылся ли файл
        cout << "Ошибка: не могу открыть файл " << filename << endl;
        return false;
    }
    for (const auto& pair : contacts) {
        const Contact& contact = pair.second; //записываем данные контакта в файл
        file << pair.first << ",";
        file << contact.firstName << ",";
        file << contact.lastName << ",";
        file << contact.patronymic << ",";
        file << contact.address << ",";
        file << contact.dateOfBirth << ","; 
        file << contact.email << ",";
        for (size_t i = 0; i < contact.phones.size(); i++) {
            file << contact.phones[i]; //записали телефон
            if (i < contact.phones.size() - 1) { // если НЕ последний телефон
                file << ";"; 
            }
        }
        file << endl; 
    }
    file.close();
    cout << "Данные сохранены в файл: " << filename << endl;
    return true;
}

//загрузить из файла
bool PhoneBook::loadFromFile(const string& filename) {
    ifstream file(filename); //открываем файл
    if (!file.is_open()) {
        cout << "Ошибка: не могу открыть файл " << filename << endl;
        return false;
    }
    contacts.clear();
    nextId = 1;
    string line;//одна строка из файла
    while (getline(file, line)) { 
        stringstream ss(line);//создаем поток для чтения из строки
        string token;//строка-часть
        vector<string> tokens;//вектор строк для частей
        while (getline(ss, token, ',')) { //откуда читаем, куда сохраняем, как делим
            tokens.push_back(token); //добавляем в начало
        }
        if (tokens.size() >= 7) {
            Contact contact; //создаем пустой контакт
            int id = stoi(tokens[0]); //заполняем данными
            contact.firstName = tokens[1];
            contact.lastName = tokens[2];
            contact.patronymic = tokens[3];
            contact.address = tokens[4];
            contact.dateOfBirth = tokens[5];
            contact.email = tokens[6];
            if (tokens.size() > 7) { //проверяет есть ли телефоны
                stringstream phoneStream(tokens[7]); //поток для чтения строки с телефонами
                string phone;
                while (getline(phoneStream, phone, ';')) { //читаем телфоны по разделителю точки с запятой
                    contact.phones.push_back(phone); //добавляем в вектор с телефонами
                }
            }
            contacts[id] = contact;
            if (id >= nextId) {
                nextId = id + 1;
            }
        }
    }

    file.close();
    cout << "Данные загружены из файла: " << filename << endl;
    return true;
}



