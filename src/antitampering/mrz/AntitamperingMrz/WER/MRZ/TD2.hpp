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
        TD2(std::vector<Fields> mrz);
        void extractFields(std::vector<Fields> mrz);
        void printMRZFields();     
};