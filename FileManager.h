#pragma once
#include "PhoneBook.h"
#include <string>

class FileManager {
public:
    static bool backupData(const PhoneBook& phoneBook, const std::string& filename);
    static bool restoreFromBackup(PhoneBook& phoneBook, const std::string& filename);
};
