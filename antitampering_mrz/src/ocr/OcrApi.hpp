#include "Ocr/Ocr.hpp"

// RESPONSE

struct Characters{
    char detectedCharacter;
    size_t detectedCharacterIndex;
    Coordinates characterPosition;
    float confindence;
};

struct OcrResultDetail
{
    char *image;
    bool result;
    float confidenceThreshold;
    Characters *detectedCharacters;
    size_t detectedCharactersSize;
    size_t error;
    char *errorMessage;
};

struct OcrResponse
{
    bool result;
    OcrResultDetail *resultDetails;
    size_t resultDetailsSize;
};


// IMPLEMENTATION

extern "C"
{

    OcrResponse process(char **arr_image, char **arr_content_type, char **arr_content_base64, Coordinates *arr_coordinates, float *arr_confidence_threshold, size_t arr_size, char *algorithm_type);

} // extern "C"

OcrResponse process(char **arr_image, char **arr_content_type, char **arr_content_base64, Coordinates *arr_coordinates, float *arr_confidence_threshold, size_t arr_size, std::shared_ptr<Ocr> detector);
