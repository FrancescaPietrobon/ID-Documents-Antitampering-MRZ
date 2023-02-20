#include "CharactersAssociatorFactory.hpp"

std::map<std::string, AssociatorType> mapAssociatorType = {
    {"DbscanPoints", DbscanPoints},
    {"DbscanBoxes", DbscanBoxes}
};

std::shared_ptr<CharactersAssociator> CharactersAssociatorFactory::createAssociator(const std::string& algorithmType)
{
    SPDLOG_INFO("Creating characters associator");
    AssociatorType type = mapAssociatorType.find(algorithmType)->second;
    switch (type)
    {
        case DbscanPoints:
            return std::shared_ptr<CharactersAssociator>(std::make_shared<DBSCANpoints>(epsDbscanPoints));
        case DbscanBoxes:
            return std::shared_ptr<CharactersAssociator>(std::make_shared<DBSCANboxes>(epsDbscanBoxes));
        default:
        {
            SPDLOG_ERROR("FATAL: Algorithm Type NOT FOUND,  {}", algorithmType);
            throw Exception(ErrorCode::DETECTOR_TYPE_NOT_FOUND, std::string("Algorithm Type not handled"));
        }
    }
}