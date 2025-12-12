#include "FileManager.h"
#include <iostream>
#include <fstream>

bool FileManager::backupData(const PhoneBook& phoneBook, const std::string& filename) {
    return phoneBook.saveToFile(filename + ".backup");
}

bool FileManager::restoreFromBackup(PhoneBook& phoneBook, const std::string& filename) {
    return phoneBook.loadFromFile(filename + ".backup");
}