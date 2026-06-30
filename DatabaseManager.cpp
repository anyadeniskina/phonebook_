#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QApplication>
#include <iostream>

DatabaseManager* DatabaseManager::m_instance = nullptr;

DatabaseManager::DatabaseManager(QObject* parent)
    : QObject(parent)
{
    // Конструктор
}

DatabaseManager::~DatabaseManager()
{
    if (m_database.isOpen()) {
        m_database.close();
    }
}

DatabaseManager& DatabaseManager::instance()
{
    if (!m_instance) {
        m_instance = new DatabaseManager(qApp);
    }
    return *m_instance;
}

bool DatabaseManager::connectToDatabase(const QString& host, int port,
                                       const QString& databaseName,
                                       const QString& userName,
                                       const QString& password)
{
    m_database = QSqlDatabase::addDatabase("QPSQL", "phonebook_connection");

    m_database.setHostName(host);
    m_database.setPort(port);
    m_database.setDatabaseName(databaseName);
    m_database.setUserName(userName);
    m_database.setPassword(password);

    if (!m_database.open()) {
        m_lastError = m_database.lastError().text();
        std::cout << "Database connection error: " << m_lastError.toStdString() << std::endl;
        return false;
    }

    std::cout << "Connected to database successfully!" << std::endl;
    createTables();
    return true;
}

bool DatabaseManager::isConnected() const
{
    return m_database.isOpen();
}

QString DatabaseManager::lastError() const
{
    return m_lastError;
}

bool DatabaseManager::createTables()
{
    QSqlQuery query(m_database);

    // Таблица контактов
    QString createContactsTable =
        "CREATE TABLE IF NOT EXISTS contacts ("
        "id SERIAL PRIMARY KEY, "
        "first_name VARCHAR(100) NOT NULL, "
        "last_name VARCHAR(100) NOT NULL, "
        "patronymic VARCHAR(100), "
        "address TEXT, "
        "date_of_birth DATE, "
        "email VARCHAR(255), "
        "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
        ")";

    if (!query.exec(createContactsTable)) {
        m_lastError = query.lastError().text();
        return false;
    }

    // Таблица телефонов
    QString createPhonesTable =
        "CREATE TABLE IF NOT EXISTS contact_phones ("
        "id SERIAL PRIMARY KEY, "
        "contact_id INTEGER REFERENCES contacts(id) ON DELETE CASCADE, "
        "phone_number VARCHAR(20) NOT NULL"
        ")";

    if (!query.exec(createPhonesTable)) {
        m_lastError = query.lastError().text();
        return false;
    }

    std::cout << "Tables created successfully!" << std::endl;
    return true;
}

bool DatabaseManager::saveContact(const Contact& contact)
{
    if (!isConnected()) {
        m_lastError = "Database not connected";
        return false;
    }

    QSqlQuery query(m_database);
    m_database.transaction();

    // Вставка контакта
    query.prepare(
        "INSERT INTO contacts (first_name, last_name, patronymic, "
        "address, date_of_birth, email) "
        "VALUES (:first_name, :last_name, :patronymic, "
        ":address, :date_of_birth, :email) "
        "RETURNING id"
    );

    query.bindValue(":first_name", contact.firstName);
    query.bindValue(":last_name", contact.lastName);
    query.bindValue(":patronymic", contact.patronymic);
    query.bindValue(":address", contact.address);
    query.bindValue(":date_of_birth", contact.dateOfBirth);
    query.bindValue(":email", contact.email);

    if (!query.exec() || !query.next()) {
        m_lastError = query.lastError().text();
        m_database.rollback();
        return false;
    }

    int contactId = query.value(0).toInt();

    // Вставка телефонов
    for (const QString& phone : contact.phones) {
        query.prepare(
            "INSERT INTO contact_phones (contact_id, phone_number) "
            "VALUES (:contact_id, :phone_number)"
        );
        query.bindValue(":contact_id", contactId);
        query.bindValue(":phone_number", phone);

        if (!query.exec()) {
            m_lastError = query.lastError().text();
            m_database.rollback();
            return false;
        }
    }

    m_database.commit();
    std::cout << "Contact saved to database with ID: " << contactId << std::endl;
    return true;
}

bool DatabaseManager::updateContact(int id, const Contact& contact)
{
    if (!isConnected()) {
        m_lastError = "Database not connected";
        return false;
    }

    QSqlQuery query(m_database);
    m_database.transaction();

    // Обновление контакта
    query.prepare(
        "UPDATE contacts SET "
        "first_name = :first_name, "
        "last_name = :last_name, "
        "patronymic = :patronymic, "
        "address = :address, "
        "date_of_birth = :date_of_birth, "
        "email = :email "
        "WHERE id = :id"
    );

    query.bindValue(":id", id);
    query.bindValue(":first_name", contact.firstName);
    query.bindValue(":last_name", contact.lastName);
    query.bindValue(":patronymic", contact.patronymic);
    query.bindValue(":address", contact.address);
    query.bindValue(":date_of_birth", contact.dateOfBirth);
    query.bindValue(":email", contact.email);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        m_database.rollback();
        return false;
    }

    // Удаление старых телефонов
    query.prepare("DELETE FROM contact_phones WHERE contact_id = :contact_id");
    query.bindValue(":contact_id", id);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        m_database.rollback();
        return false;
    }

    // Вставка новых телефонов
    for (const QString& phone : contact.phones) {
        query.prepare(
            "INSERT INTO contact_phones (contact_id, phone_number) "
            "VALUES (:contact_id, :phone_number)"
        );
        query.bindValue(":contact_id", id);
        query.bindValue(":phone_number", phone);

        if (!query.exec()) {
            m_lastError = query.lastError().text();
            m_database.rollback();
            return false;
        }
    }

    m_database.commit();
    std::cout << "Contact updated in database with ID: " << id << std::endl;
    return true;
}

bool DatabaseManager::deleteContact(int id)
{
    if (!isConnected()) {
        m_lastError = "Database not connected";
        return false;
    }

    QSqlQuery query(m_database);
    query.prepare("DELETE FROM contacts WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }

    std::cout << "Contact deleted from database with ID: " << id << std::endl;
    return true;
}

QMap<int, Contact> DatabaseManager::loadAllContacts()
{
    QMap<int, Contact> contacts;

    if (!isConnected()) {
        m_lastError = "Database not connected";
        return contacts;
    }

    QSqlQuery query(m_database);

    // Загрузка всех контактов
    query.exec(
        "SELECT id, first_name, last_name, patronymic, "
        "address, date_of_birth, email "
        "FROM contacts "
        "ORDER BY last_name, first_name"
    );

    while (query.next()) {
        Contact contact;
        int id = query.value(0).toInt();

        contact.firstName = query.value(1).toString();
        contact.lastName = query.value(2).toString();
        contact.patronymic = query.value(3).toString();
        contact.address = query.value(4).toString();
        contact.dateOfBirth = query.value(5).toDate();
        contact.email = query.value(6).toString();

        // Загрузка телефонов для этого контакта
        QSqlQuery phoneQuery(m_database);
        phoneQuery.prepare(
            "SELECT phone_number FROM contact_phones "
            "WHERE contact_id = :contact_id"
        );
        phoneQuery.bindValue(":contact_id", id);

        if (phoneQuery.exec()) {
            QStringList phones;
            while (phoneQuery.next()) {
                phones.append(phoneQuery.value(0).toString());
            }
            contact.phones = phones;
        }

        contacts[id] = contact;
    }

    std::cout << "Loaded " << contacts.size() << " contacts from database" << std::endl;
    return contacts;
}

void DatabaseManager::disconnect()
{
    if (m_database.isOpen()) {
        m_database.close();
    }
}
