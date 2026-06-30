#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "DatabaseManager.h"

//подключаем заголовки Qt
#include <QTableWidget>//таблица
#include <QPushButton>//кнопки
#include <QLineEdit>//поля ввода
#include <QDateEdit>//поле даты
#include <QVBoxLayout>//вертикальный layout
#include <QHBoxLayout>//горизонтальный layout
#include <QGridLayout>//сеточный layout
#include <QLabel>//метки
#include <QMessageBox>//окна сообщений
#include <QFileDialog>//диалоги файлов
#include <QHeaderView>//заголовки таблицы
#include <QGroupBox>//группировка

MainWindow::MainWindow(QWidget*parent)
    :QMainWindow(parent)
    ,ui(new Ui::MainWindow)
{
    ui->setupUi(this);//настройка UI
    setupUI();//наша настройка

    tableWidget->setColumnCount(7);//7 колонок
    QStringList headers={"ID","Фамилия","Имя","Отчество","Дата рождения","Email","Телефон"};//заголовки
    tableWidget->setHorizontalHeaderLabels(headers);//установка заголовков
    tableWidget->horizontalHeader()->setStretchLastSection(true);//растягиваем последнюю колонку

    //тестовые данные
    phoneBook.addContact(Contact("Иван","Иванов","Иванович",
                                 "Москва",QDate(1990,1,1),
                                 "ivan@mail.ru",
                                 QStringList{"+79161234567","+74951234567"}));//два телефона
    updateTable();//обновляем таблицу
}

MainWindow::~MainWindow()
{
    delete ui;//удаляем UI
}

void MainWindow::setupUI()
{
    QWidget* centralWidget = new QWidget(this);//центральный виджет
    setCentralWidget(centralWidget);//устанавливаем

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);//главный layout

    QGroupBox* inputGroup = new QGroupBox("Добавить контакт", this);//группа для добавления
    QGridLayout* inputLayout = new QGridLayout(inputGroup);//сеточный layout

    //первая колонка
    inputLayout->addWidget(new QLabel("Фамилия:"), 0, 0);//метка фамилии
    lastNameEdit = new QLineEdit(this);//поле фамилии
    inputLayout->addWidget(lastNameEdit, 0, 1);//добавляем поле

    inputLayout->addWidget(new QLabel("Имя:"), 1, 0);//метка имени
    firstNameEdit = new QLineEdit(this);//поле имени
    inputLayout->addWidget(firstNameEdit, 1, 1);//добавляем поле

    inputLayout->addWidget(new QLabel("Отчество:"), 2, 0);//метка отчества
    patronymicEdit = new QLineEdit(this);//поле отчества
    inputLayout->addWidget(patronymicEdit, 2, 1);//добавляем поле

    inputLayout->addWidget(new QLabel("Дата рождения:"), 3, 0);//метка даты
    dateEdit = new QDateEdit(QDate::currentDate(), this);//поле даты
    dateEdit->setDisplayFormat("dd-MM-yyyy");//формат
    inputLayout->addWidget(dateEdit, 3, 1);//добавляем поле

    //вторая колонка
    inputLayout->addWidget(new QLabel("Email:"), 0, 2);//метка email
    emailEdit = new QLineEdit(this);//поле email
    inputLayout->addWidget(emailEdit, 0, 3);//добавляем поле

    inputLayout->addWidget(new QLabel("Адрес:"), 1, 2);//метка адреса
    addressEdit = new QLineEdit(this);//поле адреса
    inputLayout->addWidget(addressEdit, 1, 3);//добавляем поле

    //группа телефонов
    QGroupBox* phoneGroup = new QGroupBox("Телефоны", this);//группа телефонов
    QVBoxLayout* phoneMainLayout = new QVBoxLayout(phoneGroup);//layout группы

    //первое поле телефона
    QLineEdit* phoneEdit1 = new QLineEdit(this);//поле телефона
    phoneEdit1->setPlaceholderText("+79161234567");//подсказка
    phoneMainLayout->addWidget(new QLabel("Телефон 1:"));//метка
    phoneMainLayout->addWidget(phoneEdit1);//добавляем поле
    phoneEdits.append(phoneEdit1);//добавляем в список

    phoneLayout = new QVBoxLayout();//layout для дополнительных телефонов
    phoneMainLayout->addLayout(phoneLayout);//добавляем layout

    //кнопки управления телефонами
    QHBoxLayout* phoneButtonsLayout = new QHBoxLayout();//layout для кнопок
    addPhoneButton = new QPushButton("+ Добавить телефон", this);//кнопка добавить
    removePhoneButton = new QPushButton("- Удалить телефон", this);//кнопка удалить
    removePhoneButton->setEnabled(false);//неактивна пока один телефон
    phoneButtonsLayout->addWidget(addPhoneButton);//добавляем кнопку
    phoneButtonsLayout->addWidget(removePhoneButton);//добавляем кнопку
    phoneButtonsLayout->addStretch();//растягиваем
    phoneMainLayout->addLayout(phoneButtonsLayout);//добавляем layout кнопок

    inputLayout->addWidget(phoneGroup, 2, 2, 2, 2);//добавляем группу телефонов

    addButton = new QPushButton("Добавить контакт", this);//кнопка добавления
    inputLayout->addWidget(addButton, 4, 0, 1, 4);//добавляем кнопку

    mainLayout->addWidget(inputGroup);//добавляем группу ввода

    //блок поиска
    QHBoxLayout* searchLayout = new QHBoxLayout();//layout поиска
    searchLayout->addWidget(new QLabel("Поиск:"));//метка поиска
    searchEdit = new QLineEdit(this);//поле поиска
    searchLayout->addWidget(searchEdit);//добавляем поле
    searchButton = new QPushButton("Найти", this);//кнопка поиска
    searchLayout->addWidget(searchButton);//добавляем кнопку
    removeButton = new QPushButton("Удалить выбранное", this);//кнопка удаления
    searchLayout->addWidget(removeButton);//добавляем кнопку
    searchLayout->addStretch();//растягиваем
    mainLayout->addLayout(searchLayout);//добавляем layout поиска

    tableWidget = new QTableWidget(this);//создаем таблицу
    mainLayout->addWidget(tableWidget);//добавляем таблицу

    //кнопки управления
    QHBoxLayout* buttonLayout = new QHBoxLayout();//layout кнопок
    showAllButton = new QPushButton("Обновить таблицу", this);//кнопка обновления
    saveButton = new QPushButton("Сохранить в файл", this);//кнопка сохранения
    loadButton = new QPushButton("Загрузить из файла", this);//кнопка загрузки

    //НОВЫЕ КНОПКИ ДЛЯ БАЗЫ ДАННЫХ
    saveToDbButton = new QPushButton("Сохранить в БД", this);
    loadFromDbButton = new QPushButton("Загрузить из БД", this);

    buttonLayout->addWidget(showAllButton);//добавляем кнопку
    buttonLayout->addWidget(saveButton);//добавляем кнопку
    buttonLayout->addWidget(loadButton);//добавляем кнопку
    buttonLayout->addWidget(saveToDbButton);//добавляем новую кнопку
    buttonLayout->addWidget(loadFromDbButton);//добавляем новую кнопку
    buttonLayout->addStretch();//растягиваем
    mainLayout->addLayout(buttonLayout);//добавляем layout кнопок

    //подключение сигналов
    connect(addButton, &QPushButton::clicked, this, &MainWindow::onAddButtonClicked);//добавить контакт
    connect(showAllButton, &QPushButton::clicked, this, &MainWindow::onShowAllButtonClicked);//обновить таблицу
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::onSaveButtonClicked);//сохранить
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::onLoadButtonClicked);//загрузить
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::onSearchButtonClicked);//найти
    connect(removeButton, &QPushButton::clicked, this, &MainWindow::onRemoveButtonClicked);//удалить
    connect(addPhoneButton, &QPushButton::clicked, this, &MainWindow::onAddPhoneClicked);//добавить телефон
    connect(removePhoneButton, &QPushButton::clicked, this, &MainWindow::onRemovePhoneClicked);//удалить телефон

    //ПОДКЛЮЧЕНИЕ НОВЫХ СИГНАЛОВ ДЛЯ БД
    connect(saveToDbButton, &QPushButton::clicked, this, &MainWindow::onSaveToDbButtonClicked);
    connect(loadFromDbButton, &QPushButton::clicked, this, &MainWindow::onLoadFromDbButtonClicked);
}

void MainWindow::onAddPhoneClicked()//добавить поле телефона
{
    int phoneCount=phoneEdits.size()+1;//номер нового телефона
    QLineEdit*newPhoneEdit=new QLineEdit(this);//новое поле
    newPhoneEdit->setPlaceholderText("+79161234567");//подсказка
    phoneLayout->addWidget(new QLabel(QString("Телефон %1:").arg(phoneCount)));//метка
    phoneLayout->addWidget(newPhoneEdit);//добавляем поле
    phoneEdits.append(newPhoneEdit);//добавляем в список
    removePhoneButton->setEnabled(true);//активируем кнопку удаления
}

void MainWindow::onRemovePhoneClicked()//удалить поле телефона
{
    if(phoneEdits.size()>1){//нельзя удалить последнее поле
        QLineEdit*lastEdit=phoneEdits.takeLast();//берем последнее поле
        QLayoutItem*labelItem=phoneLayout->takeAt(phoneLayout->count()-1);//метка
        QLayoutItem*fieldItem=phoneLayout->takeAt(phoneLayout->count()-1);//поле
        if(labelItem&&labelItem->widget())labelItem->widget()->deleteLater();//удаляем метку
        if(fieldItem&&fieldItem->widget())fieldItem->widget()->deleteLater();//удаляем поле
        delete labelItem;//удаляем item
        delete fieldItem;//удаляем item
        lastEdit->deleteLater();//удаляем поле
        removePhoneButton->setEnabled(phoneEdits.size()>1);//обновляем состояние кнопки
    }
}

void MainWindow::onAddButtonClicked()//добавить контакт
{
    if(firstNameEdit->text().isEmpty()||lastNameEdit->text().isEmpty()){//проверка обязательных полей
        QMessageBox::warning(this,"Ошибка","Заполните обязательные поля (Имя и Фамилия)");//сообщение
        return;//выход
    }

    QStringList phones;//список телефонов
    for(QLineEdit*phoneEdit:phoneEdits){//перебор всех полей телефонов
        QString phone=phoneEdit->text().trimmed();//получаем телефон
        if(!phone.isEmpty())phones.append(phone);//добавляем если не пустой
    }
QString dateStr = dateEdit->date().toString("dd-MM-yyyy");
   if (!Validator::checkDate(dateStr)) {
       QMessageBox::warning(this, "Ошибка",
           "Неверная дата рождения!\n\n"
           "Возможные причины:\n"
           "• Дата не может быть в будущем\n"
           "• Некорректный день для месяца (например, 31 февраля)\n"
           "• Неверный формат даты\n\n"
           "Текущая дата: " + QDate::currentDate().toString("dd.MM.yyyy"));
       dateEdit->setFocus();
       return;
   }
    if (!Validator::checkEmail(emailEdit->text())) {
            QMessageBox::warning(this, "Ошибка",
                "Неверный email адрес\nПример правильного формата: user@example.com");
            emailEdit->setFocus(); // Ставим курсор в поле email
            return;
    }


for(QLineEdit* phoneEdit:phoneEdits){
       QString phone=phoneEdit->text().trimmed();
       if(!phone.isEmpty()) {
           // Проверяем формат телефона
           if (!Validator::checkPhone(phone)) {
               QString errorMessage =
                   "Неверный формат телефона: " + phone + "\n\n"
                   "Правильный формат:\n"
                   "• +79161234567 (с плюсом)\n"
                   "• 89161234567 (с восьмерки)\n"
                   "• 79161234567 (с семерки)\n\n"
                   "Номер должен содержать 11 цифр после кода страны";

               QMessageBox::warning(this, "Ошибка ввода телефона", errorMessage);
               phoneEdit->setFocus(); // Ставим курсор в поле с ошибкой
               phoneEdit->selectAll(); // Выделяем текст для удобства исправления
               return;
           }
           phones.append(phone);
       }
   }
    if(phones.isEmpty()){//проверка наличия телефонов
        QMessageBox::warning(this,"Ошибка","Введите хотя бы один телефон");//сообщение
        return;//выход
    }

    Contact contact(firstNameEdit->text(),lastNameEdit->text(),patronymicEdit->text(),
                   addressEdit->text(),dateEdit->date(),emailEdit->text(),
                   phones);//создаем контакт

    if(phoneBook.addContact(contact)){//добавляем контакт
        //очищаем поля
        firstNameEdit->clear();
        lastNameEdit->clear();
        patronymicEdit->clear();
        addressEdit->clear();
        emailEdit->clear();
        dateEdit->setDate(QDate::currentDate());//сбрасываем дату

        //очищаем все поля телефонов
        for(QLineEdit*phoneEdit:phoneEdits)phoneEdit->clear();

        //оставляем только первое поле телефона
        while(phoneEdits.size()>1)onRemovePhoneClicked();

        updateTable();//обновляем таблицу
        QMessageBox::information(this,"Успех","Контакт добавлен");//сообщение
    }
}

void MainWindow::onShowAllButtonClicked()//обновить таблицу
{
    updateTable();//обновляем таблицу
}

void MainWindow::onSaveButtonClicked()//сохранить
{
    QString filename=QFileDialog::getSaveFileName(this,"Сохранить файл","","PhoneBook Files (*.pbk);;All Files (*)");//диалог
    if(!filename.isEmpty()){//если выбрали файл
        if(phoneBook.saveToFile(filename)){//сохраняем
            QMessageBox::information(this,"Успех","Данные сохранены в файл: "+filename);//сообщение
        }
    }
}

void MainWindow::onLoadButtonClicked()//загрузить
{
    QString filename=QFileDialog::getOpenFileName(this,"Загрузить файл","","PhoneBook Files (*.pbk);;All Files (*)");//диалог
    if(!filename.isEmpty()){//если выбрали файл
        if(phoneBook.loadFromFile(filename)){//загружаем
            updateTable();//обновляем таблицу
            QMessageBox::information(this,"Успех","Данные загружены из файла: "+filename);//сообщение
        }
    }
}

void MainWindow::onSearchButtonClicked()//поиск
{
    QString searchText=searchEdit->text();//текст поиска
    if(searchText.isEmpty()){//если пусто
        updateTable();//показать все
        return;//выход
    }

    for(int row=0;row<tableWidget->rowCount();++row){//перебор всех строк
        bool match=false;//флаг совпадения
        for(int col=0;col<tableWidget->columnCount();++col){//перебор всех колонок
            QTableWidgetItem*item=tableWidget->item(row,col);//ячейка
            if(item&&item->text().contains(searchText,Qt::CaseInsensitive)){//поиск
                match=true;//нашли
                break;//выход
            }
        }
        tableWidget->setRowHidden(row,!match);//скрыть/показать строку
    }
}

void MainWindow::onRemoveButtonClicked()//удалить выбранное
{
    int currentRow=tableWidget->currentRow();//текущая строка
    if(currentRow>=0){//если что-то выбрано
        QTableWidgetItem*idItem=tableWidget->item(currentRow,0);//ячейка с ID
        if(idItem){//если есть ячейка
            int id=idItem->text().toInt();//получаем ID
            if(phoneBook.removeContact(id)){//удаляем
                updateTable();//обновляем таблицу
                QMessageBox::information(this,"Успех","Контакт удален");//сообщение
            }
        }
    }else{//ничего не выбрано
        QMessageBox::warning(this,"Ошибка","Выберите контакт для удаления");//сообщение
    }
}


void MainWindow::testDatabaseConnection()
{
    DatabaseManager& db = DatabaseManager::instance();

    if (db.connectToDatabase()) {
        QMessageBox::information(this, "Успех",
        "Подключение к базе данных успешно установлено!");
    } else {
        QMessageBox::warning(this, "Ошибка",
        "Не удалось подключиться к базе данных.\nОшибка: " + db.lastError());
    }
}

void MainWindow::updateTable()//обновить таблицу
{
    tableWidget->setRowCount(0);//очищаем таблицу
    auto contacts=phoneBook.getAllContacts();//получаем все контакты
    for(auto it=contacts.begin();it!=contacts.end();++it){//перебор контактов
        int row=tableWidget->rowCount();//текущее количество строк
        tableWidget->insertRow(row);//добавляем строку
        Contact c=it.value();//получаем контакт
        tableWidget->setItem(row,0,new QTableWidgetItem(QString::number(it.key())));//ID
        tableWidget->setItem(row,1,new QTableWidgetItem(c.lastName));//фамилия
        tableWidget->setItem(row,2,new QTableWidgetItem(c.firstName));//имя
        tableWidget->setItem(row,3,new QTableWidgetItem(c.patronymic));//отчество
        tableWidget->setItem(row,4,new QTableWidgetItem(c.dateOfBirth.toString("dd-MM-yyyy")));//дата
        tableWidget->setItem(row,5,new QTableWidgetItem(c.email));//email
        tableWidget->setItem(row,6,new QTableWidgetItem(c.phones.join(", ")));//телефоны
    }

    for(int row=0;row<tableWidget->rowCount();++row){//показываем все строки
        tableWidget->setRowHidden(row,false);
    }

}
void MainWindow::onSaveToDbButtonClicked()
{
    if (phoneBook.saveToDatabase()) {
        QMessageBox::information(this, "Успех", "Данные сохранены в базу данных");
    } else {
        QMessageBox::warning(this, "Ошибка",
            "Не удалось сохранить данные в базу данных");
    }
}

void MainWindow::onLoadFromDbButtonClicked()
{
    if (phoneBook.loadFromDatabase()) {
        updateTable();
        QMessageBox::information(this, "Успех", "Данные загружены из базы данных");
    } else {
        QMessageBox::warning(this, "Ошибка",
            "Не удалось загрузить данные из базы данных");
    }
}
