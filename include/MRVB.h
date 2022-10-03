#ifndef MRVB_H
#define MRVB_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "MRZ.h"


class MRVB: public MRZ
{
    private:

    public:
        MRVB(std::vector<std::vector<Character>>, int);
        void extractFields();
};

#endif