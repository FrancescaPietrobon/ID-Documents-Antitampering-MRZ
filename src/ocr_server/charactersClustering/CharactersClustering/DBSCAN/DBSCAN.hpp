#pragma once

#include "../CharactersClustering.hpp"

#include <unordered_map>

const size_t NOT_ASSIGNED = -1;

typedef std::vector<std::vector<float>> matrix2D;

struct CharacterClustered
{
    Character character;
    size_t cluster;
};

class DBSCAN : public CharactersClustering
{
    public:
        DBSCAN(const float e);
        virtual ~DBSCAN(){};
        std::vector<Field> clusterCharacters(const Character *character, const size_t charactersSize) override;
        
    private:
        const float eps;
        std::vector<CharacterClustered> fillCharactersClustered(const Character *character, const size_t charactersSize);
        std::vector<std::vector<size_t>> findNearCharacters(std::vector<CharacterClustered> character, std::vector<std::vector<size_t>> nearCharacters);
        float computeDistance(Character char1, Character char2);
        std::vector<CharacterClustered> dfs(size_t now, size_t cluster, std::vector<CharacterClustered> character, std::vector<std::vector<size_t>> nearCharacters);
        
        std::vector<Field> computeFields(std::vector<CharacterClustered> charactersClustered, size_t numClusters);
        std::vector<Character> orderCharacters(std::multimap<float, Character> cluster);
        Field fillField(float confidence, size_t labelSize, std::vector<Character> orderedCharacters);
        std::string extractLabel(std::vector<Character> orderedCharacters);
        Coordinates extractPosition(std::vector<Character> orderedCharacters);
};
