#include "../include/Anchors.h"


Anchors::Anchors(int w, int h):
  width(w), height(h)
{
    for(float area: areas_dims)
        areas.push_back(std::pow(area, 2));

    for(float stride: strides_range)
        strides.push_back(std::pow(2, stride));

    Anchors::computeDims(); 
}


void Anchors::computeDims()
{
    std::vector<float> dim_scaled;

    float anchor_height, anchor_width;
    for(float area: areas)
    {
        for(float ratio: aspect_ratios)
        {
            anchor_height = std::sqrt(area / ratio);
            anchor_width = area / anchor_height;

            dims.push_back(anchor_width);
            dims.push_back(anchor_height);

            for(float scale: scales)
            {
                dim_scaled = {dims[0] * scale, dims[1] * scale};
                //std::cout << "dim_scaled[0]: " << dim_scaled[0] << " dim_scaled[1]: " << dim_scaled[1] << std::endl;
                anchor_dims.push_back(dim_scaled);
            }
            dims.clear();        
        }
        anchor_dims_all.push_back(anchor_dims);
        anchor_dims.clear(); 
    }
    //std::cout << anchor_dims_all.size() << " x " << anchor_dims_all[0].size() << " x " << anchor_dims_all[0][0].size() << std::endl;
}


matrix3D Anchors::meshgrid(int level)
{
  std::vector<float> point;
  matrix2D grid;
  matrix3D grid_rep;

  for(int i = 0; i < width / std::pow(2, level); ++i)
  {
    for(int j = 0; j < width / std::pow(2, level); ++j)
    {
      point = {static_cast<float>(j + RX_Y_PLUS) * static_cast<float>(std::pow(2, level)), static_cast<float>(i + RX_Y_PLUS) * static_cast<float>(std::pow(2, level))};
      //std::cout << point[0] << " " << point[1] << std::endl;
      for(int k = 0; k < 15; ++k)
        grid.push_back(point);
      grid_rep.push_back(grid);
      grid.clear();
    }
  }
//std::cout << grid_rep.size() << " x " << grid_rep[0].size() << " x " << grid_rep[0][0].size() << std::endl;
  return grid_rep;
}


void Anchors::printAnchors(matrix3D A, matrix3D B)
{
  for(size_t j = 0; j < A.size(); ++j)
    for(size_t i = 0; i < A[j].size(); ++i)
      std::cout << "h" << i << " = " << A[j][i][0] << "\t w" << i << " = " << A[j][i][1] << "\t x" << i << " = " << B[j][i][0] << "\t y" << i << " = " << B[j][i][1] << std::endl;
}


matrix2D Anchors::resizeAnchors(matrix3D A)
{
  matrix2D A_resized;
  for(size_t i = 0; i < A.size(); ++i)
    for(size_t j = 0; j < A[i].size(); ++j)
      A_resized.push_back(A[i][j]);

  return A_resized;
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


void Anchors::printAnchorShapesTransf(matrix3D grid, matrix3D area_exp, matrix3D concatenation, matrix2D resized_anchors, int level)
{
    std::cout << "\nLevel " << level - 3 << std::endl;

    std::cout << "Meshgrid: \t" << grid.size() << " x " << grid[0].size() << " x " << grid[0][0].size();
    std::cout << "\t (" << std::sqrt(grid.size()) << " x " << std::sqrt(grid.size()) << " x " << grid[0].size() << " x " << grid[0][0].size() << ")" << std::endl;

    std::cout << "Anchor sizes: \t" << area_exp.size() << " x " << area_exp[0].size() << " x " << area_exp[0][0].size();
    std::cout << "\t (" << std::sqrt(area_exp.size()) << " x " << std::sqrt(area_exp.size()) << " x " << area_exp[0].size() << " x " << area_exp[0][0].size() << ")" << std::endl;

    std::cout << "Anchors: \t"  << concatenation.size() << " x " << concatenation[0].size() << " x " << concatenation[0][0].size();
    std::cout << "\t (" << std::sqrt(concatenation.size()) << " x " << std::sqrt(concatenation.size()) << " x " << concatenation[0].size() << " x " << concatenation[0][0].size() << ")" << std::endl;
    
    std::cout << "Anchors_res: \t"  << resized_anchors.size() << " x " << resized_anchors[0].size();
    std::cout << "\t (" << std::sqrt(concatenation.size()) << " x " << std::sqrt(concatenation.size()) << " x " << concatenation[0].size() << " x " << concatenation[0][0].size() << ")" << std::endl;
    
}


matrix2D Anchors::anchorsGenerator()
{
    matrix2D all_anchors;

    for(int level: strides_range)
    {
        matrix3D grid = meshgrid(level);
        matrix2D area = anchor_dims_all[level - 3];

        matrix3D area_exp;
        for(int k = 0; k < ceil(width / std::pow(2, level))*ceil(height / std::pow(2, level)); ++k)
            area_exp.push_back(area);

        // To check that are the same with the ones considered in python
        //print_anchors(area_exp, grid);

        matrix3D concatenation = concatenate(area_exp, grid);
        matrix2D resized_anchors = resizeAnchors(concatenation);

        for(size_t i = 0; i < resized_anchors.size(); ++i)
            all_anchors.push_back(resized_anchors[i]);

        // To print the dimension for each matrix computation
        //printAnchorShapesTransf(grid, area_exp, concatenation, resized_anchors, level);
    
    }
    
    return all_anchors;
}