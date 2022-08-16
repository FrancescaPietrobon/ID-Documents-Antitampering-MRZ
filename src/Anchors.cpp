#include "Anchors.h"


Anchors::Anchors()
{
    for(double area: areas_dims)
        areas.push_back(std::pow(area, 2));

    for(double stride: strides_range)
        strides.push_back(std::pow(2, stride));

    Anchors::computeDims(); 
}


void Anchors::computeDims()
{
    std::vector<double> dim_scaled;

    float anchor_height, anchor_width;
    for(double area: areas)
    {
        for(double ratio: aspect_ratios)
        {
            anchor_height = std::sqrt(area / ratio);
            anchor_width = area / anchor_height;

            dims.push_back(anchor_width);
            dims.push_back(anchor_height);

            for(double scale: scales)
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
  std::vector<double> point;
  matrix2D grid;
  matrix3D grid_rep;

  for(int i = 0; i < FEATURE_WIDTH / std::pow(2, level); ++i)
  {
    for(int j = 0; j < FEATURE_WIDTH / std::pow(2, level); ++j)
    {
      point = {(j + 0.25)*std::pow(2, level), (i + 0.25)*std::pow(2, level)};
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
  for(int j = 0; j < A.size(); ++j)
    for(int i = 0; i < A[j].size(); ++i)
      std::cout << "h" << i << " = " << A[j][i][0] << "\t w" << i << " = " << A[j][i][1] << "\t x" << i << " = " << B[j][i][0] << "\t y" << i << " = " << B[j][i][1] << std::endl;
}


matrix2D Anchors::resizeAnchors(matrix3D A)
{
  matrix2D A_resized;
  for(int i = 0; i < A.size(); ++i)
    for(int j = 0; j < A[i].size(); ++j)
      A_resized.push_back(A[i][j]);

  return A_resized;
}


matrix3D Anchors::concatenate(matrix3D A, matrix3D B)
{
  matrix3D concatenation = A;
  std::vector<double> anchor;
  
  for(int i = 0; i < A.size(); ++i)
  {
    for(int j = 0; j < A[i].size(); ++j)
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


void Anchors::anchorsGenerator()
{
    matrix2D all_anchors;

    for(int level: strides_range)
    {
        matrix3D grid = meshgrid(level);
        matrix2D area = anchor_dims_all[level - 3];

        matrix3D area_exp;
        for(int k = 0; k < ceil(FEATURE_WIDTH / std::pow(2, level))*ceil(FEATURE_HEIGHT / std::pow(2, level)); ++k)
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
    
    std::cout << "All anchors: \t"  << all_anchors.size() << " x " << all_anchors[0].size() << std::endl;
}



/* UNUSED 

void Anchors::meshgrid_opencv(cv::Mat &X, cv::Mat &Y)
{
    std::vector<double> t_x, t_y;
    for(int i = 0; i < FEATURE_WIDTH / std::pow(2,3); i++) t_x.push_back(i + 0.5);
    for(int j = 0; j < FEATURE_HEIGHT / std::pow(2,3); j++) t_y.push_back(j + 0.5);

    cv::repeat(cv::Mat(t_x).t(), t_y.size(), 1, X);
    cv::repeat(cv::Mat(t_y), 1, t_x.size(), Y);
    
    //for(unsigned i = 0; i < X.rows; ++i)
    //  for(unsigned j = 0; i < X.cols; ++j)
    //    std::cout << X.at(i,j) << std::endl;
    
}


void Anchors::meshgrid_old(matrix3D &X, matrix3D &Y, int level)
{
  std::vector<double> t_x, t_y;
  matrix2D X2, Y2;

  for(int i = 0; i < FEATURE_WIDTH / std::pow(2, level); i++)
  {
    for(int j = 0; j < FEATURE_WIDTH / std::pow(2, level); j++)
      t_x.push_back((i + 0.5)*std::pow(2, level));
    X2.push_back(t_x);
  }
  for(int k = 0; k < 15; k++)
    X.push_back(X2);


  for(int j = 0; j < FEATURE_HEIGHT / std::pow(2, level); j++)
    t_y.push_back((j + 0.5)*std::pow(2, level));
  for(int j = 0; j < FEATURE_HEIGHT / std::pow(2, level); j++)
    Y2.push_back(t_y);
  for(int k = 0; k < 15; k++)
    Y.push_back(Y2);

}


// In AnchorsGenerator

    //int from_to[] = { 0,0,0, 1,1,1};
    //cv::mixChannels( in, 3, &tot, 1, from_to,2);
    
    //centers.push_back(Y_red, );
    //std::cout << centers.size[0] << " " << centers.size[1] << " " << centers.size[2] << std::endl;
    //std::vector<int> dim = {100, 100, 2};
    //centers.reshape(3, dim);
    //std::cout << centers.size[0] << " " << centers.size[1] << " " << centers.size[2] << std::endl;
    //cv::vconcat(X, Y, centers);
    //std::cout << centers.size() << std::endl;
    //std::vector<int> dim2 = {100, 100, 1, 1};
    //centers.reshape(4, dim2);
    //std::cout << centers.size() << std::endl;
    //centers = tf.stack(tf.meshgrid(rx, ry), axis=-1) * self._strides[level - 3]
    //centers = tf.expand_dims(centers, axis=-2)
    //centers = tf.tile(centers, [1, 1, self._num_anchors, 1])
    //dims = tf.tile(self._anchor_dims[level - 3], [feature_height, feature_width, 1, 1])
    //anchors = tf.concat([centers, dims], axis=-1)

    

*/

/*
    std::vector<float> rx(FEATURE_WIDTH);
    std::iota(std::begin(rx), std::end(rx), 1);
    std::for_each(rx.begin(), rx.end(), [](float& d) { d+=0.5;});

    std::vector<float> ry(FEATURE_HEIGHT);
    std::iota(std::begin(ry), std::end(ry), 1);
    std::for_each(ry.begin(), ry.end(), [](float& d) { d+=0.5;});

    for(unsigned i = 0; i < rx.size(); ++i)
        std::cout << rx[i] << std::endl;
    */