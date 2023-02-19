#include "CharactersAssociatorFactory.hpp"

std::map<std::string, AssociatorType> mapAssociatorType = {
    {"Dbscan", Dbscan},
    {"EuclDistance", EuclDistance}
};

std::shared_ptr<CharactersAssociator> CharactersAssociatorFactory::createAssociator(const std::string& algorithmType)
{
    SPDLOG_INFO("Creating characters associator");
    AssociatorType type = mapAssociatorType.find(algorithmType)->second;
    switch (type)
    {
        case Dbscan:
            return std::shared_ptr<CharactersAssociator>(std::make_shared<DBSCAN>(epsDbscan));
        case EuclDistance:
            return std::shared_ptr<CharactersAssociator>(std::make_shared<EuclideanDistance>(epsEuclDist));
        default:
        {
            SPDLOG_ERROR("FATAL: Algorithm Type NOT FOUND,  {}", algorithmType);
            throw Exception(ErrorCode::DETECTOR_TYPE_NOT_FOUND, std::string("Algorithm Type not handled"));
        }
    }
}