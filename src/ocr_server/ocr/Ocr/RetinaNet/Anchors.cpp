#include "Anchors.hpp"

Anchors::Anchors(int w, int h):
    width(w), height(h)
{
    for(float area: areasDims)
        this->areas.push_back(std::pow(area, 2));

    for(float stride: stridesRange)
        this->strides.push_back(std::pow(2, stride));

    Anchors::computeDims(); 
}

void Anchors::computeDims()
{
    std::vector<float> dim_scaled;
    float anchorHeight, anchorWidth;
    for(float area: areas)
    {
        for(float ratio: aspectRatios)
        {
            anchorHeight = std::sqrt(area / ratio);
            anchorWidth = area / anchorHeight;

            this->dims.push_back(anchorWidth);
            this->dims.push_back(anchorHeight);

            for(float scale: scales)
            {
                dim_scaled = {dims[0] * scale, dims[1] * scale};
                this->anchorDims.push_back(dim_scaled);
            }
            this->dims.clear();        
        }
        this->anchorDimsAll.push_back(anchorDims);
        this->anchorDims.clear(); 
    }

    SPDLOG_DEBUG("Size anchorDimsAll: {} x {} x {}", anchorDimsAll.size(), anchorDimsAll[0].size(), anchorDimsAll[0][0].size());
}

matrix2D Anchors::anchorsGenerator()
{
    matrix2D allAnchors;
    for(int level: this->stridesRange)
    {
        matrix3D grid = this->meshgrid(level);
        matrix2D area = anchorDimsAll[level - 3];
        matrix3D areaExp;
        for(int k = 0; k < ceil(width / std::pow(2, level))*ceil(height / std::pow(2, level)); ++k)
            areaExp.push_back(area);

        // To check that are the same with the ones considered in python
        //print_anchors(areaExp, grid);

        matrix3D concatenation = this->concatenate(areaExp, grid);

        /*
        //matrix3D input1 = {{9, 7}, {4, 1}, {8, 3}};
        //matrix3D input2 = {{3, 2}, {1, 7}, {0, 6}};

        std::vector<std::vector<std::vector<float>>> input1 = {{{8., 9.}, {7., 2.}},
                                                           {{5., 4.}, {1., 7.}}};
        std::vector<std::vector<std::vector<float>>> input2 = {{{7., 3.}, {2., 2.}},
                                                           {{1., 4.}, {7., 3.}}};

        matrix3D res = this->concatenate(input1, input2);
        std::cout << res.size() << std::endl;
        std::cout << res[0].size() << std::endl;
        std::cout << res[0][0][0] << std::endl;
        std::cout << res[1][0][1] << std::endl;
        */

        matrix2D resizedAnchors = this->resizeAnchors(concatenation);

        matrix3D input1 = {{{8., 9.}, {7., 2.}},
                           {{5., 4.}, {1., 7.}}};
        matrix2D res = this->resizeAnchors(input1);
        /*
        std::cout << res.size() << std::endl;
        std::cout << res[0].size() << std::endl;
        std::cout << res[0][0] << std::endl;
        std::cout << res[1][0] << std::endl;
        */

        for(size_t i = 0; i < resizedAnchors.size(); ++i)
            allAnchors.push_back(resizedAnchors[i]);

        // To print the dimension for each matrix computation
        //this->printAnchorShapesTransf(grid, areaExp, concatenation, resizedAnchors, level);
    }
    return allAnchors;
}

matrix3D Anchors::meshgrid(int level)
{
    std::vector<float> point;
    matrix2D grid;
    matrix3D gridRep;
    for(int i = 0; i < width / std::pow(2, level); ++i)
    {
        for(int j = 0; j < width / std::pow(2, level); ++j)
        {
            point = {static_cast<float>(j + RX_Y_PLUS) * static_cast<float>(std::pow(2, level)),
                     static_cast<float>(i + RX_Y_PLUS) * static_cast<float>(std::pow(2, level))};
            for(int k = 0; k < 15; ++k)
                grid.push_back(point);
            gridRep.push_back(grid);
            grid.clear();
        }
    }
    SPDLOG_DEBUG("Size gridRep: {} x {} x {}", gridRep.size(), gridRep[0].size(), gridRep[0][0].size());
    return gridRep;
}

matrix3D Anchors::concatenate(matrix3D A, matrix3D B)
{
    matrix3D concatenation = A;
    std::vector<float> anchor;
    for(size_t i = 0; i < A.size(); ++i)
    {
        for(size_t j = 0; j < A[i].size(); ++j)
        {
            anchor = {B[i][j][0], B[i][j][1], A[i][j][0], A[i][j][1]};
            concatenation[i][j] = anchor;
        }
    }
    return concatenation;
}

matrix2D Anchors::resizeAnchors(matrix3D A)
{
    matrix2D A_resized;
    for(size_t i = 0; i < A.size(); ++i)
        for(size_t j = 0; j < A[i].size(); ++j)
            A_resized.push_back(A[i][j]);
    
    //std::cout << A.size() << std::endl;
    //std::cout << A[0].size() << std::endl;
    //std::cout << A_resized[0][0] << std::endl;
    //std::cout << A_resized[1][50] << std::endl;
    return A_resized;
}

void Anchors::printAnchors(matrix3D A, matrix3D B)
{
    for(size_t j = 0; j < A.size(); ++j)
        for(size_t i = 0; i < A[j].size(); ++i)
            SPDLOG_DEBUG("h{} = {} \t w{} = {} \t x{} = {} \t y{} = {}", i, A[j][i][0], i, A[j][i][1], i, B[j][i][0], i, B[j][i][1]);
}

void Anchors::printAnchorShapesTransf(matrix3D grid, matrix3D areaExp, matrix3D concatenation, matrix2D resizedAnchors, int level)
{
    SPDLOG_DEBUG("Level {}", level - 3);
    SPDLOG_DEBUG("Meshgrid: \t {} x {} x {} \t ({} x {} x {} x {})",
                 grid.size(), grid[0].size(), grid[0][0].size(),
                 std::sqrt(grid.size()), std::sqrt(grid.size()), grid[0].size(), grid[0][0].size());
    SPDLOG_DEBUG("Anchor sizes: \t {} x {} x {} \t ({} x {} x {} x {})",
                 areaExp.size(), areaExp[0].size(), areaExp[0][0].size(),
                 std::sqrt(areaExp.size()), std::sqrt(areaExp.size()), areaExp[0].size(), areaExp[0][0].size());
    SPDLOG_DEBUG("Anchors: \t {} x {} x {} \t ({} x {} x {} x {})",
                 concatenation.size(), concatenation[0].size(), concatenation[0][0].size(),
                 std::sqrt(concatenation.size()), std::sqrt(concatenation.size()), concatenation[0].size(), concatenation[0][0].size());
    SPDLOG_DEBUG("Anchors_res: \t {} x {} \t ({} x {} x {} x {})",
                 resizedAnchors.size(), resizedAnchors[0].size(),
                 std::sqrt(concatenation.size()), std::sqrt(concatenation.size()), concatenation[0].size(), concatenation[0][0].size());
}
