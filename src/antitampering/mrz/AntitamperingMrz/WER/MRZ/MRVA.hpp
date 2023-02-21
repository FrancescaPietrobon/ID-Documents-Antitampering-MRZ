#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "../Mrz.hpp"

class MRVA: public Mrz
{
    private:

    public:
        MRVA(std::vector<Fields> mrz);
        void extractFields(std::vector<Fields> mrz);
        void extractFields();
        void printMRZFields();
};