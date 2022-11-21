#ifndef TD3_H
#define TD3_H

#include <iostream>
#include <vector>
#include <string>
#include "../MRZ.h"


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