#pragma once

#include "CharactersAssociator/DBSCAN/DBSCAN.hpp"
#include "CharactersAssociator/EuclideanDistance/EuclideanDistance.hpp"
#include "common/exceptions/Exceptions.hpp"

#include <spdlog/spdlog.h>
#include <map>

#define epsDbscan 27
#define epsEuclDist 1

enum AssociatorType
{
    Dbscan,
    EuclDistance
};

class CharactersAssociatorFactory {
    public: 
        static std::shared_ptr<CharactersAssociator> createAssociator(const std::string& algorithmType);
};