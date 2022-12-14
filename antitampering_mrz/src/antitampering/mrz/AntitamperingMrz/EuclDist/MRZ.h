#ifndef _MRZ_H_
#define _MRZ_H_

#include <iostream>
#include <vector>
#include <string>
#include "Character.h"
/*
#include "MRZ/TD1.h"
#include "MRZ/TD2.h"
#include "MRZ/TD3.h"
#include "MRZ/MRVA.h"
#include "MRZ/MRVB.h"
*/

enum MRZType
{
    td1,
    td2,
    td3,
    mrva,
    mrvb,
    NONE
};

struct detection
{
    std::string typeFieldMRZ;
    std::string fieldMRZ;
};

class MRZ
{
    protected:
        std::vector<std::vector<Character>> mrz;
        std::vector<detection> allFields;
        std::string docType;
        std::string state;
        std::string surname;
        std::string name;
        std::string docNumber;
        std::string checkDocNum;
        std::string nationality;
        std::string dateBirth;
        std::string checkDateBirth;
        std::string sex;
        std::string dateExpireDoc;
        std::string checkDateExpireDoc;
        std::string optionalData;
        bool check(std::string field, std::string checkDigit);
        bool checkOverall(std::string overallDigit); 

    public:
        MRZ(std::vector<std::vector<Character>>);
        static MRZ* createMRZ(MRZType type, std::vector<std::vector<Character>>);
        MRZ() = default;
        void printMRZ();
        void printMRZFields();
        std::vector<detection> getAllFields();
        virtual void extractFields() = 0;
};

#endif