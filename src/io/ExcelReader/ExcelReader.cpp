#include "ExcelReader.hpp"
#include <OpenXLSX.hpp>
#include <stdexcept>

ExcelReader::ExcelReader(const std::string& filePath)
    : m_filePath(filePath) {}

std::vector<Car> ExcelReader::read() {
    OpenXLSX::XLDocument doc;
    doc.open(m_filePath);

    auto wks = doc.workbook().worksheet("Sheet1"); // nom de l'onglet
    std::vector<Car> cars;

    // Ligne 1 = en-têtes → on commence à la ligne 2
    for (int row = 2; row <= wks.rowCount(); ++row) {
        std::string brand = wks.cell(row, 1).value().get<std::string>();
        std::string model = wks.cell(row, 2).value().get<std::string>();
        int year         = wks.cell(row, 3).value().get<int>();
        double price     = wks.cell(row, 4).value().get<double>();

        if (brand.empty()) break; // ligne vide = fin des données

        cars.emplace_back(brand, model, year, price);
    }

    doc.close();
    return cars;
}