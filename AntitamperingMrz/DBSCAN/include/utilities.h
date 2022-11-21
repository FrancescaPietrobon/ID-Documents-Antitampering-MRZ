#ifndef MYUTILITIES_H
#define MYUTILITIES_H

#include <iostream>
#include <vector>
#include <cctype>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "Character.h"
#include "Document.h"
#include "XMLBoxes.h"
#include "ModelBoxes.h"
#include "Anchors.h"
#include "Field.h"
#include "Fields.h"

typedef std::vector<std::vector<float>> matrix2D;

void savePredictionImage(cv::Mat img, matrix2D boxes, std::vector<float> classes, std::string img_name);

void saveCentersPredictionImage(cv::Mat img, std::vector<Character> centers, std::string img_name);

bool isNumber(const std::string& str);

int countAlnums(const std::string& s);


#endif