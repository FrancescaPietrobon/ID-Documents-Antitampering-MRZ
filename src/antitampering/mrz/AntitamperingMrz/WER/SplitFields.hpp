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
        
    private:
        std::vector<Field> mrzLines;
        MrzType findMrzType(std::vector<Field> mrzLines);
        Mrz* createMrz(MrzType mrzType, std::vector<Field> chars);
};