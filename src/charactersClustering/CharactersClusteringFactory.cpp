#include "CharactersClusteringFactory.hpp"

std::map<std::string, ClusteringType> mapClusteringType = {
    {"DbscanPoints", DbscanPoints},
    {"DbscanBoxes", DbscanBoxes}
};

std::shared_ptr<CharactersClustering> CharactersClusteringFactory::createClustering(const std::string& algorithmType)
{
    SPDLOG_INFO("Creating characters clustering");
    ClusteringType type = mapClusteringType.find(algorithmType)->second;
    switch (type)
    {
        case DbscanPoints:
            return std::shared_ptr<CharactersClustering>(std::make_shared<DBSCANpoints>(epsDbscanPoints));
        case DbscanBoxes:
            return std::shared_ptr<CharactersClustering>(std::make_shared<DBSCANboxes>(epsDbscanBoxes));
        default:
        {
            SPDLOG_ERROR("FATAL: Algorithm Type NOT FOUND,  {}", algorithmType);
            throw Exception(ErrorCode::DETECTOR_TYPE_NOT_FOUND, std::string("Algorithm Type not handled"));
        }
    }
}