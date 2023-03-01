#include "Anchors.hpp"

Anchors::Anchors(int w, int h):
  width(w), height(h)
{
    for(float area: areasDims)
        areas.push_back(std::pow(area, 2));

    for(float stride: stridesRange)
        strides.push_back(std::pow(2, stride));

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

            dims.push_back(anchorWidth);
            dims.push_back(anchorHeight);

            for(float scale: scales)
            {
                dim_scaled = {dims[0] * scale, dims[1] * scale};
                //std::cout << "dim_scaled[0]: " << dim_scaled[0] << " dim_scaled[1]: " << dim_scaled[1] << std::endl;
                anchorDims.push_back(dim_scaled);
            }
            dims.clear();        
        }
        anchorDimsAll.push_back(anchorDims);
        anchorDims.clear(); 
    }
    //std::cout << anchorDimsAll.size() << " x " << anchorDimsAll[0].size() << " x " << anchorDimsAll[0][0].size() << std::endl;
}

matrix2D Anchors::anchorsGenerator()
{
    matrix2D allAnchors;
    for(int level: stridesRange)
    {
        matrix3D grid = meshgrid(level);
        matrix2D area = anchorDimsAll[level - 3];
        matrix3D areaExp;
        for(int k = 0; k < ceil(width / std::pow(2, level))*ceil(height / std::pow(2, level)); ++k)
            areaExp.push_back(area);

        // To check that are the same with the ones considered in python
        //print_anchors(areaExp, grid);

        matrix3D concatenation = concatenate(areaExp, grid);
        matrix2D resizedAnchors = resizeAnchors(concatenation);

        for(size_t i = 0; i < resizedAnchors.size(); ++i)
            allAnchors.push_back(resizedAnchors[i]);

        // To print the dimension for each matrix computation
        //printAnchorShapesTransf(grid, areaExp, concatenation, resizedAnchors, level);
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
      point = {static_cast<float>(j + RX_Y_PLUS) * static_cast<float>(std::pow(2, level)), static_cast<float>(i + RX_Y_PLUS) * static_cast<float>(std::pow(2, level))};
      //std::cout << point[0] << " " << point[1] << std::endl;
      for(int k = 0; k < 15; ++k)
        grid.push_back(point);
      gridRep.push_back(grid);
      grid.clear();
    }
  }
  //std::cout << gridRep.size() << " x " << gridRep[0].size() << " x " << gridRep[0][0].size() << std::endl;
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
  return A_resized;
}

void Anchors::printAnchors(matrix3D A, matrix3D B)
{
  for(size_t j = 0; j < A.size(); ++j)
    for(size_t i = 0; i < A[j].size(); ++i)
      std::cout << "h" << i << " = " << A[j][i][0] << "\t w" << i << " = " << A[j][i][1] << "\t x" << i << " = " << B[j][i][0] << "\t y" << i << " = " << B[j][i][1] << std::endl;
}

void Anchors::printAnchorShapesTransf(matrix3D grid, matrix3D areaExp, matrix3D concatenation, matrix2D resizedAnchors, int level)
{
    std::cout << "\nLevel " << level - 3 << std::endl;

    std::cout << "Meshgrid: \t" << grid.size() << " x " << grid[0].size() << " x " << grid[0][0].size();
    std::cout << "\t (" << std::sqrt(grid.size()) << " x " << std::sqrt(grid.size()) << " x " << grid[0].size() << " x " << grid[0][0].size() << ")" << std::endl;

    std::cout << "Anchor sizes: \t" << areaExp.size() << " x " << areaExp[0].size() << " x " << areaExp[0][0].size();
    std::cout << "\t (" << std::sqrt(areaExp.size()) << " x " << std::sqrt(areaExp.size()) << " x " << areaExp[0].size() << " x " << areaExp[0][0].size() << ")" << std::endl;

    std::cout << "Anchors: \t"  << concatenation.size() << " x " << concatenation[0].size() << " x " << concatenation[0][0].size();
    std::cout << "\t (" << std::sqrt(concatenation.size()) << " x " << std::sqrt(concatenation.size()) << " x " << concatenation[0].size() << " x " << concatenation[0][0].size() << ")" << std::endl;
    
    std::cout << "Anchors_res: \t"  << resizedAnchors.size() << " x " << resizedAnchors[0].size();
    std::cout << "\t (" << std::sqrt(concatenation.size()) << " x " << std::sqrt(concatenation.size()) << " x " << concatenation[0].size() << " x " << concatenation[0][0].size() << ")" << std::endl;  
}