#pragma once

#include "ocr_server/charactersClustering/CharactersClusteringApi.hpp"
#include <unordered_map>

enum MrzType
{
    td1 = 1,
    td2,
    td3,
    mrva,
    mrvb,
    NONE,
};

struct MrzFields
{
    std::string fieldType;
    std::string mrzDataField;
};

class Mrz
{
    public:
        Mrz() = default;
        virtual std::vector<MrzFields> extractMrzFields(std::vector<Fields> mrz) = 0;
        void printMrzFields(std::vector<MrzFields> mrzFields);

    protected:
        std::string checkDocNum = "";
        std::string checkDateBirth = "";
        std::string checkDateExpireDoc = "";
        std::string optionalData = "";
        bool checkDigitsResult = true;
        virtual bool checkDigits(std::vector<Fields> mrz, std::vector<MrzFields> mrzFields) = 0;
        bool check(std::string field, std::string checkDigit);
        bool checkOverall(std::vector<Fields> mrz, std::string overallDigit);     
};