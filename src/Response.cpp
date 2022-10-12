#include "../include/Response.h"


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
    response.confidence = fields.getConfFinal();
    response.error = 0;
    response.error_message = convertStringtoCharPtr("");
    response.doubtful_fields = new DoubtfulFields;
    response.doubtful_fields->doubdtful_fields_size = associations.size();
    response.doubtful_fields->fields = new IncorrectDataField[response.doubtful_fields->doubdtful_fields_size];
    
    if(fields.getConfFinal() < confidence_threshold)
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