#pragma once

#include "../exceptions/Exceptions.hpp"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include "base64/base64.h"
#include <spdlog/spdlog.h>

struct Coordinates
{
    float topLeftX;
    float topLeftY;
    float bottomRightX;
    float bottomRightY;
};

struct Field {
    char* label;
    size_t labelSize;
    Coordinates position;
    float confidence;

    bool operator==(Field const & rhs) const {
        return (this->label == rhs.label && this->labelSize == rhs.labelSize && this->confidence == rhs.confidence);
    }
};

namespace utils
{
    static std::unordered_map<unsigned, std::string> dictionary = {
        {0, "0",}, {1, "1",}, {2, "2",}, {3, "3",}, {4, "4",}, {5, "5",}, {6, "6",}, {7, "7",}, {8, "8",}, {9, "9",},
        {10, "A",}, {11, "a",}, {12, "B",}, {13, "b",}, {14, "C",}, {15, "c",}, {16, "D",}, {17, "d",}, {18, "E",}, {19, "e",},
        {20, "F",}, {21, "f",}, {22, "G",}, {23, "g",}, {24, "H",}, {25, "h",}, {26, "I",}, {27, "i",}, {28, "J",}, {29, "j",},
        {30, "K",}, {31, "k",}, {32, "L",}, {33, "l",}, {34, "M",}, {35, "m",}, {36, "N",}, {37, "n",}, {38, "O",}, {39, "o",},
        {40, "P",}, {41, "p",}, {42, "Q",}, {43, "q",}, {44, "R",}, {45, "r",}, {46, "S",}, {47, "s",}, {48, "T",}, {49, "t",},
        {50, "U",}, {51, "u",}, {52, "V",}, {53, "v",}, {54, "W",}, {55, "w",}, {56, "X",}, {57, "x",}, {58, "Y",}, {59, "y",},
        {60, "Z",}, {61, "z",}, {62, "<",}
    };

    inline cv::Mat fromBase64toCvMat(const char *imageBase64)
    {
        SPDLOG_INFO("Decoding base64 Image");
        std::string imageStr = base64_decode(imageBase64);
        std::vector<char> data(imageStr.begin(), imageStr.end());
        cv::Mat image = cv::imdecode(data, cv::IMREAD_COLOR);
        if (image.empty())
        {
            SPDLOG_ERROR("Error decoding image");
            throw Exception(ErrorCode::CORRUPTED_IMAGE, "Base64 of the provided image is corrupted and it cannot be decoded");
        }
        SPDLOG_INFO("Image Decoded");
        return image;
    }

    inline bool checkCoordinatesValue(const Coordinates coordinates)
    {
        SPDLOG_INFO("Checking Coordinates");
        return !((coordinates.bottomRightX < 0) || (coordinates.bottomRightY < 0) || (coordinates.topLeftX < 0) || (coordinates.topLeftY < 0));
    }

    inline cv::Rect2d fromCoordinatesToCvRect(const Coordinates coordinates)
    {
        float x = coordinates.topLeftX;
        float y = coordinates.topLeftY;
        float width = coordinates.bottomRightX - coordinates.topLeftX;
        float height = coordinates.bottomRightY - coordinates.topLeftY;
        return cv::Rect2d(x, y, width, height);
    }

    inline Coordinates fromCvRectToCoordinates(const cv::Rect rect)
    {
        Coordinates coordinates;
        coordinates.topLeftX = rect.x;
        coordinates.topLeftY = rect.y;
        coordinates.bottomRightX = coordinates.topLeftX + rect.width;
        coordinates.bottomRightY = coordinates.topLeftY + rect.height;
        return coordinates;
    }

    inline cv::Mat prepareImage(const Coordinates coordinates, const cv::Mat &image)
    {
        SPDLOG_INFO("Preparing Image");
        cv::Mat preparedImage = image;
        if (checkCoordinatesValue(coordinates))
        {
            cv::Rect imageBound = cv::Rect(0, 0, image.cols, image.rows);
            cv::Rect faceRect = fromCoordinatesToCvRect(coordinates);
            cv::Rect safeFaceRect = imageBound & faceRect;
            preparedImage = image(safeFaceRect);
            SPDLOG_INFO("Image Cropped");
        }
        return preparedImage;
    }

    inline char *convertStringtoCharPtr(std::string str)
    {
        char *cstr = new char[str.length() + 1];
        strcpy(cstr, str.c_str());
        return cstr;
    }
}