#include "OcrApi.hpp"
#include "OcrFactory.hpp"

#include "common/utils/utils.hpp"

#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <ostream>
#include <string>
#include <new>

#define DEFAULT_CONFIDENCE_THRESHOLD 0.3

// IMPLEMENTATION

OcrResponse buildGlobalErrorResponse(const Exception &exception);

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
        catch (const Exception &e)
        {
            SPDLOG_ERROR("Error Processing Request : {}", e.getMessage());
            res = buildGlobalErrorResponse(e);
            return res;
        }

        res = processImage(arr_image, arr_content_type, arr_content_base64, arr_coordinates, arr_confidence_threshold, arr_size, detector);
        return res;
    }
}

OcrResponse processImage(char **arr_image, char **arr_content_type, char **arr_content_base64, Coordinates *arr_coordinates, float *arr_confidence_threshold, size_t arr_size, std::shared_ptr<Ocr> detector)
{
    OcrResponse res;
    res.resultDetailsSize = arr_size;
    res.resultDetails = new OcrResultDetail[res.resultDetailsSize];
    for (std::size_t i = 0; i < res.resultDetailsSize; i++)
    {
        res.resultDetails[i].image = utils::convertStringtoCharPtr(arr_image[i]);
        res.resultDetails[i].confidenceThreshold = arr_confidence_threshold[i] != -1.0 ? arr_confidence_threshold[i] : DEFAULT_CONFIDENCE_THRESHOLD;
        res.resultDetails[i].error = 0;
        res.resultDetails[i].errorMessage = utils::convertStringtoCharPtr("");
        std::vector<Characters> ocrResults;
        try
        {
            cv::Mat image = utils::fromBase64toCvMat(arr_content_base64[i]);
            cv::imwrite("../pre_cut.jpg", image);
            Coordinates characterRect = arr_coordinates[i];
            image = utils::prepareImage(characterRect, image);
            ocrResults = detector->detect(image, res.resultDetails[i].confidenceThreshold);
        }
        catch (Exception &ex)
        {
            res.resultDetails[i].charactersSize = 0;
            res.resultDetails[i].error = ex.getCode();
            res.resultDetails[i].errorMessage = utils::convertStringtoCharPtr(ex.getMessage());
            continue;
        }
        res.resultDetails[i].charactersSize = ocrResults.size();
        res.resultDetails[i].characters = new Characters[res.resultDetails[i].charactersSize];
        for (std::size_t j = 0; j < ocrResults.size(); j++)
        {
            res.resultDetails[i].characters[j].label = ocrResults[j].label;
            res.resultDetails[i].characters[j].labelIndex = ocrResults[j].labelIndex;
            res.resultDetails[i].characters[j].position = ocrResults[j].position;
            res.resultDetails[i].characters[j].confidence = ocrResults[j].confidence;
        }
    }
    return res;
}

OcrResponse buildGlobalErrorResponse(const Exception &exception)
{
    OcrResponse res;
    res.resultDetailsSize = 1;
    res.resultDetails = new OcrResultDetail[res.resultDetailsSize];
    res.resultDetails[0].image = utils::convertStringtoCharPtr("");
    res.resultDetails[0].confidenceThreshold = -1;
    res.resultDetails[0].charactersSize = 0;
    res.resultDetails[0].error = exception.getCode();
    res.resultDetails[0].errorMessage = utils::convertStringtoCharPtr(exception.getMessage());
    return res;
}