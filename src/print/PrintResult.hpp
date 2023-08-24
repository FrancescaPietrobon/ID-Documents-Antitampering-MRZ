#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

#include "ocr_server/ocr/OcrApi.hpp"
#include "ocr_server/charactersClustering/CharactersClusteringApi.hpp"
#include "antitampering/mrz/AntitamperingMrzApi.hpp"

void saveImgOcrResponse(cv::Mat img, Character *characters, size_t charactersSize);
void saveImgDbscanResponse(cv::Mat img, Field* fields, size_t fieldsSize);

void printDbscanResponse(ClusteringResponse clusteringResult);
void printAntitamperingMrzResponse(AntitamperingMrzResponse antitamperingMrzResponse);
