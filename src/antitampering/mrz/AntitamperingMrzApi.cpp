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
    AntitamperingMrzResponse associate(ClusteringResponse ocrResponse, float *arr_confidence_threshold, char* algorithmType)
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
            res = buildGlobalErrorResponseAntitampMrz(e);
            return res;
        }
        res = associateFields(ocrResponse, arr_confidence_threshold, associator);
        return res;
    }
}

AntitamperingMrzResponse associateFields(ClusteringResponse ocrResponse, float *arr_confidence_threshold, std::shared_ptr<AntitamperingMrz> associator)
{
    AntitamperingMrzResponse res;
    res.resultDetailsSize = ocrResponse.resultDetailsSize;
    res.resultDetails = new AntitamperingMrzResultDetail[res.resultDetailsSize];
    for (std::size_t i = 0; i < res.resultDetailsSize; i++)
    {
        res.resultDetails[i].image = utils::convertStringtoCharPtr(ocrResponse.resultDetails[i].image);
        res.resultDetails[i].error = 0;
        res.resultDetails[i].errorMessage = utils::convertStringtoCharPtr("");
        res.resultDetails[i].confidenceThreshold = arr_confidence_threshold[i] != -1.0 ? arr_confidence_threshold[i] : CONF_THRESHOLD_WER;
        std::pair<std::vector<AssociatedField>, std::vector<DoubtfulFields>> associations;
        std::vector<DoubtfulFields> doubtfulFields;
        float finalConfidence;
        try
        {
            associations = associator->extractAssociations(ocrResponse.resultDetails[i].fields, ocrResponse.resultDetails[i].fieldsSize);
            doubtfulFields = associations.second;
            finalConfidence = associator->computeConfFinal(doubtfulFields, associations.first);
        }
        catch (Exception &ex)
        {
            res.resultDetails[i].doubdtfulFieldSize = 0;
            res.resultDetails[i].error = ex.getCode();
            res.resultDetails[i].errorMessage = utils::convertStringtoCharPtr(ex.getMessage());
            continue;
        }
        res.resultDetails[i].confidence = finalConfidence;
        res.resultDetails[i].doubdtfulFieldSize = doubtfulFields.size();
        res.resultDetails[i].doubtfulFields = new DoubtfulFields[res.resultDetails[i].doubdtfulFieldSize];
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
    res.resultDetails[0].confidenceThreshold = -1;
    res.resultDetails[0].confidence = -1;
    res.resultDetails[0].doubdtfulFieldSize = 0;
    res.resultDetails[0].error = exception.getCode();
    res.resultDetails[0].errorMessage = utils::convertStringtoCharPtr(exception.getMessage());
    return res;
}