#include "../include/utilities.h"

void savePredictionImage(cv::Mat img, matrix2D boxes, std::vector<float> classes, std::string img_name)
{
    cv::Mat new_image = img;
    int fontFace = cv::FONT_HERSHEY_SIMPLEX;
    double fontScale = 0.4;
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


matrix2D computeCenters(matrix2D boxes, matrix2D anchorBoxes)
{
    matrix2D center = boxes;
    for(size_t i = 0; i < boxes.size(); ++i)
    {
        center[i][0] = boxes[i][0] * anchorBoxes[i][2] + anchorBoxes[i][0];
        center[i][1] = boxes[i][1] * anchorBoxes[i][3] + anchorBoxes[i][1];
        center[i][2] = std::exp(boxes[i][2]) *  anchorBoxes[i][2];
        center[i][3] = std::exp(boxes[i][3]) *  anchorBoxes[i][3]; 
    }
    return center;
}