#include "CharactersClusteringApi.hpp"

#include "common/utils/utils.hpp"

#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <new>

#define DEFAULT_CONFIDENCE_THRESHOLD_CHAR_CLUST 0.3

// IMPLEMENTATION

ClusteringResponse buildGlobalErrorResponseCharsClust(const Exception &exception);

extern "C"
{
    ClusteringResponse cluster(OcrResponse ocrResponse, char *algorithmType)
    {
        ClusteringResponse res;
        std::shared_ptr<CharactersClustering> cluster = nullptr;
        try
        {
            cluster = CharactersClusteringFactory::createClustering(std::string(algorithmType));
        }
        catch (const Exception &e)
        {
            SPDLOG_ERROR("Error Processing Request : {}", e.getMessage());
            res = buildGlobalErrorResponseCharsClust(e);
            return res;
        }

        res = clusterChar(ocrResponse, cluster);
        return res;
    }
}

ClusteringResponse clusterChar(OcrResponse ocrResponse, std::shared_ptr<CharactersClustering> cluster)
{
    ClusteringResponse res;
    float sumConf;
    res.resultDetailsSize = ocrResponse.resultDetailsSize;
    res.resultDetails = new ClusteringResultDetail[res.resultDetailsSize];
    for (std::size_t i = 0; i < res.resultDetailsSize; i++)
    {
        sumConf = 0;
        res.resultDetails[i].image = utils::convertStringtoCharPtr(ocrResponse.resultDetails[i].image);
        res.resultDetails[i].error = 0;
        res.resultDetails[i].errorMessage = utils::convertStringtoCharPtr("");
        std::vector<Field> clusteringResults;
        try
        {
            clusteringResults = cluster->clusterCharacters(ocrResponse.resultDetails[i].characters, ocrResponse.resultDetails[i].charactersSize);
        }
        catch (Exception &ex)
        {
            res.resultDetails[i].fieldsSize = 0;
            res.resultDetails[i].error = ex.getCode();
            res.resultDetails[i].errorMessage = utils::convertStringtoCharPtr(ex.getMessage());
            continue;
        }
        res.resultDetails[i].fieldsSize = clusteringResults.size();
        res.resultDetails[i].fields = new Field[res.resultDetails[i].fieldsSize];
        for (std::size_t j = 0; j < clusteringResults.size(); j++)
        {
            res.resultDetails[i].fields[j].label = clusteringResults[j].label;
            res.resultDetails[i].fields[j].labelSize = clusteringResults[j].labelSize;
            res.resultDetails[i].fields[j].position = clusteringResults[j].position;
            res.resultDetails[i].fields[j].confidence = clusteringResults[j].confidence;
            sumConf += clusteringResults[j].confidence;
        }
        res.resultDetails[i].confidence = sumConf / res.resultDetails[i].fieldsSize;
    }
    return res;
}

ClusteringResponse buildGlobalErrorResponseCharsClust(const Exception &exception)
{
    ClusteringResponse res;
    res.resultDetailsSize = 1;
    res.resultDetails = new ClusteringResultDetail[res.resultDetailsSize];
    res.resultDetails[0].image = utils::convertStringtoCharPtr("");
    res.resultDetails[0].confidence = -1;
    res.resultDetails[0].fieldsSize = 0;
    res.resultDetails[0].error = exception.getCode();
    res.resultDetails[0].errorMessage = utils::convertStringtoCharPtr(exception.getMessage());
    return res;
}