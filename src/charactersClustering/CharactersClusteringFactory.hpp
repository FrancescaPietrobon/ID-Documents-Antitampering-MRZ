#pragma once

#include "CharactersClustering/DBSCANpoints/DBSCANpoints.hpp"
#include "CharactersClustering/DBSCANboxes/DBSCANboxes.hpp"
#include "common/exceptions/Exceptions.hpp"

#include <spdlog/spdlog.h>
#include <map>

#define epsDbscanPoints 8 //7
#define epsDbscanBoxes 30 //3

enum ClusteringType
{
    DbscanPoints = 1,
    DbscanBoxes
};

class CharactersClusteringFactory {
    public: 
        static std::shared_ptr<CharactersClustering> createClustering(const std::string& algorithmType);
};