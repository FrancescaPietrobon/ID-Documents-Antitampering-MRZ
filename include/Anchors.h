#ifndef ANCHORS_H
#define ANCHORS_H

#include <iostream>
#include <vector>
#include <numeric>
#include <math.h>
#include <algorithm>

#define FEATURE_WIDTH 800
#define FEATURE_HEIGHT 800
#define RX_Y_PLUS 0.5

typedef std::vector<std::vector<float>> matrix2D;
typedef std::vector<std::vector<std::vector<float>>> matrix3D;

class Anchors
{
    public:
        Anchors();
        void computeDims();
        matrix3D meshgrid(int level);
        void printAnchors(matrix3D A, matrix3D B);
        matrix2D resizeAnchors(matrix3D A);
        matrix3D concatenate(matrix3D A, matrix3D B);
        matrix2D anchorsGenerator();
        void printAnchorShapesTransf(matrix3D grid, matrix3D area_exp, matrix3D concatenation, matrix2D resized_anchors, int level);

    private:
        std::vector<float> aspect_ratios = {0.2, 0.3, 0.5, 1.0, 2.0};
        std::vector<float> scales = {0.4, 0.6, 0.8};
        std::vector<float> areas_dims = {32, 64, 128, 256, 512};
        std::vector<float> strides_range = {3, 4, 5, 6, 7};

        std::vector<float> areas;
        std::vector<float> strides;
        std::vector<float> dims;
        matrix2D anchor_dims;
        matrix3D anchor_dims_all;

};

#endif