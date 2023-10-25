#pragma once

#include "CharactersClustering/CharactersClustering.hpp"
#include "factory/CharactersClusteringFactory.hpp"

// RESPONSE
struct ClusteringResultDetail
{
    char *image;
    Field *field;
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

    ClusteringResponse cluster(OcrResponse ocrResponse, char *algorithmType);

} // extern "C"

ClusteringResponse clusterChar(OcrResponse ocrResponse, std::shared_ptr<CharactersClustering> cluster);
