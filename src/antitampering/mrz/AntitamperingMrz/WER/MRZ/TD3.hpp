#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "../Mrz.hpp"

class TD3: public Mrz
{
    private:
        std::string checkOptionalData;
        std::string checkOverallDigit;

    public:
        TD3(std::vector<Fields> mrz);
        void extractFields(std::vector<Fields> mrz);
        void printMRZFields();
};