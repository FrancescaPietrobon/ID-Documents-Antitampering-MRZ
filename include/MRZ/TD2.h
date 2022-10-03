#ifndef TD2_H
#define TD2_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "../MRZ.h"


class TD2: public MRZ
{
    private:

    public:
        TD2(std::vector<std::vector<Character>>, int);
        void extractFields();
        std::string checkOptionalData;
        std::string checkOverall;

};


#endif