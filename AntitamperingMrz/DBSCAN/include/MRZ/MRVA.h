#ifndef MRVA_H
#define MRVA_H

#include <iostream>
#include <vector>
#include <string>
#include "../MRZ.h"


class MRVA: public MRZ
{
    private:

    public:
        MRVA(std::vector<std::vector<Character>>);
        void extractFields();
        void printMRZFields();
};

#endif