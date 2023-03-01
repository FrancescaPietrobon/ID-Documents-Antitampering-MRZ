#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "../Mrz.hpp"

class MRVB: public Mrz
{
    private:
        bool checkDigits(std::vector<Fields> mrz, std::vector<MrzFields> mrzFields);
        
    public:
        std::vector<MrzFields> extractMrzFields(std::vector<Fields> mrz);
};