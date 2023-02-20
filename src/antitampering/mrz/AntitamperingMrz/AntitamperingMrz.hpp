#pragma once

#include "charactersClustering/CharactersClusteringApi.hpp"

#include <spdlog/spdlog.h>

struct DoubtfulFields {
    char* fieldType;
    char* dataField;
    char* mrzDataField;
    float confidenceField;
};

class AntitamperingMrz {
    public:
        AntitamperingMrz(){};
        virtual ~AntitamperingMrz(){};
        virtual std::vector<DoubtfulFields> check(const Fields *doubtfulFields, const size_t doubtfulFieldsSize) = 0;
};