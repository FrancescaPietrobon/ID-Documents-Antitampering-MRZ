#ifndef _TD3_H_
#define _TD3_H_

#include <iostream>
#include <vector>
#include <string>
#include "MRZ.h"


class TD3: public MRZ
{
    private:
        std::string checkOptionalData;
        std::string checkOverallDigit;

    public:
        TD3(std::vector<std::vector<Character>>);
        void extractFields();
        void printMRZFields();
};


#endif