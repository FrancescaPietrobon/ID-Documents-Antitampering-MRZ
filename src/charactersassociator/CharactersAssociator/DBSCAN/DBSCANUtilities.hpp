#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

#include "../CharactersAssociator.hpp"

void savePredictionImage(cv::Mat img, Fields* fields, size_t fieldsSize);