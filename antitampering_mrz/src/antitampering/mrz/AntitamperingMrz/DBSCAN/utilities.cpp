#include "utilities.h"


bool isNumber(const std::string& str)
{
    return str.find_first_not_of("0123456789") == std::string::npos;
}


void savePredictionImage(cv::Mat img, matrix2D boxes, std::vector<float> classes, std::string img_name)
{
    cv::Mat new_image = img;
    int fontFace = cv::FONT_HERSHEY_SIMPLEX;
    float fontScale = 0.4;
    int thickness_pred = 1;
    int thickness_rect = 1;
    int lineType = cv::LINE_8;

    extern std::unordered_map<unsigned, std::string> dictionary;

    for(size_t i = 0; i < classes.size(); ++i)
    {
        cv::rectangle(new_image, cv::Point(boxes[i][0], boxes[i][1]), cv::Point(boxes[i][2], boxes[i][3]), cv::Scalar(255,0,0),thickness_rect, lineType);
        cv::putText(new_image, dictionary[classes[i]],  cv::Point((boxes[i][2] + boxes[i][0])/2 - 4 , boxes[i][1] - 4), fontFace, fontScale, cv::Scalar(0, 0, 0), thickness_pred);
    }
    cv::imwrite(img_name, new_image);
}


void saveCentersPredictionImage(cv::Mat img, std::vector<Character> centers, std::string img_name)
{
    cv::Mat new_image = img;
    int thickness = 5;
    int lineType = cv::LINE_8;
    std::vector<int> color;
    color.resize(centers.size());

    extern std::unordered_map<unsigned, std::string> dictionary;

    for(size_t i = 0; i < centers.size(); ++i)
    {
        color[i] = (centers[i].getCluster() + 2);
        cv::circle(new_image, cv::Point(centers[i].getX(), centers[i].getY()), 0, cv::Scalar(0, 0, color[i]*20), thickness, lineType);
    }
    cv::imwrite(img_name, new_image);
}