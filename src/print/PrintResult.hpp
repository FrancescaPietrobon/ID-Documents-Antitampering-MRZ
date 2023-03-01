#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

#include "ocr_server/OcrApi.hpp"
#include "charactersClustering/CharactersClusteringApi.hpp"
#include "antitampering/mrz/AntitamperingMrzApi.hpp"

void saveImgOcrResponse(cv::Mat img, Characters *characters, size_t charactersSize);
void saveImgDbscanResponse(cv::Mat img, Fields* fields, size_t fieldsSize);

void printDbscanResponse(ClusteringResponse clusteringResult);
void printAntitamperingMrzResponse(AntitamperingMrzResponse antitamperingMrzResponse);