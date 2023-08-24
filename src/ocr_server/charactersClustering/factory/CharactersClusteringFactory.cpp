#include "CharactersClusteringFactory.hpp"

std::map<std::string, ClusteringType> mapClusteringType = {
    {"Dbscan", Dbscan},
    //{"KMeans", KMeans}
};

std::shared_ptr<CharactersClustering> CharactersClusteringFactory::createClustering(const std::string& algorithmType)
{
    SPDLOG_INFO("Creating characters clustering");
    ClusteringType type = mapClusteringType.find(algorithmType)->second;
    switch (type)
    {
        case Dbscan:
            return std::shared_ptr<CharactersClustering>(std::make_shared<DBSCAN>(epsDbscan));
        
        default:
        {
            SPDLOG_ERROR("FATAL: Algorithm Type NOT FOUND,  {}", algorithmType);
            throw Exception(ErrorCode::DETECTOR_TYPE_NOT_FOUND, std::string("Algorithm Type not handled"));
        }
    }
}
