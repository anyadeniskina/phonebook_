#include "PhoneBook.h"
#include "DatabaseManager.h"
#include <QTextStream>
#include <QFile>
#include <iostream>

using namespace std;

// конструктор
PhoneBook::PhoneBook(): nextId(1), dbPort(5432), dbInitialized(false) {}

int PhoneBook::getCount() const {
    return contacts.size();
}

bool PhoneBook::isEmpty() const {
    return contacts.empty();
}

bool PhoneBook::containsId(int id) const {
    return contacts.find(id) != contacts.end();
}

bool PhoneBook::addContact(const Contact& contact)
{
    // проверяем обязательные поля
    if (!Validator::checkName(contact.firstName)) {
        cout << "Ошибка: неверное имя" << endl;
        return false;
    }
    if (!Validator::checkName(contact.lastName)) {
        cout << "Ошибка: неверная фамилия" << endl;
        return false;
    }
    if (!Validator::checkDate(contact.dateOfBirth.toString("dd-MM-yyyy"))) {
        cout << "Ошибка: неверная дата рождения" << endl;
        return false;
    }
    if (!Validator::checkEmail(contact.email)) {
        cout << "Ошибка: неверный email" << endl;
        return false;
    }
    if (!contact.patronymic.isEmpty() && !Validator::checkName(contact.patronymic)) {
        cout << "Ошибка: неверное отчество" << endl;
        return false;
    }

    if (contact.phones.empty()) {
        cout << "Ошибка: должен быть хотя бы один телефон" << endl;
        return false;
    }

    for (const auto& phone : contact.phones) {
        if (!Validator::checkPhone(phone)) {
            cout << "Ошибка: неверный номер телефона: " << phone.toStdString() << endl;
            return false;
        }
    }

    contacts[nextId] = contact;
    cout << "Контакт добавлен с ID: " << nextId << endl;
    nextId++;

    return true;
}

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

bool PhoneBook::updateContact(int id, const Contact& newContact)
{
    if (!containsId(id)) {
        cout << "Ошибка: контакт с ID " << id << " не найден" << endl;
        return false;
    }

    if (!Validator::checkName(newContact.firstName) || !Validator::checkName(newContact.lastName) ||
        !Validator::checkDate(newContact.dateOfBirth.toString("dd-MM-yyyy")) || !Validator::checkEmail(newContact.email))
    {
        cout << "Ошибка: неверные данные контакта" << endl;
        return false;
    }

    if (newContact.phones.empty()) {
        cout << "Ошибка: должен быть хотя бы один телефон" << endl;
        return false;
    }

    for (const auto& phone : newContact.phones) {
        if (!Validator::checkPhone(phone)) {
            cout << "Ошибка: неверный номер телефона: " << phone.toStdString() << endl;
            return false;
        }
    }

    contacts[id] = newContact;
    cout << "Контакт с ID " << id << " обновлен" << endl;
    return true;
}

Contact PhoneBook::getContact(int id) const {
    auto it = contacts.find(id);
    if (it != contacts.end()) {
        return it.value();
    }
    return Contact();
}

// показать все контакты
void PhoneBook::showAll() const {
    if (contacts.empty()) {
        cout << "Телефонная книга пуста" << endl;
        return;
    }
    cout << " Все контакты " << endl;

    QMapIterator<int, Contact> it(contacts);
    while (it.hasNext()) {
        it.next();
        printContact(it.value(), it.key());
    }
}

// поиск по имени
void PhoneBook::findByName(const QString& name) const {
    cout << " Результаты поиска по имени: " << name.toStdString() << endl;
    bool found = false;

    QMapIterator<int, Contact> it(contacts);
    while (it.hasNext()) {
        it.next();
        const Contact& contact = it.value();

        if (contact.firstName.contains(name) ||
            contact.lastName.contains(name) ||
            contact.patronymic.contains(name)) {
            printContact(contact, it.key());
            found = true;
        }
    }

    if (!found) {
        cout << "Контакты не найдены" << endl;
    }
}

// поиск по телефону
void PhoneBook::findByPhone(const QString& phone) const {
    cout << " Результаты поиска по телефону: " << phone.toStdString() << endl;
    bool found = false;

    QMapIterator<int, Contact> it(contacts);
    while (it.hasNext()) {
        it.next();
        const Contact& contact = it.value();

        for (const auto& contactPhone : contact.phones) {
            if (contactPhone.contains(phone)) {
                printContact(contact, it.key());
                found = true;
                break;
            }
        }
    }

    if (!found) {
        cout << "Контакты не найдены" << endl;
    }
}

// показать конкретный контакт
void PhoneBook::showContact(int id) const {
    auto it = contacts.find(id);
    if (it != contacts.end()) {
        printContact(it.value(), id);
    }
    else {
        cout << "Контакт с ID " << id << " не найден" << endl;
    }
}

// cохранить в файл
bool PhoneBook::saveToFile(const QString& filename) const {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        cout << "Ошибка: не могу открыть файл " << filename.toStdString() << endl;
        return false;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");

    QMapIterator<int, Contact> it(contacts);
    while (it.hasNext()) {
        it.next();
        const Contact& contact = it.value();

        out << it.key() << ",";
        out << contact.firstName << ",";
        out << contact.lastName << ",";
        out << contact.patronymic << ",";
        out << contact.address << ",";
        out << contact.dateOfBirth.toString("dd-MM-yyyy") << ",";
        out << contact.email << ",";

        for (int i = 0; i < contact.phones.size(); i++) {
            out << contact.phones[i];
            if (i < contact.phones.size() - 1) {
                out << ";";
            }
        }
        out << "\n";
    }

    file.close();
    cout << "Данные сохранены в файл: " << filename.toStdString() << endl;
    return true;
}

// загрузить из файла
bool PhoneBook::loadFromFile(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        cout << "Ошибка: не могу открыть файл " << filename.toStdString() << endl;
        return false;
    }

    contacts.clear();
    nextId = 1;

    QTextStream in(&file);
    in.setCodec("UTF-8");

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split(",");
        if (parts.size() >= 7) {
            Contact contact;

            int id = parts[0].toInt();
            contact.firstName = parts[1];
            contact.lastName = parts[2];
            contact.patronymic = parts[3];
            contact.address = parts[4];
            contact.dateOfBirth = QDate::fromString(parts[5], "dd-MM-yyyy");
            contact.email = parts[6];

            if (parts.size() > 7) {
                QStringList phones = parts[7].split(";");
                contact.phones = phones;
            }

            contacts[id] = contact;
            if (id >= nextId) {
                nextId = id + 1;
            }
        }
    }

    file.close();
    cout << "Данные загружены из файла: " << filename.toStdString() << endl;
    return true;
}
// Новые методы для работы с БД
bool PhoneBook::saveToDatabase()
{
    DatabaseManager& db = DatabaseManager::instance();

    // Подключаемся если не подключены
    if (!db.isConnected()) {
        if (!db.connectToDatabase()) {
            cout << "Ошибка: Не удалось подключиться к базе данных" << endl;
            return false;
        }
    }

    // Сохраняем все контакты
    QMapIterator<int, Contact> it(contacts);
    while (it.hasNext()) {
        it.next();
        if (!db.saveContact(it.value())) {
            cout << "Ошибка при сохранении контакта в базу данных: "
                 << db.lastError().toStdString() << endl;
            return false;
        }
    }

    cout << "Данные сохранены в базу данных. Контактов: " << contacts.size() << endl;
    return true;
}

bool PhoneBook::loadFromDatabase()
{
    DatabaseManager& db = DatabaseManager::instance();

    // Подключаемся если не подключены
    if (!db.isConnected()) {
        if (!db.connectToDatabase()) {
            cout << "Ошибка: Не удалось подключиться к базе данных" << endl;
            return false;
        }
    }

    QMap<int, Contact> loadedContacts = db.loadAllContacts();

    if (loadedContacts.isEmpty()) {
        cout << "В базе данных нет контактов" << endl;
        return false;
    }

    // Заменяем текущие контакты
    contacts = loadedContacts;

    // Обновляем nextId
    nextId = 1;
    if (!contacts.isEmpty()) {
        auto keys = contacts.keys();
        nextId = *std::max_element(keys.begin(), keys.end()) + 1;
    }

    cout << "Данные загружены из базы данных. Загружено контактов: "
         << contacts.size() << endl;
    return true;
}

void PhoneBook::setDatabaseCredentials(const QString& host, int port,
                                      const QString& dbName,
                                      const QString& user,
                                      const QString& password)
{
    this->dbHost = host;
    this->dbPort = port;
    this->dbName = dbName;
    this->dbUser = user;
    this->dbPassword = password;
}

bool PhoneBook::isDatabaseConnected() const
{
    return DatabaseManager::instance().isConnected();
}
// вспомогательный метод для вывода контакта
void PhoneBook::printContact(const Contact& contact, int id) const {
    cout << "ID: " << id << endl;
    cout << "ФИО: " << contact.lastName.toStdString() << " " << contact.firstName.toStdString();
    if (!contact.patronymic.isEmpty()) {
        cout << " " << contact.patronymic.toStdString();
    }
    cout << endl;
    cout << "Адрес: " << contact.address.toStdString() << endl;
    cout << "Дата рождения: " << contact.dateOfBirth.toString("dd-MM-yyyy").toStdString() << endl;
    cout << "Email: " << contact.email.toStdString() << endl;
    cout << "Телефоны: ";
    for (int i = 0; i < contact.phones.size(); i++) {
        cout << contact.phones[i].toStdString();
        if (i < contact.phones.size() - 1) {
            cout << ", ";
        }
    }
    cout << endl;


}
