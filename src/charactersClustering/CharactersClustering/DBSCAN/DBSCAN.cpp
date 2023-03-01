#include "DBSCAN.hpp"

DBSCAN::DBSCAN(float e):
    eps(e){};

std::vector<Fields> DBSCAN::clusterCharacters(const Characters *characters, const size_t charactersSize)
{
    SPDLOG_INFO("Compute Characters Clusters");
    std::vector<CharactersClustered> charactersClustered = fillCharactersClustered(characters, charactersSize);
    std::vector<std::vector<size_t>> nearCharacters;
    nearCharacters.resize(charactersSize);

    SPDLOG_INFO("Find near characters");
    nearCharacters = findNearCharacters(charactersClustered, nearCharacters);
    size_t clusterIdx = -1;
    for(size_t i = 0; i < charactersSize; i++)
    {
        if(charactersClustered[i].cluster != NOT_ASSIGNED)
            continue;
        charactersClustered = dfs(i, ++clusterIdx, charactersClustered, nearCharacters);
    }
    
    SPDLOG_INFO("Compute fields");
    std::vector<Fields> fields = computeFields(charactersClustered, clusterIdx);

    return fields;
}

std::vector<CharactersClustered> DBSCAN::fillCharactersClustered(const Characters *characters, const size_t charactersSize)
{
    std::vector<CharactersClustered> charactersClustered;
    CharactersClustered characterClustered;
    for(size_t i = 0; i < charactersSize; ++i)
    {
        characterClustered.character = characters[i];
        characterClustered.cluster = NOT_ASSIGNED;
        charactersClustered.push_back(characterClustered);
    }
    return charactersClustered; 
}

std::vector<std::vector<size_t>> DBSCAN::findNearCharacters(std::vector<CharactersClustered> characters, std::vector<std::vector<size_t>> nearCharacters)
{
    for(size_t i = 0; i < characters.size(); i++)
        for(size_t j = 0; j < characters.size(); j++)
        {
            if(i == j)
                continue;
            if(computeDistance(characters[i].character, characters[j].character) <= eps)
                nearCharacters[i].push_back(j);
        }
    return nearCharacters;
}

float DBSCAN::computeDistance(Characters char1, Characters char2)
{
    Coordinates mostRight = char1.position.topLeftX < char2.position.topLeftX ? char1.position : char2.position;
    Coordinates mostLeft = char2.position.topLeftX < char1.position.topLeftX ? char1.position : char2.position;
    float xDiff = mostRight.bottomRightX - mostLeft.topLeftX;
    
    Coordinates upper = char1.position.topLeftY < char2.position.topLeftY ? char1.position : char2.position;
    Coordinates lower = char2.position.topLeftY < char1.position.topLeftY ? char1.position : char2.position;
    float yDiff = lower.topLeftY - upper.topLeftY;

    return std::sqrt(xDiff * xDiff + 10 * yDiff * yDiff);
}

std::vector<CharactersClustered> DBSCAN::dfs(size_t now, size_t cluster, std::vector<CharactersClustered> characters, std::vector<std::vector<size_t>> nearCharacters)
{
    characters[now].cluster = cluster;
    for(auto & next: nearCharacters[now])
    {
        if(characters[next].cluster != NOT_ASSIGNED) continue;
        characters = dfs(next, cluster, characters, nearCharacters);
    }
    return characters;
}

std::vector<Fields> DBSCAN::computeFields(std::vector<CharactersClustered> charactersClustered, size_t numClusters)
{
    std::vector<Fields> fields;
    Fields field;
    std::multimap<float, Characters> cluster;
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
        std::vector<Characters> orderedCharacters = orderCharacters(cluster);
        field = fillField(sumConfidence/countElements, countElements, orderedCharacters);
        fields.push_back(field);
        cluster.clear();
    }
    return fields;
}

std::vector<Characters> DBSCAN::orderCharacters(std::multimap<float, Characters> cluster)
{
    // To order wrt x coordinate
    std::vector<Characters> orderedCharacters;
    for(auto & character: cluster)
        orderedCharacters.push_back(character.second);
    return orderedCharacters;
}

Fields DBSCAN::fillField(float confidence, size_t labelSize, std::vector<Characters> orderedCharacters)
{
    Fields field;
    field.confidence = confidence;
    field.labelSize = labelSize;
    field.label = utils::convertStringtoCharPtr(extractLabel(orderedCharacters));
    field.position = extractPosition(orderedCharacters);
    return field;
}

std::string DBSCAN::extractLabel(std::vector<Characters> orderedCharacters)
{
    std::string label;
    for(size_t i = 0; i < orderedCharacters.size(); ++i)
        label.push_back(orderedCharacters[i].label);
    return label;
}

Coordinates DBSCAN::extractPosition(std::vector<Characters> orderedCharacters)
{   
    Coordinates position;
    position.topLeftX = orderedCharacters[0].position.topLeftX;
    position.topLeftY = orderedCharacters[0].position.topLeftY;
    position.bottomRightX = orderedCharacters[orderedCharacters.size()-1].position.bottomRightX;
    position.bottomRightY = orderedCharacters[orderedCharacters.size()-1].position.bottomRightY;
    return position;
}