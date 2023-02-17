#include "CharactersAssociatorFactory.hpp"

std::map<std::string, AlgorithmType> mapAlgorithmType = {
    {"DBSCAN", DBSCAN}
};

std::shared_ptr<Associator> CharactersAssociatorFactory::createAssociator(const std::string& algorithmType)
{
    SPDLOG_INFO("Creating characters associator");
    AlgorithmType type = mapAlgorithmType.find(algorithmType)->second;
    switch (type)
    {
        case DBSCAN:
        {
            return std::shared_ptr<Associator>(std::make_shared<DBSCAN>());
        }
        default:
        {
            SPDLOG_ERROR("FATAL: Algorithm Type NOT FOUND,  {}", algorithmType);
            throw Exception(ErrorCode::DETECTOR_TYPE_NOT_FOUND, std::string("Algorithm Type not handled"));
        }
    }
}