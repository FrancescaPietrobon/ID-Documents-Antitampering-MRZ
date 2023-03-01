#pragma once

#include "../CharactersClustering.hpp"

#include <unordered_map>

const size_t NOT_ASSIGNED = -1;

typedef std::vector<std::vector<float>> matrix2D;

struct CharactersClustered
{
    Characters character;
    size_t cluster;
};

class DBSCAN : public CharactersClustering
{
    public:
        DBSCAN(const float e);
        virtual ~DBSCAN(){};
        std::vector<Fields> clusterCharacters(const Characters *characters, const size_t charactersSize) override;
        
    private:
        const float eps;
        std::vector<CharactersClustered> fillCharactersClustered(const Characters *characters, const size_t charactersSize);
        std::vector<std::vector<size_t>> findNearCharacters(std::vector<CharactersClustered> characters, std::vector<std::vector<size_t>> nearCharacters);
        float computeDistance(Characters char1, Characters char2);
        std::vector<CharactersClustered> dfs(size_t now, size_t cluster, std::vector<CharactersClustered> characters, std::vector<std::vector<size_t>> nearCharacters);
        std::vector<Fields> computeFields(std::vector<CharactersClustered> charactersClustered, size_t numClusters);
        std::vector<Characters> orderCharacters(std::multimap<float, Characters> cluster);
        Fields fillField(float confidence, size_t labelSize, std::vector<Characters> orderedCharacters);
        std::string extractLabel(std::vector<Characters> orderedCharacters);
        Coordinates extractPosition(std::vector<Characters> orderedCharacters);
};