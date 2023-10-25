#pragma once

#include "common/utils/utils.hpp"

#include <spdlog/spdlog.h>

struct DoubtfulField
{
    char* fieldType;
    char* dataField;
    char* mrzDataField;
    float confidenceField;
};

struct AssociatedField
{
    char* fieldType;
    Field field;
    char* mrzDataField;
    float confidenceField;
};

class AntitamperingMrz
{
    public:
        AntitamperingMrz(){};
        virtual ~AntitamperingMrz(){};
        virtual std::vector<DoubtfulField> extractDoubtfulFields(const Field *allFields, const size_t fieldsSize) = 0;
        virtual float computeConfFinal(std::vector<DoubtfulField> doubtfulAss) = 0;
};
