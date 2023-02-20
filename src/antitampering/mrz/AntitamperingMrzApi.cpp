#include "AntitamperingMrzApi.hpp"
#include "AntitamperingMrzFactory.hpp"

#include "common/exceptions/Exceptions.hpp"

#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <new>

// IMPLEMENTATION

AntitamperingMrzResponse buildGlobalErrorResponse(const Exception& exception);

extern "C" {
    AntitamperingMrzResponse associate(ClusteringResponse ocrResponse, char* algorithmType)
    {
        AntitamperingMrzResponse res;
        std::shared_ptr<AntitamperingMrz> associator = nullptr;
        try
        {
            associator = AntitamperingMrzFactory::createAntitamperingMrz(std::string(algorithmType));
        }
        catch(const Exception& e)
        {
            SPDLOG_ERROR("Error Processing Request : {}", e.getMessage());
            res = buildGlobalErrorResponse(e);
            return res;
        }
        res = associateFields(ocrResponse, associator);
        return res;
    }
}

AntitamperingMrzResponse associateFields(ClusteringResponse ocrResponse, std::shared_ptr<AntitamperingMrz> associator)
{
    AntitamperingMrzResponse res;
    res.result = true;
    res.resultDetailsSize = ocrResponse.resultDetailsSize;
    res.resultDetails = new AntitamperingMrzResultDetail[res.resultDetailsSize];
    float sumConfidences;
    for (std::size_t i = 0; i < res.resultDetailsSize; i++)
    {
        sumConfidences = 0;
        res.resultDetails[i].image = utils::convertStringtoCharPtr(ocrResponse.resultDetails[i].image);
        res.resultDetails[i].result = true;
        res.resultDetails[i].error = 0;
        res.resultDetails[i].errorMessage = utils::convertStringtoCharPtr("");
        res.resultDetails[i].confidenceThresholdOcr = ocrResponse.resultDetails[i].confidenceThreshold;
        std::vector<DoubtfulFields> associatorResult;
        try
        {
            associatorResult = associator->check(ocrResponse.resultDetails[i].fields, ocrResponse.resultDetails[i].fieldsSize);
        }
        catch (Exception &ex)
        {
            res.resultDetails[i].doubdtfulFieldSize = 0;
            res.resultDetails[i].error = ex.getCode();
            res.resultDetails[i].errorMessage = utils::convertStringtoCharPtr(ex.getMessage());
            res.resultDetails[i].confidenceThresholdOcr = ocrResponse.resultDetails[i].confidenceThreshold;
            continue;
        }
        res.resultDetails[i].doubdtfulFieldSize = associatorResult.size();
        res.resultDetails[i].doubtfulFields = new DoubtfulFields[res.resultDetails[i].doubdtfulFieldSize];
        for (std::size_t j = 0; j < associatorResult.size(); j++)
        {
            res.resultDetails[i].doubtfulFields[j].fieldType = associatorResult[j].fieldType;
            res.resultDetails[i].doubtfulFields[j].dataField = associatorResult[j].dataField;
            res.resultDetails[i].doubtfulFields[j].mrzDataField = associatorResult[j].mrzDataField;
            res.resultDetails[i].doubtfulFields[j].confidenceField = associatorResult[j].confidenceField;
            sumConfidences += associatorResult[j].confidenceField;
        }
        res.resultDetails[i].confidence = sumConfidences / associatorResult.size();
        if(associatorResult.size() > 0)
        {
            res.resultDetails[i].result = false;
            res.result = false;
        }
    }
    return res;
}

AntitamperingMrzResponse buildGlobalErrorResponse(const Exception &exception)
{
    AntitamperingMrzResponse res;
    res.result = false;
    res.resultDetailsSize = 1;
    res.resultDetails = new AntitamperingMrzResultDetail[res.resultDetailsSize];
    res.resultDetails[0].image = utils::convertStringtoCharPtr("");
    res.resultDetails[0].result = false;
    res.resultDetails[0].confidenceThresholdOcr = -1;
    res.resultDetails[0].confidence = -1;
    res.resultDetails[0].doubdtfulFieldSize = 0;
    res.resultDetails[0].error = exception.getCode();
    res.resultDetails[0].errorMessage = utils::convertStringtoCharPtr(exception.getMessage());
    return res;
}