#pragma once

#include "ocr_server/charactersClustering/CharactersClusteringApi.hpp"

#include <spdlog/spdlog.h>

struct DoubtfulFields {
    char* fieldType;
    char* dataField;
    char* mrzDataField;
    float confidenceField;
};

struct AssociatedField
{
    char* fieldType;
    Fields field;
    char* mrzDataField;
    float confidenceField;
};

class AntitamperingMrz
{
    public:
        AntitamperingMrz(){};
        virtual ~AntitamperingMrz(){};
        virtual std::vector<DoubtfulFields> extractDoubtfulFields(const Fields *allFields, const size_t fieldsSize) = 0;
        virtual float computeConfFinal(std::vector<DoubtfulFields> doubtfulAss) = 0;
};