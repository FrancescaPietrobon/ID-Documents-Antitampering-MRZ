#pragma once

#include "../AntitamperingMrz.hpp"
#include "Mrz.hpp"

class SplitFields
{
    public:
        SplitFields(){};
        std::vector<Field> findMrzLines(const Field *fields, const size_t fieldsSize);
        std::vector<MrzField> extractMrzFields(std::vector<Field> mrzLines);
        std::vector<Field> extractFieldsWithoutMrz(const Field *allFields, const size_t fieldsSize, std::vector<Field> mrzLines);
        bool getCheckDigitsResult();
        
    private:
        bool checkDigitsResult = true;
        MrzType findMrzType(std::vector<Field> mrzLines);
        Mrz* createMrz(MrzType mrzType, std::vector<Field> chars);
        bool isTD1(std::vector<Field> mrzLines);
        bool isTD2(std::vector<Field> mrzLines);
        bool isTD3(std::vector<Field> mrzLines);
        bool isMRVA(std::vector<Field> mrzLines);
        bool isMRVB(std::vector<Field> mrzLines);
};
