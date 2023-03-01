#pragma once

#include "charactersClustering/CharactersClusteringApi.hpp"

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

class AntitamperingMrz {
    public:
        AntitamperingMrz(){};
        virtual ~AntitamperingMrz(){};
        virtual std::pair<std::vector<AssociatedField>, std::vector<DoubtfulFields>> extractAssociations(const Fields *doubtfulFields, const size_t doubtfulFieldsSize) = 0;
        virtual float computeConfFinal(std::vector<DoubtfulFields> doubtfulAss, std::vector<AssociatedField> finAss) = 0;
};