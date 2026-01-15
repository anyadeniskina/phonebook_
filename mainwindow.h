#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>//главное окно
#include "PhoneBook.h"//телефонная книга

//предварительные объявления
class QTableWidget;
class QPushButton;
class QLineEdit;
class QDateEdit;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QGroupBox;

QT_BEGIN_NAMESPACE
namespace Ui{class MainWindow;}
QT_END_NAMESPACE

class MainWindow:public QMainWindow
{
    Q_OBJECT//для сигналов и слотов

public:
    MainWindow(QWidget*parent=nullptr);//конструктор
    ~MainWindow();//деструктор

private slots://слоты
    void onAddButtonClicked();//добавить контакт
    void onShowAllButtonClicked();//обновить таблицу
    void onSaveButtonClicked();//сохранить
    void onLoadButtonClicked();//загрузить
    void onSearchButtonClicked();//найти
    void onRemoveButtonClicked();//удалить
    void onAddPhoneClicked();//добавить поле телефона
    void onRemovePhoneClicked();//удалить поле телефона
    void updateTable();//обновить таблицу

    //Новые слоты для БД
    void onSaveToDbButtonClicked();
    void onLoadFromDbButtonClicked();

private:
    Ui::MainWindow*ui;//UI форма
    PhoneBook phoneBook;//телефонная книга

    //виджеты
    QTableWidget*tableWidget;//таблица
    QPushButton*addButton;//добавить контакт
    QPushButton*showAllButton;//обновить таблицу
    QPushButton*saveButton;//сохранить
    QPushButton*loadButton;//загрузить
    QPushButton*searchButton;//найти
    QPushButton*removeButton;//удалить
    // Новые кнопки для БД
    QPushButton* saveToDbButton;
    QPushButton* loadFromDbButton;

    //поля ввода
    QLineEdit*firstNameEdit;//имя
    QLineEdit*lastNameEdit;//фамилия
    QLineEdit*patronymicEdit;//отчество
    QLineEdit*addressEdit;//адрес
    QDateEdit*dateEdit;//дата рождения
    QLineEdit*emailEdit;//email
    QLineEdit*searchEdit;//поиск

    //для нескольких телефонов
    QList<QLineEdit*>phoneEdits;//список полей телефонов
    QPushButton*addPhoneButton;//кнопка добавить телефон
    QPushButton*removePhoneButton;//кнопка удалить телефон
    QVBoxLayout*phoneLayout;//layout для телефонов
    QPushButton* testDbButton;  // Кнопка теста БД
    void testDatabaseConnection();  // Метод теста
    void setupUI();//настройка интерфейса
};

#endif // MAINWINDOW_H
