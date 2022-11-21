#ifndef _ANTITAMPERING_MRZ_API_H_
#define _ANTITAMPERING_MRZ_API_H_

#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <ostream>
#include <new>
//#include "AntitamperingMrz/DBSCAN/include/Response.h"
#include "AntitamperingMrz/AntitamperingMrz.h"

// RESPONSE

/*
struct IncorrectDataField {
    char* data_field;
    char* mrz_data_field;
};


struct DoubtfulFields {
    IncorrectDataField* fields;
    size_t doubdtful_fields_size;
};

struct OcrMrzResponseResult {
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
    OcrMrzResponseResult* result_details;
    size_t result_details_size;
};
*/

// IMPLEMENTATION

extern "C" {

    OcrMrzResponse process(char **arr_image, char **arr_content_type, char **arr_content_base64, char **arr_json, float* confidence_threshold, size_t arr_size);

} // extern "C"

#endif