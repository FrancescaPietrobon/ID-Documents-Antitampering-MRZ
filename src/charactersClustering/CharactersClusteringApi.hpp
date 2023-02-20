#pragma once

#include "CharactersClustering/CharactersClustering.hpp"
#include "CharactersClusteringFactory.hpp"

// RESPONSE
struct ClusteringResultDetail
{
    char *image;
    Fields *fields;
    size_t fieldsSize;
    float confidence;
    size_t error;
    char *errorMessage;
};

struct ClusteringResponse
{
    ClusteringResultDetail *resultDetails;
    size_t resultDetailsSize;
};

// IMPLEMENTATION

extern "C"
{

    ClusteringResponse cluster(OcrResponse ocrResponse, char *algorithm_type);

} // extern "C"

ClusteringResponse clusterChar(OcrResponse ocrResponse, std::shared_ptr<CharactersClustering> cluster);