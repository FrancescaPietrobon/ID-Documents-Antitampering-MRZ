#pragma once

#include "antitampering/mrz/AntitamperingMrz/AntitamperingMrz.hpp"
#include <unordered_map>
#include <map>

enum MrzType
{
    td1 = 1,
    td2,
    td3,
    mrva,
    mrvb,
    NONE,
};

struct MrzField
{
    std::string fieldType;
    std::string mrzDataField;
};

class Mrz
{
    public:
        Mrz() = default;
        virtual std::vector<MrzField> extractMrzFields(std::vector<Field> mrz) = 0;
        virtual bool checkDigits(std::vector<Field> mrz, std::vector<MrzField> mrzFields) = 0;
        void printMrzFields(std::vector<MrzField> mrzFields);

    protected:
        std::string checkDocNum = "";
        std::string checkDateBirth = "";
        std::string checkDateExpireDoc = "";
        std::string optionalData = "";
        bool check(std::string field, std::string checkDigit);
        bool checkOverall(std::vector<Field> mrz, std::string overallDigit);     
};