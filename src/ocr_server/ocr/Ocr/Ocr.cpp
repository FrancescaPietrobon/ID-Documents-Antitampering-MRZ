#include "Ocr.hpp"

cv::Mat Ocr::binarization(cv::Mat img)
{
    cv::Mat gray_sharpened_image;
    cv::cvtColor(img, gray_sharpened_image, cv::COLOR_BGR2GRAY);

    cv::imwrite("../../printResults/gray.JPG", gray_sharpened_image);

    // Apply a Gaussian blur filter
    cv::Mat image_blurred;
    cv::GaussianBlur(gray_sharpened_image, image_blurred, cv::Size(15, 15), 1, 1);

    // Apply the sharpening filter
    cv::Mat kernel = (cv::Mat_<float>(3, 3) << -1, -1, -1, -1, 9, -1, -1, -1, -1);
    cv::Mat sharpened_image;
    cv::filter2D(image_blurred, sharpened_image, -1, kernel);

    // Apply local binarization
    int block_size = 71;
    double C = 2;
    cv::Mat binary_image_1;
    cv::adaptiveThreshold(sharpened_image, binary_image_1, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, block_size, C);

    // Apply global binarization
    double threshold = cv::mean(sharpened_image)[0] * 0.9;
    cv::Mat binary_image_2;
    cv::threshold(sharpened_image, binary_image_2, threshold, 255, cv::THRESH_BINARY);

    // Combine binarizations
    cv::Mat result = binary_image_1 | binary_image_2;

    cv::Mat binaryImg(img.rows, img.cols, CV_8UC3, cv::Scalar(0, 0, 0));
    std::vector<cv::Mat> channels(3, result);
    cv::merge(channels, binaryImg);

    return binaryImg;
}