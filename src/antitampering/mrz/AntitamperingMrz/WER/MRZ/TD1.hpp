#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "../Mrz.hpp"

class TD1: public Mrz
{
    private:
        std::string secondOptionalData = "";
        std::string checkFirstTwoLines = "";
        std::string checkOptionalData = "";
        bool checkDigits(std::vector<Field> mrz, std::vector<MrzField> mrzFields);

    public:
        std::vector<MrzField> extractMrzFields(std::vector<Field> mrz);
};