#ifndef _TD1_H_
#define _TD1_H_

#include <iostream>
#include <vector>
#include <string>
#include "MRZ.h"


class TD1: public MRZ
{
    private:
        std::string secondOptionalData;
        std::string checkFirstTwoLines;
        std::string checkOptionalData;

    public:
        TD1(std::vector<std::vector<Character>>);
        void extractFields();
        void printMRZFields();
};


#endif