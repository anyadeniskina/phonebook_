#include <iostream>
#include <string>
#include <vector>
#include "PhoneBook.h"
#include "Contact.h"
#include "Validator.h"

// Функция для ввода данных контакта
Contact inputContact() {
    Contact contact;
    string input;

    // Ввод имени
    while (true) {
        cout << "Введите имя: ";
        getline(cin, input);
        input = Validator::trim(input);
        if (Validator::checkName(input)) {
            contact.firstName = input;
            break;
        }
        cout << "Неверное имя! Используйте только буквы, цифры, дефис и пробел." << endl;
    }

    // Ввод фамилии
    while (true) {
        cout << "Введите фамилию: ";
        getline(cin, input);
        input = Validator::trim(input);
        if (Validator::checkName(input)) {
            contact.lastName = input;
            break;
        }
        cout << "Неверная фамилия!" << endl;
    }

    // Ввод отчества (необязательно)
    cout << "Введите отчество (необязательно): ";
    getline(cin, input);
    input = Validator::trim(input);
    if (!input.empty() && Validator::checkName(input)) {
        contact.patronymic = input;
    }

    // Ввод адреса
    cout << "Введите адрес: ";
    getline(cin, contact.address);
    contact.address = Validator::trim(contact.address);

    // Ввод даты рождения в формате ДД-ММ-ГГГГ
    while (true) {
        cout << "Введите дату рождения (ДД-ММ-ГГГГ): ";
        getline(cin, input);
        input = Validator::trim(input);
        if (Validator::checkDate(input)) {
            contact.dateOfBirth = input;
            break;
        }
        cout << "Неверная дата! Формат: ДД-ММ-ГГГГ" << endl;
    }

    // Ввод email
    while (true) {
        cout << "Введите email: ";
        getline(cin, input);
        input = Validator::trim(input);
        if (Validator::checkEmail(input)) {
            contact.email = input;
            break;
        }
        cout << "Неверный email! Пример: user@example.com" << endl;
    }
    cout << "Введите первый номер телефона: ";
    getline(cin, input);
    input = Validator::trim(input);

    string formatted = Validator::formatPhone(input);
    if (Validator::checkPhone(formatted)) {
        contact.phones.push_back(formatted);
        cout << "Основной телефон: " << formatted << endl;
    }
    else {
        cout << "Неверный номер! Контакт не будет добавлен." << endl;
        return contact;
    }

    cout << "Добавить дополнительный телефон? (1-да, 0-нет): ";
    getline(cin, input);
    input = Validator::trim(input);

    while (input == "1" || input == "да") {
        cout << "Введите дополнительный телефон: ";
        getline(cin, input);
        input = Validator::trim(input);

        formatted = Validator::formatPhone(input);
        if (Validator::checkPhone(formatted)) {
            contact.phones.push_back(formatted);
            cout << "Доп. телефон добавлен: " << formatted << endl;
        }
        else {
            cout << "Неверный номер!" << endl;
        }

        cout << "Добавить еще телефон? (1-да, 0-нет): ";
        getline(cin, input);
        input = Validator::trim(input);
    }

    return contact;
}

void showMenu() {
    cout << "\nТЕЛЕФОННАЯ КНИГА " << endl;
    cout << "1. Добавить контакт" << endl;
    cout << "2. Показать все контакты" << endl;
    cout << "3. Найти по имени" << endl;
    cout << "4. Найти по телефону" << endl;
    cout << "5. Удалить контакт" << endl;
    cout << "6. Редактировать контакт" << endl;
    cout << "7. Сохранить в файл" << endl;
    cout << "8. Загрузить из файла" << endl;
    cout << "9. Выйти" << endl;
    cout << "Выберите действие: ";
}

int main() {
    setlocale(LC_ALL, "Rus");
    PhoneBook phoneBook;
    int choice = 0;
    while (choice != 9) {
        showMenu();
        cin >> choice;
        cin.ignore();
        switch (choice) {
        case 1: {
            Contact newContact = inputContact();
            phoneBook.addContact(newContact);
            break;
        }
        case 2:
            phoneBook.showAll();
            break;
        case 3: {
            string name;
            cout << "Введите имя для поиска: ";
            getline(cin, name);
            phoneBook.findByName(name);
            break;
        }
        case 4: {
            string phone;
            cout << "Введите телефон для поиска: ";
            getline(cin, phone);
            phoneBook.findByPhone(phone);
            break;
        }
        case 5: {
            int id;
            cout << "Введите ID контакта для удаления: ";
            cin >> id;
            cin.ignore();
            phoneBook.removeContact(id);
            break;
        }
        case 6: {
            int id;
            cout << "Введите ID контакта для редактирования: ";
            cin >> id;
            cin.ignore();

            if (phoneBook.containsId(id)) {
                cout << "Введите новые данные:" << endl;
                Contact updatedContact = inputContact();
                phoneBook.updateContact(id, updatedContact);
            }
            else {
                cout << "Контакт не найден!" << endl;
            }
            break;
        }
        case 7:
            phoneBook.saveToFile("phonebook.txt");
            break;
        case 8:
            phoneBook.loadFromFile("phonebook.txt");
            break;
        case 9:
            phoneBook.saveToFile("phonebook.txt");
            cout << "До свидания!" << endl;
            break;
        default:
            cout << "Неверный выбор!" << endl;
        }
    }

    return 0;
}