#pragma once

#include "AntitamperingMrz/AntitamperingMrz.hpp"
#include "ocr_server/charactersClustering/CharactersClusteringApi.hpp"

// RESPONSE

struct AntitamperingMrzResultDetail {
    char* image;
    bool result;
    float confidenceThresholdOcr;
    float confidenceThresholdAntitampering;
    float confidence;
    bool checkDigistsMrz;
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

    AntitamperingMrzResponse associate(ClusteringResponse ocrResponse, float *arr_confidence_threshold, char* algorithmType);

} // extern "C"

AntitamperingMrzResponse associateFields(ClusteringResponse ocrResponse, float *arr_confidence_threshold, std::shared_ptr<AntitamperingMrz> associator);