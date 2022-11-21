#ifndef _MRVB_H_
#define _MRVB_H_

#include <iostream>
#include <vector>
#include <string>
#include "../MRZ.h"


class MRVB: public MRZ
{
    private:

    public:
        MRVB(std::vector<std::vector<Character>>);
        void extractFields();
        void printMRZFields();
};

#endif