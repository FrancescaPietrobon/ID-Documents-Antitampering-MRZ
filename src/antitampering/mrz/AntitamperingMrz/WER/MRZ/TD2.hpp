#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "../Mrz.hpp"

class TD2: public Mrz
{
    private:
        std::string checkOptionalData = "";
        std::string checkOverallDigit = "";
        bool checkDigits(std::vector<Field> mrz, std::vector<MrzField> mrzFields);

    public:
        std::vector<MrzField> extractMrzFields(std::vector<Field> mrz);
};