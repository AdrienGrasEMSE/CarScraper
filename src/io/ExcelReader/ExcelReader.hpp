#pragma once
#include <string>
#include <vector>
#include "../core/models/Car.hpp"

class ExcelReader {

    private:
        std::string m_filePath;

    public:
        explicit ExcelReader(const std::string& filePath);
        
};