#ifndef ANCHORS_H_
#define ANCHORS_H_

#include <iostream>
#include <vector>
#include <numeric>
#include <math.h>
#include <algorithm>

#define FEATURE_WIDTH 800
#define FEATURE_HEIGHT 800

typedef std::vector<std::vector<double>> matrix2D;
typedef std::vector<std::vector<std::vector<double>>> matrix3D;
typedef std::vector<std::vector<std::vector<std::vector<double>>>> matrix4D;

class Anchors
{
    public:
        Anchors();
        void computeDims();
        matrix3D meshgrid(int level);
        void printAnchors(matrix3D A, matrix3D B);
        matrix2D resizeAnchors(matrix3D A);
        matrix3D concatenate(matrix3D A, matrix3D B);
        void anchorsGenerator();
        void printAnchorShapesTransf(matrix3D grid, matrix3D area_exp, matrix3D concatenation, matrix2D resized_anchors, int level);

    private:
        std::vector<double> aspect_ratios = {0.2, 0.3, 0.5, 1.0, 2.0};
        std::vector<double> scales = {0.4, 0.6, 0.8};
        std::vector<double> areas_dims = {32, 64, 128, 256, 512};
        std::vector<double> strides_range = {3, 4, 5, 6, 7};

        std::vector<double> areas;
        std::vector<double> strides;
        std::vector<double> dims;
        matrix2D anchor_dims;
        matrix3D anchor_dims_all;

};

#endif