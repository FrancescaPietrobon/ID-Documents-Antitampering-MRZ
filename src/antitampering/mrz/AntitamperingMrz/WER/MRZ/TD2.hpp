#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "../Mrz.hpp"

class TD2: public Mrz
{
    private:
        std::string checkOptionalData;
        std::string checkOverallDigit;

    public:
        MrzFields extractMrzFields(std::vector<Fields> mrz);
        void printMrzFields(MrzFields mrzFields);     
};