#pragma once

#include "Ocr/Ocr.hpp"

// RESPONSE
struct OcrResultDetail
{
    char *image;
    float confidenceThreshold;
    Characters *characters;
    size_t charactersSize;
    size_t error;
    char *errorMessage;
};

struct OcrResponse
{
    OcrResultDetail *resultDetails;
    size_t resultDetailsSize;
};

// IMPLEMENTATION

extern "C"
{

    OcrResponse process(char **arr_image, char **arr_content_base64, Coordinates *arr_coordinates, float *arr_confidence_threshold, size_t arr_size, char *algorithm_type);

} // extern "C"

OcrResponse processImage(char **arr_image, char **arr_content_base64, Coordinates *arr_coordinates, float *arr_confidence_threshold, size_t arr_size, std::shared_ptr<Ocr> detector);
