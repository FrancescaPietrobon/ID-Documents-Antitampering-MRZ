#include "PrintResult.hpp"

void printOCRResult(cv::Mat img, Characters *characters, size_t charactersSize)
{
    cv::Mat new_image = img;
    int lineType = cv::LINE_AA;
    int fontFace = cv::FONT_HERSHEY_SIMPLEX;
    float fontScale = 0.3;
    int thickness = 1;
    int baseline=0;
    for(size_t i = 0; i < charactersSize; ++i)
    {
        cv::rectangle(new_image,
                      cv::Point(characters[i].position.topLeftX, characters[i].position.topLeftY),
                      cv::Point(characters[i].position.bottomRightX, characters[i].position.bottomRightY),
                      cv::Scalar(255, 0, 0),
                      thickness, cv::LINE_8);

        std::string label, conf;
        label.append(1, characters[i].label);
        conf = fmt::format("{:.1f}", characters[i].confidence);

        // Print labels
        cv::Size labelSize = cv::getTextSize(label, fontFace, fontScale, thickness, &baseline);
        cv::Point labelOrg(characters[i].position.topLeftX + labelSize.width/4, characters[i].position.topLeftY - labelSize.height/4);
        cv::putText(new_image, label, labelOrg, fontFace, fontScale, cv::Scalar(0,0,0), thickness, lineType);

        // Print confidences
        cv::Size confSize = cv::getTextSize(conf, fontFace, fontScale*0.6, thickness, &baseline);
        cv::Point confOrg(characters[i].position.topLeftX, characters[i].position.bottomRightY + confSize.height);
        cv::putText(new_image, conf, confOrg, fontFace, fontScale*0.6, cv::Scalar(0,0,0), thickness, lineType);
    }
    cv::imwrite("../../printResults/OCR.jpg", new_image);
}

void printDbscanResult(cv::Mat img, Fields* fields, size_t fieldsSize)
{
    cv::Mat new_image = img;
    int lineType = cv::LINE_AA;
    int fontFace = cv::FONT_HERSHEY_SIMPLEX;
    float fontScale = 0.3;
    int thickness = 1;
    int baseline=0;
    for(size_t i = 0; i < fieldsSize; ++i)
    {
        cv::rectangle(new_image,
                      cv::Point(fields[i].position.topLeftX, fields[i].position.topLeftY),
                      cv::Point(fields[i].position.bottomRightX, fields[i].position.bottomRightY),
                      cv::Scalar(255, 0, 0),
                      thickness, cv::LINE_8);

        std::string label, conf;
        for(size_t j = 0; j < fields[i].labelSize; ++j)
            label.push_back(fields[i].label[j]);

        conf = fmt::format("{:.1f}", fields[i].confidence);

        // Print labels
        cv::Size labelSize = cv::getTextSize(label, fontFace, fontScale, thickness, &baseline);
        cv::Point labelOrg(fields[i].position.topLeftX + labelSize.width/4, fields[i].position.topLeftY - labelSize.height/4);
        cv::putText(new_image, label, labelOrg, fontFace, fontScale, cv::Scalar(0,0,0), thickness, lineType);

        // Print confidences
        cv::Size confSize = cv::getTextSize(conf, fontFace, fontScale*0.6, thickness, &baseline);
        cv::Point confOrg(fields[i].position.topLeftX, fields[i].position.bottomRightY + confSize.height);
        cv::putText(new_image, conf, confOrg, fontFace, fontScale*0.6, cv::Scalar(0,0,0), thickness, lineType);
    }
    cv::imwrite("../../printResults/DBSCAN.jpg", new_image);
}