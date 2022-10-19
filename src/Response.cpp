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
    std::vector<association> doubtfulFields = fields.getDoubtfulFields();
    response.image = convertStringtoCharPtr(image);
    response.result = fields.getResult();
    response.confidence_threshold = confidence_threshold;
    response.confidence = fields.getConfFinal();
    response.error = 0;
    response.error_message = convertStringtoCharPtr("");
    response.doubtful_fields = new DoubtfulFields;
    response.doubtful_fields->doubdtful_fields_size = doubtfulFields.size();
    response.doubtful_fields->fields = new IncorrectDataField[response.doubtful_fields->doubdtful_fields_size];
    
    if(fields.getConfFinal() > confidence_threshold)
    {
        size_t i = 0;
        for(auto ass: doubtfulFields)
        {
            response.doubtful_fields->fields[i].data_field = convertStringtoCharPtr(ass.dataField);
            response.doubtful_fields->fields[i].mrz_data_field = convertStringtoCharPtr(ass.mrzDataField);
            ++i;
        }
    }
    return response;
}

void printResponse(OcrMrzResponseResult res)
{
    std::cout << "\n\nFINAL RESPONSE" <<std::endl;

    std::cout << "\nImage name: " << res.image << std::endl;

    std::cout << "\nResult: " << std::boolalpha << res.result << std::endl;
    std::cout << "\nConfidence threshold: " << res.confidence_threshold << std::endl;
    std::cout << "\nConfidence: " << res.confidence << std::endl;
    std::cout << "\nDoubtful fields: "<< std::endl;
    
    for(size_t i = 0; i < res.doubtful_fields->doubdtful_fields_size; ++i)
    {
        std::cout << "\ndata-field: " << res.doubtful_fields->fields->data_field << std::endl;
        std::cout << "mrz-data-field: " << res.doubtful_fields->fields->mrz_data_field << std::endl;
    }
    
    std::cout << "\nNumber of doubtful fields: " << res.doubtful_fields->doubdtful_fields_size << std::endl;
}