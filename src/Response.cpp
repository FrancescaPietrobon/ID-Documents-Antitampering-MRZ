#include "../include/Response.h"

/*
void fillResponse(Fields fields, float confidence_threshold)
{
    OcrMrzResponse res;
    res.result = true;
    res.result_details_size = 1;
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
        
    }
}
*/

char* convertStringtoCharPtr(std::string str)
{
    char *cstr = new char[str.length() + 1];
    strcpy(cstr, str.c_str());
    return cstr;
} 


OcrMrzResponseResult fillResponse(std::string image, Fields fields, float confidence_threshold)
{
    OcrMrzResponseResult response;
    std::map<std::string, std::pair<std::pair<std::string,std::string>, float>> associations = fields.getFinalAssociations();
    response.image = convertStringtoCharPtr(image);
    response.result = fields.getResult();
    response.confidence_threshold = confidence_threshold;
    response.confidence = fields.getFinalConf();
    response.error = 0;
    response.error_message = convertStringtoCharPtr("");
    response.doubtful_fields = new DoubtfulFields;
    response.doubtful_fields->doubdtful_fields_size = associations.size();
    response.doubtful_fields->fields = new IncorrectDataField[response.doubtful_fields->doubdtful_fields_size];
    
    if(fields.getFinalConf() < confidence_threshold)
    {
        size_t i = 0;
        for(auto association: associations)
        {
            response.doubtful_fields->fields[i].data_field = convertStringtoCharPtr(association.second.first.first);
            response.doubtful_fields->fields[i].mrz_data_field = convertStringtoCharPtr(association.second.first.second);
            ++i;
        }
    }
    return response;
}

/*
std::map<std::string, std::pair<std::pair<std::string,std::string>, float>>

float finalConf = fields.computeFinalConf();
bool result = true;

std::cout << "\nImage name: " << imagePath << std::endl;
if(finalConf <= CONF_THRESHOLD)
    result = false;


std::cout << "\nField: " << association.first << std::endl;
std::cout << "data-field: " << association.second.first.first << std::endl;
std::cout << "mrz-data-field: " << association.second.first.second << std::endl;
std::cout << "confidence: " << association.second.second << std::endl;
*/