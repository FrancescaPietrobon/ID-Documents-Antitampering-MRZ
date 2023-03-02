#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "../Mrz.hpp"

class MRVA: public Mrz
{
    private:
        bool checkDigits(std::vector<Field> mrz, std::vector<MrzField> mrzFields);
        
    public:
        std::vector<MrzField> extractMrzFields(std::vector<Field> mrz);
};