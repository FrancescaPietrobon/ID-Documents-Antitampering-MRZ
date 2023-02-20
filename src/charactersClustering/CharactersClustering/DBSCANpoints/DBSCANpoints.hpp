#pragma once

#include "../CharactersClustering.hpp"

#include <unordered_map>
#include "Point.hpp"

const size_t NOT_CLASSIFIED = -1;

typedef std::vector<std::vector<float>> matrix2D;

class DBSCANpoints : public CharactersClustering
{
    public:
        DBSCANpoints(const float e);
        virtual ~DBSCANpoints(){};
        std::vector<Fields> clusterCharacters(const Characters *characters, const size_t charactersSize) override;

    private:
        const float eps;
        size_t size;
        std::vector<Point> points;
        std::vector<std::vector<size_t>> adjPoints;
        size_t clusterIdx = -1;
        std::vector<std::vector<size_t>> cluster;

        Fields fillField(float confidence, size_t labelSize, std::vector<Point> orderedCharacters);
        std::vector<Point> computePointsFromCharacters(const Characters *characters, const size_t charactersSize);
        std::vector<std::vector<size_t>> findNearPoints(std::vector<Point> points);
        std::vector<Point> dfs(size_t now, size_t c, std::vector<Point> &points, std::vector<std::vector<size_t>> &adjPoints);
        std::vector<std::vector<size_t>> assignCluster(std::vector<Point> points);
        std::vector<Fields> computeFields(std::vector<Point> points, std::vector<std::vector<size_t>> adjPoints);
        std::vector<Point> orderCharacters(std::multimap<float, Point> cluster);
        std::string extractLabel(std::vector<Point> orderedCharacters);
        Coordinates extractPosition(std::vector<Point> orderedCharacters);
};