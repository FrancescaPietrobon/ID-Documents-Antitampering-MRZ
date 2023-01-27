#ifndef _RESPONSE_H
#define _RESPONSE_H

#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <ostream>
#include <new>
#include "Fields.h"

// RESPONSE

struct IncorrectDataField {
    char* data_field;
    char* mrz_data_field;
};


struct DoubtfulFields {
    IncorrectDataField* fields;
    size_t doubdtful_fields_size;
};

struct OcrMrzResultDetail {
    char* image;
    bool result;
    float confidence_threshold;
    float confidence;
    DoubtfulFields* doubtful_fields;
    size_t error;
    char* error_message;
};

struct OcrMrzResponse {
    bool result;
    OcrMrzResultDetail* result_details;
    size_t result_details_size;
};

char* convertStringtoCharPtr(std::string str);

OcrMrzResultDetail fillResponse(std::string image, Fields fields, float confidence_threshold);
void printResponse(OcrMrzResultDetail res);

#endif