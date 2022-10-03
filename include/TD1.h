#ifndef TD1_H
#define TD1_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "MRZ.h"


class TD1: public MRZ
{
    private:
        std::string secondOptionalData;
        std::string checkFirstTwoLines;
        std::string checkOptionalData;
    public:
        TD1(std::vector<std::vector<Character>>, int);
        void extractFields();
};


#endif