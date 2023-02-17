#include "CharactersAssociatorApi.hpp"
#include "CharactersAssociatorFactory.hpp"

#include "common/utils/utils.hpp"

#include "logging/LogParameters.hpp"

#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <new>

// IMPLEMENTATION

AssociatorResponse buildGlobalErrorResponse(const Exception &exception);

extern "C"
{
    AssociatorResponse process(OcrResponse *ocrResponse, char *algorithm_type)
    {
        AssociatorResponse res;
        std::shared_ptr<CharactersAssociator> associator = nullptr;
        try
        {
            associator = CharactersAssociatorFactory::createAssociator(std::string(algorithm_type));
        }
        catch (const Exception &e)
        {
            SPDLOG_ERROR("Error Processing Request : {}", e.getMessage());
            res = buildGlobalErrorResponse(e);
            return res;
        }

        res = process(ocrResponse, associator);
        return res;
    }
}

AssociatorResponse process(OcrResponse *ocrResponse, std::shared_ptr<CharactersAssociator> associator)
{
    AssociatorResponse res;
    res.resultDetailsSize = arr_size;
    res.resultDetails = new AssociatorResultDetail[res.resultDetailsSize];
    for (std::size_t i = 0; i < res.resultDetailsSize; i++)
    {
        res.resultDetails[i].image = utils::convertStringtoCharPtr(arr_image[i]);
        res.resultDetails[i].error = 0;
        res.resultDetails[i].errorMessage = utils::convertStringtoCharPtr("");
        std::vector<Fields> associatorResults;
        try
        {
            associatorResults = detector->associate(ocrResponse);
        }
        catch (Exception &ex)
        {
            res.resultDetails[i].fieldsSize = 0;
            res.resultDetails[i].error = ex.getCode();
            res.resultDetails[i].errorMessage = utils::convertStringtoCharPtr(ex.getMessage());
            continue;
        }
        res.resultDetails[i].fieldsSize = associatorResults.size();
        res.resultDetails[i].fields = new Fields[res.resultDetails[i].fieldsSize];
        for (std::size_t j = 0; j < associatorResults.size(); j++)
        {
            res.resultDetails[i].fields[j].label = associatorResults[j].label;
            res.resultDetails[i].fields[j].labelSize = associatorResults[j].labelSize;
            res.resultDetails[i].fields[j].position = associatorResults[j].position;
            res.resultDetails[i].fields[j].confidence = associatorResults[j].confidence;
        }
    }
    return res;
}

AssociatorResponse buildGlobalErrorResponse(const Exception &exception)
{
    AssociatorResponse res;
    res.resultDetailsSize = 1;
    res.resultDetails = new AssociatorResultDetail[res.resultDetailsSize];
    res.resultDetails[0].image = utils::convertStringtoCharPtr("");
    res.resultDetails[0].confidence = -1;
    res.resultDetails[0].fieldsSize = 0;
    res.resultDetails[0].error = exception.getCode();
    res.resultDetails[0].errorMessage = utils::convertStringtoCharPtr(exception.getMessage());
    return res;
}