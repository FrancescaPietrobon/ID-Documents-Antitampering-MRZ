#ifndef _ANTITAMPERING_MRZ_FACTORY_H_
#define _ANTITAMPERING_MRZ_FACTORY_H_

#include "antitampering/mrz/AntitamperingMrz/AntitamperingMrz.h"
#include "antitampering/mrz/AntitamperingMrz/DBSCAN/AntitamperingMrzDBSCAN.h"
#include "antitampering/mrz/exceptions/Exceptions.hpp"
//#include <json/writer.h>

#include <spdlog/spdlog.h>

#define ANTITAMPERING_MRZ_MODEL_PATH_ENV_VAR "ANTITAMPERING_MRZ_MODEL_PATH"
//#define DEFAULT_ANTITAMPERING_MRZ_TYPE "DBSCAN"
#define DEFAULT_ANTITAMPERING_MRZ_TYPE "DBSCAN"
#define ANTITAMPERING_MRZ_TYPE_ENV_VAR "ANTITAMPERING_MRZ_TYPE"


class AntitamperingMrzFactory {
    public: 
        static std::shared_ptr<AntitamperingMrz> createAntitamperingMrz(const std::string& modelPath);
};

std::shared_ptr<AntitamperingMrz> AntitamperingMrzFactory::createAntitamperingMrz(const std::string& modelPath)
{
    SPDLOG_INFO("Loading model");
    char* antitamperingMrzTypeEnvVar = std::getenv(ANTITAMPERING_MRZ_TYPE_ENV_VAR);
    std::string antitamperingMrzType = antitamperingMrzTypeEnvVar ? antitamperingMrzTypeEnvVar : DEFAULT_ANTITAMPERING_MRZ_TYPE;

    if(strcmp(antitamperingMrzType.c_str(),"DBSCAN") == 0)
    {
        char* OCRModelPathEnvVar = std::getenv(ANTITAMPERING_MRZ_MODEL_PATH_ENV_VAR);
        std::string OCRModelPath = (OCRModelPathEnvVar ? OCRModelPathEnvVar : modelPath);
        AntitamperingMrzDBSCAN antitamperingMrzDBSCAN = AntitamperingMrzDBSCAN(OCRModelPath);
        std::shared_ptr<AntitamperingMrz> antitamperingMrz = std::make_shared<AntitamperingMrzDBSCAN>(antitamperingMrzDBSCAN);
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