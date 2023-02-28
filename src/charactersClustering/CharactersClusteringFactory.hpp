#pragma once

#include "CharactersClustering/DBSCAN/DBSCAN.hpp"
#include "common/exceptions/Exceptions.hpp"

#include <spdlog/spdlog.h>
#include <map>

#define epsDbscan 30

enum ClusteringType
{
    Dbscan = 1,
    //KMeans
};

class CharactersClusteringFactory {
    public: 
        static std::shared_ptr<CharactersClustering> createClustering(const std::string& algorithmType);
};