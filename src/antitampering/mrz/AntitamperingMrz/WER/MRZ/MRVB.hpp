#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "../Mrz.hpp"

class MRVB: public Mrz
{
    public:
        MrzFields extractMrzFields(std::vector<Fields> mrz);
        void printMrzFields(MrzFields mrzFields);
};