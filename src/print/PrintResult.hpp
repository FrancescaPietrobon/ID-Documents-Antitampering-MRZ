#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

#include "ocr/Ocr/Ocr.hpp"
#include "charactersassociator/CharactersAssociator/CharactersAssociator.hpp"

void printOCRResult(cv::Mat img, Characters *characters, size_t charactersSize);
void printDbscanResult(cv::Mat img, Fields* fields, size_t fieldsSize);