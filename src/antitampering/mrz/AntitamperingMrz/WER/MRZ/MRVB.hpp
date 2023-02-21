#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "../Mrz.hpp"

class MRVB: public Mrz
{
    private:

    public:
        MRVB(std::vector<Fields> mrz);
        void extractFields(std::vector<Fields> mrz);
        void printMRZFields();
};