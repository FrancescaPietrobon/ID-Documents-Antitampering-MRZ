#ifndef TD3_H
#define TD3_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "MRZ.h"


class TD3: public MRZ
{
    private:
        std::string getCheckOptionalData();
        std::string checkOverall;
        std::string checkOptionalData;

    public:
        TD3(std::vector<std::vector<Character>>, int);
        void extractFields();
        void printMRZFields();
};


#endif