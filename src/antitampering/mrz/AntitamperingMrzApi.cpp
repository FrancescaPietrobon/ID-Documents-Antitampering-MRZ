#include "AntitamperingMrzApi.hpp"
#include "factory/AntitamperingMrzFactory.hpp"

#include "common/exceptions/Exceptions.hpp"

#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <new>

// IMPLEMENTATION

AntitamperingMrzResponse buildGlobalErrorResponseAntitampMrz(const Exception& exception);

extern "C" {
    AntitamperingMrzResponse associate(char **arr_image, char **arr_content_type, DocumentFields *document_fields, size_t arr_size, char *algorithm_type)
    {
        AntitamperingMrzResponse res;
        std::shared_ptr<AntitamperingMrz> associator = nullptr;
        try
        {
            associator = AntitamperingMrzFactory::createAntitamperingMrz(std::string(algorithm_type));
        }
        catch(const Exception& e)
        {
            SPDLOG_ERROR("Error Processing Request : {}", e.getMessage());
            res = buildGlobalErrorResponseAntitampMrz(e);
            return res;
        }
        res = associateFields(arr_image, arr_content_type, document_fields, arr_size, associator);
        return res;
    }
}

AntitamperingMrzResponse associateFields(char **arr_image, char **arr_content_type, DocumentFields *document_fields, size_t arr_size, std::shared_ptr<AntitamperingMrz> associator)
{
    AntitamperingMrzResponse res;
    res.resultDetailsSize = arr_size;
    res.resultDetails = new AntitamperingMrzResultDetail[res.resultDetailsSize];
    for (std::size_t i = 0; i < res.resultDetailsSize; ++i)
    {
        res.resultDetails[i].image = utils::convertStringtoCharPtr(arr_image[i]);
        res.resultDetails[i].error = 0;
        res.resultDetails[i].errorMessage = utils::convertStringtoCharPtr("");
        std::vector<DoubtfulField> doubtfulFields;
        float finalConfidence;
        try
        {
            doubtfulFields = associator->extractDoubtfulFields(document_fields[i].fields, document_fields[i].fieldsSize);
            finalConfidence = associator->computeConfFinal(doubtfulFields);
        }
        catch (Exception &ex)
        {
            res.resultDetails[i].doubdtfulFieldsSize = 0;
            res.resultDetails[i].error = ex.getCode();
            res.resultDetails[i].errorMessage = utils::convertStringtoCharPtr(ex.getMessage());
            continue;
        }
        res.resultDetails[i].confidence = finalConfidence;
        res.resultDetails[i].doubdtfulFieldsSize = doubtfulFields.size();
        res.resultDetails[i].doubtfulFields = new DoubtfulField[res.resultDetails[i].doubdtfulFieldsSize];
        for (std::size_t j = 0; j < doubtfulFields.size(); j++)
        {
            res.resultDetails[i].doubtfulFields[j].fieldType = doubtfulFields[j].fieldType;
            res.resultDetails[i].doubtfulFields[j].dataField = doubtfulFields[j].dataField;
            res.resultDetails[i].doubtfulFields[j].mrzDataField = doubtfulFields[j].mrzDataField;
            res.resultDetails[i].doubtfulFields[j].confidenceField = doubtfulFields[j].confidenceField;
        }
    }
    return res;
}

AntitamperingMrzResponse buildGlobalErrorResponseAntitampMrz(const Exception &exception)
{
    AntitamperingMrzResponse res;
    res.resultDetailsSize = 1;
    res.resultDetails = new AntitamperingMrzResultDetail[res.resultDetailsSize];
    res.resultDetails[0].image = utils::convertStringtoCharPtr("");
    res.resultDetails[0].confidence = -1;
    res.resultDetails[0].doubdtfulFieldsSize = 0;
    res.resultDetails[0].error = exception.getCode();
    res.resultDetails[0].errorMessage = utils::convertStringtoCharPtr(exception.getMessage());
    return res;
}