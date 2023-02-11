#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../../TestHelper.hpp"
#include "ocr/OcrApi.hpp"
#include "ocr/Ocr/Ocr.hpp"
#include "ocr/Ocr/RetinaNet/OcrRetinaNet.hpp"
#include "ocr/OcrFactory.hpp"
#include "ocr/exceptions/Exceptions.hpp"
#include "base64/base64.h"

#include <spdlog/cfg/env.h>

class MockOcr : public Ocr
{
    public:
        //MOCK_METHOD(OcrResponse, process, (char **arr_image, char **arr_content_type, char **arr_content_base64, Coordinates *arr_coordinates, float *arr_confidence_threshold, size_t arr_size), (override));
        MOCK_METHOD(std::vector<Characters>, detect, (const cv::Mat image, const float confidenceThreshold), (override));
};

class OcrTestFixture : public ::testing::Test {
    protected:
    
    virtual void SetUp() {
        spdlog::cfg::load_env_levels();
    }

    std::string getBase64(cv::Mat imagePath);

    virtual void TearDown() {
    }

};

std::string OcrTestFixture::getBase64(cv::Mat image)
{
    std::vector<uchar> buffer;
    cv::imencode(".png", image, buffer);
    std::string imageBase64 = base64_encode(buffer.data(), buffer.size());
    return imageBase64;
}


// TEST OF UTILITIES

TEST_F(OcrTestFixture, CVMatToMatrix2DUnitTest)
{
    const int sz[] = {1, 2, 1, 4}; 
    float data[10] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    cv::Mat cvMat = cv::Mat(4, sz, CV_32F, data);
    matrix2D funcMatrix = CVMatToMatrix2D(cvMat);

    std::vector<float> row1 = {1, 2, 3, 4};
    std::vector<float> row2 = {5, 6, 7, 8};
    matrix2D myMatrix = {row1, row2};
    
    ASSERT_TRUE(myMatrix == funcMatrix) << TestHelper::PrintTo();
}

TEST_F(OcrTestFixture, convertToConersUnitTest)
{
    std::vector<float> xywhBox1 = {2, 2, 2, 2};
    std::vector<float> xywhBox2 = {3, 4, 2, 4};
    matrix2D xywhBoxes = {xywhBox1, xywhBox2};
    matrix2D funcCornersBoxes = convertToConers(xywhBoxes);

    std::vector<float> cornersBox1 = {1, 1, 3, 3};
    std::vector<float> cornersBox2 = {2, 2, 4, 6};
    matrix2D myCornersBoxes = {cornersBox1, cornersBox2};

    ASSERT_TRUE(myCornersBoxes == funcCornersBoxes) << TestHelper::PrintTo();
}

TEST_F(OcrTestFixture, reshapeBoxUnitTest)
{
    std::vector<float> box = {2, 2, 2, 2};
    float xAlter = 2;
    float yAlter = 1;
    Coordinates coords = reshapeBox(box, xAlter, yAlter);

    ASSERT_TRUE(coords.topLeftX == 1) << TestHelper::PrintTo();
    ASSERT_TRUE(coords.topLeftY == 2) << TestHelper::PrintTo();
    ASSERT_TRUE(coords.bottomRightX == 1) << TestHelper::PrintTo();
    ASSERT_TRUE(coords.bottomRightY == 2) << TestHelper::PrintTo();
}

TEST_F(OcrTestFixture, multVarianceUnitTest)
{
    std::vector<float> box1 = {2, 2, 2, 2};
    std::vector<float> box2 = {3, 4, 2, 4};
    matrix2D boxes = {box1, box2};
    std::vector<float> variance = {0.1, 0.1, 0.2, 0.2};
    matrix2D funcBoxes = multVariance(boxes, variance);

    std::vector<float> myBox1 = {0.2, 0.2, 0.4, 0.4};
    std::vector<float> myBox2 = {0.3, 0.4, 0.4, 0.8};
    matrix2D myBoxes = {myBox1, myBox2};

    ASSERT_TRUE(myBoxes == funcBoxes) << TestHelper::PrintTo();
}

TEST_F(OcrTestFixture, applySigmoidUnitTest)
{
    std::vector<float> predClass1 = {0.9, 0.1, 0.15};
    std::vector<float> predClass2 = {0.2, 0.7, 0.3};
    matrix2D predClasses = {predClass1, predClass2};
    matrix2D funcPredClasses = applySigmoid(predClasses);

    float threshold = 0.0001;
    ASSERT_TRUE(abs(funcPredClasses[0][0] - 0.710949) < threshold) << TestHelper::PrintTo();
    ASSERT_TRUE(abs(funcPredClasses[0][1] - 0.524979) < threshold) << TestHelper::PrintTo();
    ASSERT_TRUE(abs(funcPredClasses[0][2] - 0.53743) < threshold) << TestHelper::PrintTo();
    ASSERT_TRUE(abs(funcPredClasses[1][0] - 0.549834) < threshold) << TestHelper::PrintTo();
    ASSERT_TRUE(abs(funcPredClasses[1][1] - 0.668188) < threshold) << TestHelper::PrintTo();
    ASSERT_TRUE(abs(funcPredClasses[1][2] - 0.574443) < threshold) << TestHelper::PrintTo();
}


// TESTS OF DETECTION

TEST_F(OcrTestFixture, CheckPredictionUnitTest)
{
    char **images = new char *[1];
    images[0] = convertStringtoCharPtr("images0");
    char **contentType = new char *[1];
    contentType[0] = convertStringtoCharPtr("image/jpeg");
    char **contentBase64 = new char *[1];
    contentBase64[0] = convertStringtoCharPtr(getBase64(cv::imread("testData/images/AFG_AO_01001_FRONT.JPG")));
    Coordinates *coordinates = new Coordinates[1];
    coordinates[0] = Coordinates{0, 0, 800, 800};
    float *thresholds = new float[1];
    thresholds[0] = 0.3;
    char *algoType = new char;
    algoType = convertStringtoCharPtr("RetinaNet");

    OcrResponse result = process(images, contentType, contentBase64, coordinates, thresholds, 1, algoType);

    float threshold = 0.001;
    ASSERT_TRUE(result.resultDetails[0].charactersSize == 185) << TestHelper::PrintTo();
    ASSERT_TRUE(result.resultDetails[0].characters[0].label == '<') << TestHelper::PrintTo();
    ASSERT_TRUE(result.resultDetails[0].characters[0].labelIndex == 62) << TestHelper::PrintTo();
    ASSERT_TRUE(abs(result.resultDetails[0].characters[0].position.topLeftX - 382.914) < threshold) << TestHelper::PrintTo();
    ASSERT_TRUE(abs(result.resultDetails[0].characters[0].position.topLeftY - 381.942) < threshold) << TestHelper::PrintTo();
    ASSERT_TRUE(abs(result.resultDetails[0].characters[0].position.bottomRightX - 394.38) < threshold) << TestHelper::PrintTo();
    ASSERT_TRUE(abs(result.resultDetails[0].characters[0].position.bottomRightY - 394.204) < threshold) << TestHelper::PrintTo();
    ASSERT_TRUE(abs(result.resultDetails[0].characters[0].confidence - 0.999999) < threshold) << TestHelper::PrintTo();
}

TEST_F(OcrTestFixture, BadCoordinatesUnitTest)
{
    char **images = new char *[1];
    images[0] = convertStringtoCharPtr("images0");
    char **contentType = new char *[1];
    contentType[0] = convertStringtoCharPtr("image/jpeg");
    char **contentBase64 = new char *[1];
    contentBase64[0] = convertStringtoCharPtr(getBase64(cv::imread("testData/images/AFG_AO_01001_FRONT.JPG")));
    Coordinates *coordinates = new Coordinates[1];
    coordinates[0] = Coordinates{0, 0, 0, 0};
    float *thresholds = new float[1];
    thresholds[0] = 0.3;
    char *algoType = new char;
    algoType = convertStringtoCharPtr("RetinaNet");

    OcrResponse result = process(images, contentType, contentBase64, coordinates, thresholds, 1, algoType);

    ASSERT_TRUE(result.resultDetailsSize == 1) << TestHelper::PrintTo();
    ASSERT_TRUE(result.resultDetails[0].charactersSize == 0) << TestHelper::PrintTo();
    ASSERT_TRUE(result.resultDetails[0].error == 1002) << TestHelper::PrintTo();
    ASSERT_TRUE(strcmp(result.resultDetails[0].errorMessage, "Not able to find compliant image with provided metadata") == 0) << TestHelper::PrintTo();
}

TEST_F(OcrTestFixture, OcrTypeNotFoundUnitTest)
{
    char **images = new char *[1];
    images[0] = convertStringtoCharPtr("images0");
    char **contentType = new char *[1];
    contentType[0] = convertStringtoCharPtr("image/jpeg");
    char **contentBase64 = new char *[1];
    contentBase64[0] = convertStringtoCharPtr(getBase64(cv::imread("testData/images/AFG_AO_01001_FRONT.JPG")));
    Coordinates *coordinates = new Coordinates[1];
    coordinates[0] = Coordinates{0, 0, 0, 0};
    float *thresholds = new float[1];
    thresholds[0] = 0.3;
    char *algoType = new char;
    algoType = convertStringtoCharPtr("NewType");

    OcrResponse result = process(images, contentType, contentBase64, coordinates, thresholds, 1, algoType);

    ASSERT_TRUE(result.resultDetailsSize == 1) << TestHelper::PrintTo();
    ASSERT_TRUE(strcmp(result.resultDetails[0].image, "") == 0) << TestHelper::PrintTo();
    ASSERT_TRUE(result.resultDetails[0].confidenceThreshold == -1) << TestHelper::PrintTo();
    ASSERT_TRUE(result.resultDetails[0].charactersSize == 0) << TestHelper::PrintTo();
    ASSERT_TRUE(result.resultDetails[0].error == 1004) << TestHelper::PrintTo();
    ASSERT_TRUE(strcmp(result.resultDetails[0].errorMessage, "Algorithm Type not handled") == 0) << TestHelper::PrintTo();
}