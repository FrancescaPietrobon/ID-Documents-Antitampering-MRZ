#pragma once

#include "charactersClustering/CharactersClusteringApi.hpp"
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
        std::vector<Fields> findMrzLines(const Fields *fields, const size_t fieldsSize);
        std::vector<MrzFields> extractMrz(std::vector<Fields> mrzLines);
        void printMrzFields(std::vector<MrzFields> mrzFields);

    protected:
        std::string checkDocNum = "";
        std::string checkDateBirth = "";
        std::string checkDateExpireDoc = "";
        std::string optionalData = "";
        bool checkDigitsResult = true;
        static Mrz* createMrz(MrzType type, std::vector<Fields>);
        virtual std::vector<MrzFields> extractMrzFields(std::vector<Fields> mrz) = 0;
        MrzType findMrzType(std::vector<Fields> mrzLines);
        virtual bool checkDigits(std::vector<Fields> mrz, std::vector<MrzFields> mrzFields) = 0;
        bool check(std::string field, std::string checkDigit);
        bool checkOverall(std::vector<Fields> mrz, std::string overallDigit);
        std::unordered_map<char, unsigned> digit_conversion = {{'<', 0,}, {'0', 0,}, {'1', 1,}, {'2', 2,}, {'3', 3,},
                                                               {'4', 4,}, {'5', 5,}, {'6', 6,}, {'7', 7,}, {'8', 8,},
                                                               {'9', 9,}, {'A', 10,}, {'B', 11,}, {'C', 12,}, {'D', 13,},
                                                               {'E', 14,}, {'F', 15,}, {'G', 16,}, {'H', 17,}, {'I', 18,},
                                                               {'J', 19,}, {'K', 20,}, {'L', 21,}, {'M', 22,}, {'N', 23,},
                                                               {'O', 24,}, {'P', 25,}, {'Q', 26,}, {'R', 27,}, {'S', 28,},
                                                               {'T', 29,}, {'U', 30,}, {'V', 31,}, {'W', 32,}, {'X', 33,},
                                                               {'Y', 34,}, {'Z', 35,}};
     
};