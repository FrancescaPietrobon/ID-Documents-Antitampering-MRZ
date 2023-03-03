#pragma once

#include <iostream>
#include <vector>
#include <numeric>
#include <math.h>
#include <algorithm>
#include <spdlog/spdlog.h>

// Meshgrid
#define RX_Y_PLUS 0.5

typedef std::vector<std::vector<float>> matrix2D;
typedef std::vector<std::vector<std::vector<float>>> matrix3D;

class Anchors
{
    private:
        const std::vector<float> aspectRatios = {0.25, 0.42, 0.59, 0.76, 1.0};
        const std::vector<float> scales = {0.4, 0.6, 0.8};
        const std::vector<float> areasDims = {30., 40., 50., 60., 70.};
        const std::vector<float> stridesRange = {3, 4, 5, 6, 7};
        int width;
        int height;
        std::vector<float> areas;
        std::vector<float> strides;
        std::vector<float> dims;
        matrix2D anchorDims;
        matrix3D anchorDimsAll;

    public:
        Anchors(int width, int height);
        void computeDims();
        matrix3D meshgrid(int level);
        matrix2D resizeAnchors(matrix3D A);
        matrix3D concatenate(matrix3D A, matrix3D B);
        matrix2D anchorsGenerator();

        void printAnchors(matrix3D A, matrix3D B);
        void printAnchorShapesTransf(matrix3D grid, matrix3D areaExp, matrix3D concatenation, matrix2D resizedAnchors, int level);
};