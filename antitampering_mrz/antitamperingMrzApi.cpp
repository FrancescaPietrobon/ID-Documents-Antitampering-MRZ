#include "antitamperingMrzApi.hpp"
#include "antitamperingMrzFactory.hpp"
#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <ostream>
#include <string>
#include <new>
#include <iostream>
#include <cstring>

/*
char* convertStringtoCharPtr(std::string str)
{
    char *cstr = new char[str.length() + 1];
    strcpy(cstr, str.c_str());
    return cstr;
}
*/

// IMPLEMENTATION

extern "C" {
    OcrMrzResponse process(char **arr_image, char **arr_content_type, char **arr_content_base64, char **arr_json, float* confidence_threshold, size_t arr_size)
    {
        std::string modelsPath = "/usr/share/phygitalai/models";
        std::shared_ptr<AntitamperingMrzFactory> factory = std::shared_ptr<AntitamperingMrzFactory>(new AntitamperingMrzFactory());
        std::shared_ptr<AntitamperingMrz> antitamperingMrzService = factory -> initialize(modelsPath);
        
        OcrMrzResponse res;
        res.result = true;
        res.result_details_size = arr_size;
        res.result_details = new OcrMrzResponseResult[res.result_details_size];
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
            
            //my code

            AntitamperingMrzData document;
            document.imageName = arr_image[i];
            //document.imageBase64 = arr_content_base64[i];
            //document.documentMetaData = arr_json[i];
            document.confidenceThreshold = confidence_threshold[i];

            res.result_details[i] = antitamperingMrzService -> check(document);
            
            if(i == 0)
            {
                res.result_details[i].doubtful_fields -> fields[0].data_field = convertStringtoCharPtr("Antonio");
                res.result_details[i].doubtful_fields -> fields[0].mrz_data_field = convertStringtoCharPtr("Antofdsio");
                res.result_details[i].doubtful_fields -> fields[1].data_field = convertStringtoCharPtr("22/07/1987");
                res.result_details[i].doubtful_fields -> fields[1].mrz_data_field = convertStringtoCharPtr("19870823");
            }
            else
            {
                res.result_details[i].doubtful_fields -> fields[0].data_field = convertStringtoCharPtr("Prova");
                res.result_details[i].doubtful_fields -> fields[0].mrz_data_field= convertStringtoCharPtr("Test");
                res.result_details[i].doubtful_fields -> fields[1].data_field = convertStringtoCharPtr("M");
                res.result_details[i].doubtful_fields -> fields[1].mrz_data_field = convertStringtoCharPtr("N");
            
            }
        }
        return res;
    }

}