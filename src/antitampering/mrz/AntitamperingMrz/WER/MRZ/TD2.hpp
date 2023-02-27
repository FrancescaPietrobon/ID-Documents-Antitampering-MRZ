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
        bool checkDigits(std::vector<Fields> mrz, std::vector<MrzFields> mrzFields);

    public:
        std::vector<MrzFields> extractMrzFields(std::vector<Fields> mrz);
};