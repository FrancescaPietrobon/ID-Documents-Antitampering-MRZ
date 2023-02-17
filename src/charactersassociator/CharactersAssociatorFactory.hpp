#pragma once

#include "charactersassociator/DBSCAN/DBSCAN.hpp"
#include "common/exceptions/Exceptions.hpp"

#include <spdlog/spdlog.h>
#include <map>

enum AlgorithmType
{
    DBSCAN = 1
};

class CharactersAssociatorFactory {
    public: 
        static std::shared_ptr<Associator> createAssociator(const std::string& algorithmType);
};