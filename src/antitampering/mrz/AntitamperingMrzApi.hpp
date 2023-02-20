#pragma once

#include "AntitamperingMrz/AntitamperingMrz.hpp"
#include "charactersClustering/CharactersClusteringApi.hpp"

// RESPONSE

struct AntitamperingMrzResultDetail {
    char* image;
    bool result;
    float confidenceThresholdOcr;
    float confidence;
    DoubtfulFields* doubtfulFields;
    size_t doubdtfulFieldSize;
    size_t error;
    char* errorMessage;
};

struct AntitamperingMrzResponse {
    AntitamperingMrzResultDetail* resultDetails;
    size_t resultDetailsSize;
    bool result;
};

// IMPLEMENTATION

extern "C" {

    AntitamperingMrzResponse associate(ClusteringResponse ocrResponse, char* algorithmType);

} // extern "C"

AntitamperingMrzResponse associateFields(ClusteringResponse ocrResponse, std::shared_ptr<AntitamperingMrz> associator);