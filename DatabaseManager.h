#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include "Contact.h"
#include <QMap>

class DatabaseManager : public QObject
{
    Q_OBJECT

public:
    static DatabaseManager& instance();

    bool connectToDatabase(const QString& host = "localhost",
                          int port = 5432,
                          const QString& databaseName = "phonebook_db",
                          const QString& userName = "postgres",
                          const QString& password = "111");
    bool isConnected() const;
    QString lastError() const;

    // Основные операции
    bool saveContact(const Contact& contact);
    bool updateContact(int id, const Contact& contact);
    bool deleteContact(int id);
    QMap<int, Contact> loadAllContacts();

    // Утилиты
    bool createTables();
    void disconnect();

private:
    DatabaseManager(QObject* parent = nullptr);
    ~DatabaseManager();

    QSqlDatabase m_database;
    QString m_lastError;
    static DatabaseManager* m_instance;
};

#endif // DATABASEMANAGER_H
