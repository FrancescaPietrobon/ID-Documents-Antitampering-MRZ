#ifndef _UTILITIES_RETINANET_HPP_
#define _UTILITIES_RETINANET_HPP_

#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "../Ocr.hpp"

#include <vector>

typedef std::vector<std::vector<float>> matrix2D;

matrix2D CVMatToMatrix2D(cv::Mat cvMat);
matrix2D multVariance(matrix2D box, std::vector<float> variance);
matrix2D applySigmoid(matrix2D A);
Coordinates reshapeBox(std::vector<float> box, float xAlter, float yAlter);
matrix2D convertToConers(matrix2D boxes);

void printPredictions(matrix2D pred);
void savePredictionImage(cv::Mat img, std::vector<OcrData> characters);

#endif