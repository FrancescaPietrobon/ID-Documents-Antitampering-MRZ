#include "DBSCANpoints.hpp"

DBSCANpoints::DBSCANpoints(float e):
    eps(e){};

std::vector<Fields> DBSCANpoints::associateCharacters(const Characters *characters, const size_t charactersSize)
{
    SPDLOG_INFO("Compute Points");
    points = computePointsFromCharacters(characters, charactersSize);
    size = points.size();
    adjPoints.resize(points.size());

    SPDLOG_INFO("findNearPoints");
    adjPoints = findNearPoints(points);
    for(size_t i = 0; i < size; i++)
    {
        if(points[i].getCluster() != NOT_CLASSIFIED)
            continue;
        points = dfs(i, ++clusterIdx, points, adjPoints);
    }
    
    SPDLOG_INFO("assignCluster");
    cluster = assignCluster(points);

    SPDLOG_INFO("computeFields");
    std::vector<Fields> fields = computeFields(points, adjPoints);

    return fields;
}


std::vector<Point> DBSCANpoints::computePointsFromCharacters(const Characters *characters, const size_t charactersSize)
{
    std::vector<Point> points;
    float h, w, xCenter, yCenter;

    for(size_t i = 0; i < charactersSize; i++)
    {
        w = characters[i].position.bottomRightX - characters[i].position.topLeftX;
        h = characters[i].position.bottomRightY - characters[i].position.topLeftY;

        xCenter = characters[i].position.topLeftX + w / 2;
        yCenter = characters[i].position.topLeftY + h / 2;
 
        Point character(characters[i], xCenter, yCenter, NOT_CLASSIFIED);
        points.push_back(character);
    }
    return points;
}


std::vector<std::vector<size_t>> DBSCANpoints::findNearPoints(std::vector<Point> points)
{
    for(size_t i = 0; i < points.size(); i++)
        for(size_t j = 0; j < points.size(); j++)
        {
            if(i == j)
                continue;
            if(points[i].distance(points[j]) <= eps)
                adjPoints[i].push_back(j);
        }
    return adjPoints;
}

std::vector<Point> DBSCANpoints::dfs(size_t now, size_t c, std::vector<Point> &points, std::vector<std::vector<size_t>> &adjPoints)
{
    points[now].setCluster(c);
    for(auto & next: adjPoints[now]) {
        if(points[next].getCluster() != NOT_CLASSIFIED) continue;
        dfs(next, c, points, adjPoints);
    }
    return points;
}

std::vector<std::vector<size_t>> DBSCANpoints::assignCluster(std::vector<Point> points)
{
    cluster.resize(clusterIdx+1);
    for(size_t i = 0; i < size; i++)
        cluster[points[i].getCluster()].push_back(i);

    return cluster;
}

std::vector<Fields> DBSCANpoints::computeFields(std::vector<Point> points, std::vector<std::vector<size_t>> adjPoints)
{
    std::vector<Fields> fields;
    Fields field;
    std::multimap<float, Point> cluster;
    float sumConfidence;
    size_t countElements;

    for(size_t i = 0; i <= clusterIdx; ++i)
    {
        sumConfidence = 0;
        countElements = 0;
        for(size_t j = 0; j < points.size(); ++j)
        {
            if(points[j].getCluster() == i)
            {
                cluster.emplace(points[j].getX(), points[j]);
                sumConfidence += points[j].getConfidence();
                countElements += 1;
            }
        }
        std::vector<Point> orderedCharacters = orderCharacters(cluster);
        field = fillField(sumConfidence/countElements, countElements, orderedCharacters);
        fields.push_back(field);
        cluster.clear();
    }
    return fields;
}

Fields DBSCANpoints::fillField(float confidence, size_t labelSize, std::vector<Point> orderedCharacters)
{

    Fields field;
    field.confidence = confidence;
    field.labelSize = labelSize;
    field.label = utils::convertStringtoCharPtr(extractLabel(orderedCharacters));
    field.position = extractPosition(orderedCharacters);
    //std::cout << "label: " << field.label << "\t labelSize: " << field.labelSize << "\t confidence: " << confidence << std::endl;

    return field;
}

std::vector<Point> DBSCANpoints::orderCharacters(std::multimap<float, Point> cluster)
{
    // To order wrt x coordinate
    std::vector<Point> orderedCharacters;
    for(auto & character: cluster)
        orderedCharacters.push_back(character.second);
    return orderedCharacters;
}

std::string DBSCANpoints::extractLabel(std::vector<Point> orderedCharacters)
{
    std::string label;
    for(size_t i = 0; i < orderedCharacters.size(); ++i)
        label.push_back(orderedCharacters[i].getLabel());

    return label;
}

Coordinates DBSCANpoints::extractPosition(std::vector<Point> orderedCharacters)
{   
    Coordinates position;
    position.topLeftX = orderedCharacters[0].getPosition().topLeftX;
    position.topLeftY = orderedCharacters[0].getPosition().topLeftY;
    position.bottomRightX = orderedCharacters[orderedCharacters.size()-1].getPosition().bottomRightX;
    position.bottomRightY = orderedCharacters[orderedCharacters.size()-1].getPosition().bottomRightY;

    return position;
}