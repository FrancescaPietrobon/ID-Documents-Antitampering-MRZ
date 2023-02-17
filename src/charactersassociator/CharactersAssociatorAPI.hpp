#pragma once

#include "CharactersAssociator/CharactersAssociator.hpp"

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

    AssociatorResponse process(OcrResponse *ocrResponse, char *algorithm_type);

} // extern "C"

AssociatorResponse process(OcrResponse *ocrResponse, std::shared_ptr<CharactersAssociator> associator);