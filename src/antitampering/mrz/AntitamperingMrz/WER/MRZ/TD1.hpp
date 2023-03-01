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
        bool checkDigits(std::vector<Fields> mrz, std::vector<MrzFields> mrzFields);

    public:
        std::vector<MrzFields> extractMrzFields(std::vector<Fields> mrz);
};