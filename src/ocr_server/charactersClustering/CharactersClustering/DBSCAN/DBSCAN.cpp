#include "DBSCAN.hpp"

DBSCAN::DBSCAN(float e):
    eps(e){};

std::vector<Field> DBSCAN::clusterCharacters(const Character *characters, const size_t charactersSize)
{
    SPDLOG_INFO("Compute Characters Clusters");
    std::vector<CharacterClustered> charactersClustered = this->fillCharactersClustered(characters, charactersSize);
    std::vector<std::vector<size_t>> nearCharacters;
    nearCharacters.resize(charactersSize);

    SPDLOG_INFO("Find near characters");
    nearCharacters = this->findNearCharacters(charactersClustered, nearCharacters);
    size_t clusterIdx = -1;
    for(size_t i = 0; i < charactersSize; i++)
    {
        if(charactersClustered[i].cluster != NOT_ASSIGNED)
            continue;
        charactersClustered = dfs(i, ++clusterIdx, charactersClustered, nearCharacters);
    }
    
    SPDLOG_INFO("Compute fields");
    std::vector<Field> fields = this->computeFields(charactersClustered, clusterIdx);

    return fields;
}

std::vector<CharacterClustered> DBSCAN::fillCharactersClustered(const Character *characters, const size_t charactersSize)
{
    std::vector<CharacterClustered> charactersClustered;
    CharacterClustered characterClustered;
    for(size_t i = 0; i < charactersSize; ++i)
    {
        characterClustered.character = characters[i];
        characterClustered.cluster = NOT_ASSIGNED;
        charactersClustered.push_back(characterClustered);
    }
    return charactersClustered; 
}

std::vector<std::vector<size_t>> DBSCAN::findNearCharacters(std::vector<CharacterClustered> characters, std::vector<std::vector<size_t>> nearCharacters)
{
    for(size_t i = 0; i < characters.size(); i++)
        for(size_t j = 0; j < characters.size(); j++)
        {
            if(i == j)
                continue;
            if(this->computeDistance(characters[i].character, characters[j].character) <= this->eps)
                nearCharacters[i].push_back(j);
        }
    return nearCharacters;
}

float DBSCAN::computeDistance(Character char1, Character char2)
{
    Coordinates mostRight = char1.position.topLeftX < char2.position.topLeftX ? char1.position : char2.position;
    Coordinates mostLeft = char2.position.topLeftX < char1.position.topLeftX ? char1.position : char2.position;
    float xDiff = mostRight.bottomRightX - mostLeft.topLeftX;
    
    Coordinates upper = char1.position.topLeftY < char2.position.topLeftY ? char1.position : char2.position;
    Coordinates lower = char2.position.topLeftY < char1.position.topLeftY ? char1.position : char2.position;
    float yDiff = lower.topLeftY - upper.topLeftY;

    return std::sqrt(xDiff * xDiff + 10 * yDiff * yDiff);
}

std::vector<CharacterClustered> DBSCAN::dfs(size_t now, size_t cluster, std::vector<CharacterClustered> characters, std::vector<std::vector<size_t>> nearCharacters)
{
    characters[now].cluster = cluster;
    for(auto & next: nearCharacters[now])
    {
        if(characters[next].cluster != NOT_ASSIGNED)
            continue;
        characters = this->dfs(next, cluster, characters, nearCharacters);
    }
    return characters;
}

std::vector<Field> DBSCAN::computeFields(std::vector<CharacterClustered> charactersClustered, size_t numClusters)
{
    std::vector<Field> fields;
    Field field;
    std::multimap<float, Character> cluster;
    float sumConfidence;
    size_t countElements;
    for(size_t i = 0; i <= numClusters; ++i)
    {
        sumConfidence = 0;
        countElements = 0;
        for(size_t j = 0; j < charactersClustered.size(); ++j)
        {
            if(charactersClustered[j].cluster == i)
            {
                cluster.emplace(charactersClustered[j].character.position.bottomRightX, charactersClustered[j].character);
                sumConfidence += charactersClustered[j].character.confidence;
                countElements += 1;
            }
        }
        std::vector<Character> orderedCharacters = this->orderCharacters(cluster);
        field = this->fillField(sumConfidence/countElements, countElements, orderedCharacters);
        fields.push_back(field);
        cluster.clear();
    }
    return fields;
}

std::vector<Character> DBSCAN::orderCharacters(std::multimap<float, Character> cluster)
{
    // To order wrt x coordinate
    std::vector<Character> orderedCharacters;
    for(auto & character: cluster)
        orderedCharacters.push_back(character.second);
    return orderedCharacters;
}

Field DBSCAN::fillField(float confidence, size_t labelSize, std::vector<Character> orderedCharacters)
{
    Field field;
    field.confidence = confidence;
    field.labelSize = labelSize;
    field.label = utils::convertStringtoCharPtr(this->extractLabel(orderedCharacters));
    field.position = this->extractPosition(orderedCharacters);
    return field;
}

std::string DBSCAN::extractLabel(std::vector<Character> orderedCharacters)
{
    std::string label;
    for(size_t i = 0; i < orderedCharacters.size(); ++i)
        label.append(1, orderedCharacters[i].label[0]);
    return label;
}

Coordinates DBSCAN::extractPosition(std::vector<Character> orderedCharacters)
{   
    Coordinates position;
    position.topLeftX = orderedCharacters[0].position.topLeftX;
    position.topLeftY = orderedCharacters[0].position.topLeftY;
    position.bottomRightX = orderedCharacters[orderedCharacters.size()-1].position.bottomRightX;
    position.bottomRightY = orderedCharacters[orderedCharacters.size()-1].position.bottomRightY;
    return position;
}
