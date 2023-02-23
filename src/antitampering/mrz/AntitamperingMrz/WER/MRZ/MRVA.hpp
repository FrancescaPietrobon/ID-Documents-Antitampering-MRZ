#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "../Mrz.hpp"

class MRVA: public Mrz
{
    public:
        MrzFields extractMrzFields(std::vector<Fields> mrz);
        void printMrzFields(MrzFields mrzFields);
};