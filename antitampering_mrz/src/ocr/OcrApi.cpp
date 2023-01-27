#include "OcrApi.hpp"
#include "ocr_utils/ocrUtils.hpp"
#include "OcrFactory.hpp"
#include "exceptions/Exceptions.hpp"

#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <ostream>
#include <string>
#include <new>
#include <iostream>

#define DEFAULT_CONFIDENCE_THRESHOLD 0.5

// IMPLEMENTATION

char *convertStringtoCharPtr(const std::string &str);

OcrResponse buildGlobalErrorResponse(const OcrException &exception);

extern "C"
{
    OcrResponse process(char **arr_image, char **arr_content_type, char **arr_content_base64, Coordinates *arr_coordinates, float *arr_confidence_threshold, size_t arr_size, char *algorithm_type)
    {
        OcrResponse res;
        std::shared_ptr<Ocr> detector = nullptr;
        try
        {
            detector = OcrFactory::createOCR(std::string(algorithm_type));
        }
        catch (const OcrException &e)
        {
            SPDLOG_ERROR("Error Processing Request : {}", e.getMessage());
            res = buildGlobalErrorResponse(e);
            return res;
        }

        res = process(arr_image, arr_content_type, arr_content_base64, arr_coordinates, arr_confidence_threshold, arr_size, detector);
        return res;
    }
}

OcrResponse process(char **arr_image, char **arr_content_type, char **arr_content_base64, Coordinates *arr_coordinates, float *arr_confidence_threshold, size_t arr_size, std::shared_ptr<Ocr> detector)
{
    OcrResponse res;
    res.result = true;
    res.resultDetailsSize = arr_size;
    res.resultDetails = new OcrResultDetail[res.resultDetailsSize];
    for (std::size_t i = 0; i < res.resultDetailsSize; i++)
    {
        res.resultDetails[i].image = convertStringtoCharPtr(arr_image[i]);
        res.resultDetails[i].confidenceThreshold = arr_confidence_threshold[i] != -1.0 ? arr_confidence_threshold[i] : DEFAULT_CONFIDENCE_THRESHOLD;
        res.resultDetails[i].error = 0;
        res.resultDetails[i].errorMessage = convertStringtoCharPtr("");
        std::vector<OcrData> ocrResults;
        try
        {
            cv::Mat image = fromBase64toCvMat(arr_content_base64[i]);
            Coordinates characterRect = arr_coordinates[i];
            image = prepareImage(characterRect, image);
            ocrResults = detector->detect(image, res.resultDetails[i].confidenceThreshold);
        }
        catch (OcrException &ex)
        {
            res.resultDetails[i].detectedCharactersSize = 0;
            res.resultDetails[i].result = false;
            res.resultDetails[i].error = ex.getCode();
            res.resultDetails[i].errorMessage = convertStringtoCharPtr(ex.getMessage());
            res.result = false;
            continue;
        }
        res.resultDetails[i].detectedCharactersSize = ocrResults.size();
        res.resultDetails[i].detectedCharacters = new Characters[res.resultDetails[i].detectedCharactersSize];
        res.resultDetails[i].result = (ocrResults.size() > 0);
        for (std::size_t j = 0; j < ocrResults.size(); j++)
        {
            res.resultDetails[i].detectedCharacters[j].detectedCharacter = ocrResults[j].label;
            res.resultDetails[i].detectedCharacters[j].detectedCharacterIndex = ocrResults[j].labelIndex;
            res.resultDetails[i].detectedCharacters[j].characterPosition = ocrResults[j].position;
            res.resultDetails[i].detectedCharacters[j].confindence = ocrResults[j].confidence;
        }
    }
    return res;
}

char *convertStringtoCharPtr(const std::string &str)
{
    char *cstr = new char[str.length() + 1];
    strcpy(cstr, str.c_str());
    return cstr;
}

OcrResponse buildGlobalErrorResponse(const OcrException &exception)
{
    OcrResponse res;
    res.result = false;
    res.resultDetailsSize = 1;
    res.resultDetails = new OcrResultDetail[res.resultDetailsSize];
    res.resultDetails[0].image = convertStringtoCharPtr("");
    res.resultDetails[0].confidenceThreshold = -1;
    res.resultDetails[0].detectedCharactersSize = 0;
    res.resultDetails[0].error = exception.getCode();
    res.resultDetails[0].errorMessage = convertStringtoCharPtr(exception.getMessage());
    return res;
}