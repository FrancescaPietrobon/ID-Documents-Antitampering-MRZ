#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>

#define FEATURE_WIDTH 800
#define FEATURE_HEIGHT 800

typedef std::vector<std::vector<float>> matrix2D;
typedef std::vector<std::vector<std::vector<float>>> matrix3D;
typedef std::vector<std::vector<std::vector<std::vector<float>>>> matrix4D;

void savePredictionImage(cv::Mat img, matrix2D boxes, std::vector<float> classes, std::string img_name);
matrix2D extractPredCVMat(cv::Mat boxPrediction);
void applySigmoid(matrix2D &A);

matrix2D computeCenters(matrix2D boxes, matrix2D anchorBoxes);
matrix2D computeCorners(matrix2D boxes);
void extractImageWithPrediction(cv::Mat img, matrix2D boxes, matrix2D pred, float threshold, float threshold_nms);

#endif