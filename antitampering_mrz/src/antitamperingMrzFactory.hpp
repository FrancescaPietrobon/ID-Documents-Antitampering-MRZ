#ifndef _ANTITAMPERING_MRZ_FACTORY_H_
#define _ANTITAMPERING_MRZ_FACTORY_H_

#include "AntitamperingMrz/AntitamperingMrz.h"
#include "AntitamperingMrz/EuclDist/AntitamperingMrzEuclDist.h"
#include "exceptions/Exceptions.hpp"
//#include <json/writer.h>

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>

#define ANTITAMPERING_MRZ_MODEL_PATH_ENV_VAR "ANTITAMPERING_MRZ_MODEL_PATH"
//#define DEFAULT_ANTITAMPERING_MRZ_TYPE "DBSCAN"
#define DEFAULT_ANTITAMPERING_MRZ_TYPE "EuclDist"
#define ANTITAMPERING_MRZ_TYPE_ENV_VAR "ANTITAMPERING_MRZ_TYPE"


class AntitamperingMrzFactory {
    public: 
        std::shared_ptr<AntitamperingMrz> initialize(std::string& modelPath);
};

std::shared_ptr<AntitamperingMrz> AntitamperingMrzFactory::initialize(std::string& modelPath)
{
    SPDLOG_INFO("Loading model");
    char* antitamperingMrzTypeEnvVar = std::getenv(ANTITAMPERING_MRZ_TYPE_ENV_VAR);
    std::string antitamperingMrzType = antitamperingMrzTypeEnvVar ? antitamperingMrzTypeEnvVar : DEFAULT_ANTITAMPERING_MRZ_TYPE;
    /*
    if(strcmp(antitamperingMrzType.c_str(),"DBSCAN") == 0)
    {
        char* OCRModelPathEnvVar = std::getenv(OCR_MODEL_PATH_ENV_VAR);
        std::string OCRModelPath = (OCRModelPathEnvVar ? OCRModelPathEnvVar : modelPath);
        AntitamperingMrzDBSCAN antitamperingMrzDBSCAN = AntitamperingMrzDBSCAN(OCRModelPath);
        std::shared_ptr<AntitamperingMrz> antitamperingMrz = std::make_shared<AntitamperingMrzDBSCAN>(antitamperingMrzDBSCAN);
        return antitamperingMrz;
    }
    else
    */
    if(strcmp(antitamperingMrzType.c_str(),"EuclDist") == 0)
    {
        char* OCRModelPathEnvVar = std::getenv(ANTITAMPERING_MRZ_MODEL_PATH_ENV_VAR);
        std::string OCRModelPath = (OCRModelPathEnvVar ? OCRModelPathEnvVar : modelPath);
        AntitamperingMrzEuclDist antitamperingMrzEuclDist = AntitamperingMrzEuclDist(OCRModelPath);
        std::shared_ptr<AntitamperingMrz> antitamperingMrz = std::make_shared<AntitamperingMrzEuclDist>(antitamperingMrzEuclDist);
        return antitamperingMrz;
    }
    else
    {
        SPDLOG_ERROR("Antitampering Mrz Type not accepted: {}", antitamperingMrzType);
        throw AntitamperingMrzException(AntitamperingMrzErrorCode::ANTITAMPERING_MRZ_TYPE_NOT_FOUND, "AntitamperingMrz Type NOT accepted : " + antitamperingMrzType);
    }
    SPDLOG_INFO("Model successfully loaded");
}


#endif