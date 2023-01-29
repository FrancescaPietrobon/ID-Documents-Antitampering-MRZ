#include "antitamperingMrzApi.hpp"
#include "antitamperingMrzFactory.hpp"

#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <ostream>
#include <string>
#include <new>
#include <iostream>

/*
char* convertStringtoCharPtr(std::string str)
{
    char *cstr = new char[str.length() + 1];
    strcpy(cstr, str.c_str());
    return cstr;
}
*/

// IMPLEMENTATION

OcrMrzResponse buildGlobalErrorResponse(const AntitamperingMrzException& exception);

extern "C" {

    OcrMrzResponse processOld(char **arr_image, char **arr_content_base64, Coordinates *arr_coordinates, size_t arr_size, float* arr_confidence_threshold, char* modelPath)
    {
        OcrMrzResponse res;
        std::shared_ptr<AntitamperingMrz> detector = nullptr;
        try
        {
            detector = AntitamperingMrzFactory::createAntitamperingMrz(std::string(modelPath));
        }
        catch(const AntitamperingMrzException& e)
        {
            SPDLOG_ERROR("Error Processing Request : {}", e.getMessage());
            res = buildGlobalErrorResponse(e);
            return res;
        }
        res = processOld(arr_image, arr_content_base64, arr_coordinates, arr_confidence_threshold, arr_size, detector);
        return res;
    }
}


OcrMrzResponse processOld(char **arr_image, char **arr_content_base64, Coordinates *arr_coordinates, size_t arr_size, float* confidence_threshold)
{
    std::string modelsPath = "/usr/share/phygitalai/models";
    std::shared_ptr<AntitamperingMrzFactory> factory = std::shared_ptr<AntitamperingMrzFactory>(new AntitamperingMrzFactory());
    std::shared_ptr<AntitamperingMrz> antitamperingMrzService = factory -> createAntitamperingMrz(modelsPath);
    
    OcrMrzResponse res;
    res.result = true;
    res.result_details_size = arr_size;
    res.result_details = new OcrMrzResultDetail[res.result_details_size];
    for(unsigned i = 0; i < res.result_details_size; i++)
    {
        res.result_details[i].image =  convertStringtoCharPtr(arr_image[i]);
        res.result_details[i].result = false;
        res.result_details[i].confidence_threshold = confidence_threshold[i];
        res.result_details[i].confidence = 0.6;
        res.result_details[i].error = 0;
        res.result_details[i].error_message = convertStringtoCharPtr("");
        res.result_details[i].doubtful_fields = new DoubtfulFields;
        res.result_details[i].doubtful_fields -> doubdtful_fields_size = 2;
        res.result_details[i].doubtful_fields -> fields = new IncorrectDataField[res.result_details[i].doubtful_fields -> doubdtful_fields_size];
        
        AntitamperingMrzData document;
        document.imageName = arr_image[i];
        //document.imageBase64 = arr_content_base64[i];
        document.confidenceThreshold = confidence_threshold[i];

        res.result_details[i] = antitamperingMrzService -> check(document);
    }
    return res;
}


OcrMrzResponse buildGlobalErrorResponse(const AntitamperingMrzException& exception)
{
    OcrMrzResponse res;
    res.result = false;
    res.result_details_size = 1;
    res.result_details = new OcrMrzResultDetail[res.result_details_size];
    res.result_details[0].image = convertStringtoCharPtr("");
    res.result_details[0].result = false;
    res.result_details[0].confidence_threshold = -1;
    res.result_details[0].confidence = 0.6;
    res.result_details[0].error = exception.getCode();
    res.result_details[0].error_message = convertStringtoCharPtr(exception.getMessage());
    return res;
}