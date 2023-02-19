#pragma once

#include "CharactersAssociator/CharactersAssociator.hpp"
#include "CharactersAssociatorFactory.hpp"

// RESPONSE
struct AssociatorResultDetail
{
    char *image;
    Fields *fields;
    size_t fieldsSize;
    float confidence;
    size_t error;
    char *errorMessage;
};

struct AssociatorResponse
{
    AssociatorResultDetail *resultDetails;
    size_t resultDetailsSize;
};

// IMPLEMENTATION

extern "C"
{

    AssociatorResponse associate(OcrResponse ocrResponse, char *algorithm_type);

} // extern "C"

AssociatorResponse associateChar(OcrResponse ocrResponse, std::shared_ptr<CharactersAssociator> associator);