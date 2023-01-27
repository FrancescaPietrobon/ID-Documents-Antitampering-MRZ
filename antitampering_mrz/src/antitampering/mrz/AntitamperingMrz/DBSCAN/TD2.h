#ifndef _TD2_H_
#define _TD2_H_

#include <iostream>
#include <vector>
#include <string>
#include "MRZ.h"


class TD2: public MRZ
{
    private:
        std::string checkOptionalData;
        std::string checkOverallDigit;

    public:
        TD2(std::vector<std::vector<Character>>);
        void extractFields();
        void printMRZFields();     
};


#endif