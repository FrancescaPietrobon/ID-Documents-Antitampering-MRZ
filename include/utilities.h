#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <vector>
#include <cctype>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "DBSCAN.h"

#define FEATURE_WIDTH 800
#define FEATURE_HEIGHT 800

typedef std::vector<std::vector<float>> matrix2D;

void savePredictionImage(cv::Mat img, matrix2D boxes, std::vector<float> classes, std::string img_name);
matrix2D computeCenters(matrix2D boxes, matrix2D anchorBoxes);

void saveCentersPredictionImage(cv::Mat img, std::vector<Character> centers, std::string img_name);

int countAlnums(const std::string& s);


#endif