#ifndef _OCR_UTILS_HPP_
#define _OCR_UTILS_HPP_

#include "../exceptions/Exceptions.hpp"
#include "../../Base64/base64.h"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

cv::Mat fromBase64toCvMat(const char* imageBase64)
{
    SPDLOG_INFO("Decoding base64 Image");
    std::string imageStr = base64_decode(imageBase64);
    std::vector<char> data(imageStr.begin(), imageStr.end());
    cv::Mat image = cv::imdecode(data, cv::IMREAD_COLOR);
    if(image.empty())
    {
        SPDLOG_ERROR("Error decoding image");
        throw OcrException(OcrErrorCode::CORRUPTED_IMAGE, "Base64 of the provided image is corrupted and it cannot be decoded");   
    }
    SPDLOG_INFO("Image Decoded");
    return image;
}

bool checkCoordinatesValue(const Coordinates coordinates)
{
    SPDLOG_INFO("Checking Coordinates");
    return !((coordinates.bottomRightX < 0) || (coordinates.bottomRightY < 0) || (coordinates.topLeftX < 0) || (coordinates.topLeftY < 0));
}

cv::Rect fromCoordinatesToCvRect(const Coordinates coordinates)
{
    int x = coordinates.topLeftX;
    int y = coordinates.topLeftY;
    int width = coordinates.bottomRightX - coordinates.topLeftX;
    int height = coordinates.bottomRightY - coordinates.topLeftY;
    return cv::Rect(x, y, width, height);
}

Coordinates fromCvRectToCoordinates(const cv::Rect rect)
{
    Coordinates coordinates;
    coordinates.topLeftX = rect.x;
    coordinates.topLeftY = rect.y;
    coordinates.bottomRightX = coordinates.topLeftX + rect.width;
    coordinates.bottomRightY = coordinates.topLeftY + rect.height;
    return coordinates;
}

cv::Mat prepareImage(const Coordinates coordinates, const cv::Mat& image)
{
    SPDLOG_INFO("Preparing Image");
    cv::Mat preparedImage = image;
    if(checkCoordinatesValue(coordinates))
    {
        cv::Rect imageBound = cv::Rect(0, 0, image.cols, image.rows);
        cv::Rect faceRect = fromCoordinatesToCvRect(coordinates);
        cv::Rect safeFaceRect = imageBound & faceRect;
        preparedImage = image(safeFaceRect);
        SPDLOG_INFO("Image Cropped");
    }
    return preparedImage;
}

#endif