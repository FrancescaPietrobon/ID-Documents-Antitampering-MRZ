#include "AntitamperingMrzFactory.hpp"


std::map<std::string, AssociatorType> mapAssociatorType = {
    {"levenshtein", levenshtein},
    //{"boh", boh}
};

std::shared_ptr<AntitamperingMrz> AntitamperingMrzFactory::createAntitamperingMrz(const std::string& algorithmType)
{
    SPDLOG_INFO("Creating Antitampering MRZ");
    AssociatorType type = mapAssociatorType.find(algorithmType)->second;
    switch (type)
    {
        case levenshtein:
            return std::shared_ptr<AntitamperingMrz>(std::make_shared<Associations>(CONF_THRESHOLD_LEVENSHTEIN));
        default:
        {
            SPDLOG_ERROR("FATAL: Algorithm Type NOT FOUND,  {}", algorithmType);
            throw Exception(ErrorCode::DETECTOR_TYPE_NOT_FOUND, std::string("Algorithm Type not handled"));
        }
    }
}
