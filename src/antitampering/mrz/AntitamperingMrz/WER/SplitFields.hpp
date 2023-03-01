#pragma once

#include "../AntitamperingMrz.hpp"
#include "Mrz.hpp"

class SplitFields
{
    public:
        SplitFields(){};
        std::vector<Fields> findMrzLines(const Fields *fields, const size_t fieldsSize);
        std::vector<MrzFields> extractMrzFields(std::vector<Fields> mrzLines);
        std::vector<Fields> extractFieldsWithoutMrz(const Fields *allFields, const size_t fieldsSize, std::vector<Fields> mrzLines);
        
    private:
        std::vector<Fields> mrzLines;
        MrzType findMrzType(std::vector<Fields> mrzLines);
        Mrz* createMrz(MrzType mrzType, std::vector<Fields> chars);
};