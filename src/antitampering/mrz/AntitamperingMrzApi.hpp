#pragma once

#include "AntitamperingMrz/AntitamperingMrz.hpp"

// RESPONSE

struct AntitamperingMrzResultDetail {
    char* image;
    float confidence;
    DoubtfulField* doubtfulFields;
    size_t doubdtfulFieldsSize;
    size_t error;
    char* errorMessage;
};

struct AntitamperingMrzResponse {
    AntitamperingMrzResultDetail* resultDetails;
    size_t resultDetailsSize;
};

struct DocumentFields
{
    Field *fields;
    size_t fieldsSize;
};

// IMPLEMENTATION

extern "C" {
    AntitamperingMrzResponse associate(char **arr_image, char **arr_content_type, DocumentFields *document_fields, size_t arr_size, char *algorithm_type);

} // extern "C"

AntitamperingMrzResponse associateFields(char **arr_image, char **arr_content_type, DocumentFields *document_fields, size_t arr_size, std::shared_ptr<AntitamperingMrz> associator);