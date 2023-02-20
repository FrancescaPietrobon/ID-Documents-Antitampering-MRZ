#include "DBSCANboxes.hpp"

DBSCANboxes::DBSCANboxes(float e):
    eps(e){};

std::vector<Fields> DBSCANboxes::associateCharacters(const Characters *characters, const size_t charactersSize)
{
    SPDLOG_INFO("Compute Characters Clusters");
    std::vector<CharactersClustered> charactersClustered = fillCharactersClustered(characters, charactersSize);
    std::vector<std::vector<size_t>> nearCharacters;
    nearCharacters.resize(charactersSize);
    std::cout << "nearCharacters size post resize: " << nearCharacters.size() << std::endl;

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
    std::cout << "clusterIdx: " << clusterIdx << std::endl;
    std::vector<Fields> fields = computeFields(charactersClustered, clusterIdx);

    return fields;
}

std::vector<CharactersClustered> DBSCANboxes::fillCharactersClustered(const Characters *characters, const size_t charactersSize)
{
    std::vector<CharactersClustered> charactersClustered;
    CharactersClustered characterClustered;
    for(size_t i = 0; i < charactersSize; ++i)
    {
        characterClustered.character = characters[i];
        characterClustered.cluster = NOT_ASSIGNED;
        charactersClustered.push_back(characterClustered);
    }
    std::cout << "charactersClustered size post fill: " << charactersClustered.size() << std::endl;
    return charactersClustered; 
}

std::vector<std::vector<size_t>> DBSCANboxes::findNearCharacters(std::vector<CharactersClustered> characters, std::vector<std::vector<size_t>> nearCharacters)
{
    for(size_t i = 0; i < characters.size(); i++)
        for(size_t j = 0; j < characters.size(); j++)
        {
            if(i == j)
                continue;
            if(computeDistance(characters[i].character, characters[j].character) <= eps)
                nearCharacters[i].push_back(j);
        }
    std::cout << "nearCharacters size post findNearCharacters: " << nearCharacters.size() << std::endl;
    return nearCharacters;
}

float DBSCANboxes::computeDistance(Characters char1, Characters char2)
{
    Coordinates mostRight = char1.position.topLeftX < char2.position.topLeftX ? char1.position : char2.position;
    Coordinates mostLeft = char2.position.topLeftX < char1.position.topLeftX ? char1.position : char2.position;
    
    float xDiff = mostRight.bottomRightX - mostLeft.topLeftX;
    
    Coordinates upper = char1.position.topLeftY < char2.position.topLeftY ? char1.position : char2.position;
    Coordinates lower = char2.position.topLeftY < char1.position.topLeftY ? char1.position : char2.position;
    
    float yDiff = lower.topLeftY - upper.topLeftY;

    return std::sqrt(xDiff * xDiff + 10 * yDiff * yDiff);
    //return std::sqrt(0.05 * xDiff * xDiff + yDiff * yDiff);
}

std::vector<CharactersClustered> DBSCANboxes::dfs(size_t now, size_t cluster, std::vector<CharactersClustered> characters, std::vector<std::vector<size_t>> nearCharacters)
{
    characters[now].cluster = cluster;
    for(auto & next: nearCharacters[now])
    {
        if(characters[next].cluster != NOT_ASSIGNED) continue;
        characters = dfs(next, cluster, characters, nearCharacters);
    }
    return characters;
}

std::vector<Fields> DBSCANboxes::computeFields(std::vector<CharactersClustered> charactersClustered, size_t numClusters)
{
    std::vector<Fields> fields;
    Fields field;
    std::multimap<float, Characters> cluster;
    float sumConfidence;
    size_t countElements;
    std::cout << "charactersClustered.size() in compute fields: " << charactersClustered.size() << std::endl;
    std::cout << "numClusters in compute fields: " << numClusters << std::endl;
    for(size_t i = 0; i <= numClusters; ++i)
    {
        std::cout << i << std::endl;
        sumConfidence = 0;
        countElements = 0;
        for(size_t j = 0; j < charactersClustered.size(); ++j)
        {
            if(charactersClustered[j].cluster == i)
            {
                std::cout << "same cluster! " << charactersClustered[j].cluster << " " << i << std::endl;
                cluster.emplace(charactersClustered[j].character.position.bottomRightX, charactersClustered[j].character);
                sumConfidence += charactersClustered[j].character.confidence;
                countElements += 1;
            }
        }
        std::cout << "post for" << std::endl;
        std::vector<Characters> orderedCharacters = orderCharacters(cluster);
        field = fillField(sumConfidence/countElements, countElements, orderedCharacters);
        fields.push_back(field);
        cluster.clear();
    }
    return fields;
}

std::vector<Characters> DBSCANboxes::orderCharacters(std::multimap<float, Characters> cluster)
{
    std::cout << "Order char" << std::endl;
    // To order wrt x coordinate
    std::vector<Characters> orderedCharacters;
    for(auto & character: cluster)
        orderedCharacters.push_back(character.second);
    std::cout << "Size of orderedCharacters post Order char: " << orderedCharacters.size() << std::endl;
    return orderedCharacters;
}

Fields DBSCANboxes::fillField(float confidence, size_t labelSize, std::vector<Characters> orderedCharacters)
{
    std::cout << "fillField" << std::endl;
    Fields field;
    field.confidence = confidence;
    field.labelSize = labelSize;
    field.label = utils::convertStringtoCharPtr(extractLabel(orderedCharacters));
    field.position = extractPosition(orderedCharacters);
    std::cout << "label: " << field.label << "\t labelSize: " << field.labelSize << "\t confidence: " << confidence << std::endl;
    return field;
}

std::string DBSCANboxes::extractLabel(std::vector<Characters> orderedCharacters)
{
    std::cout << "extractLabel" << std::endl;
    std::string label;
    for(size_t i = 0; i < orderedCharacters.size(); ++i)
        label.push_back(orderedCharacters[i].label);
    return label;
}

Coordinates DBSCANboxes::extractPosition(std::vector<Characters> orderedCharacters)
{   
    std::cout << "extractPosition" << std::endl;
    std::cout << "orderedCharacters size: " << orderedCharacters.size() << std::endl;
    Coordinates position;
    position.topLeftX = orderedCharacters[0].position.topLeftX;
    position.topLeftY = orderedCharacters[0].position.topLeftY;
    position.bottomRightX = orderedCharacters[orderedCharacters.size()-1].position.bottomRightX;
    position.bottomRightY = orderedCharacters[orderedCharacters.size()-1].position.bottomRightY;
    std::cout << "end extractPosition" << std::endl;
    return position;
}