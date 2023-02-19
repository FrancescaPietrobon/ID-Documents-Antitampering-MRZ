#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

#include "charactersassociator/CharactersAssociator/CharactersAssociator.hpp"

void printDbscanResult(cv::Mat img, Fields* fields, size_t fieldsSize);