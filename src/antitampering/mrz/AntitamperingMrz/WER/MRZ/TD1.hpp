#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "../Mrz.hpp"

class TD1: public Mrz
{
    private:
        std::string secondOptionalData;
        std::string checkFirstTwoLines;
        std::string checkOptionalData;

    public:
        MrzFields extractMrzFields(std::vector<Fields> mrz);
        void printMrzFields(MrzFields mrzFields);
};