#ifndef TD2_H
#define TD2_H

#include <iostream>
#include <vector>
#include <string>
#include "../MRZ.h"


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