#include "OcrRetinaNet.hpp"
#include <iostream>
#include <vector>

static std::unordered_map<unsigned, char> dictionary = {
    {0, '0',}, {1, '1',}, {2, '2',}, {3, '3',}, {4, '4',}, {5, '5',}, {6, '6',}, {7, '7',}, {8, '8',}, {9, '9',},
    {10, 'A',}, {11, 'a',}, {12, 'B',}, {13, 'b',}, {14, 'C',}, {15, 'c',}, {16, 'D',}, {17, 'd',}, {18, 'E',}, {19, 'e',},
    {20, 'F',}, {21, 'f',}, {22, 'G',}, {23, 'g',}, {24, 'H',}, {25, 'h',}, {26, 'I',}, {27, 'i',}, {28, 'J',}, {29, 'j',},
    {30, 'K',}, {31, 'k',}, {32, 'L',}, {33, 'l',}, {34, 'M',}, {35, 'm',}, {36, 'N',}, {37, 'n',}, {38, 'O',}, {39, 'o',},
    {40, 'P',}, {41, 'p',}, {42, 'Q',}, {43, 'q',}, {44, 'R',}, {45, 'r',}, {46, 'S',}, {47, 's',}, {48, 'T',}, {49, 't',},
    {50, 'U',}, {51, 'u',}, {52, 'V',}, {53, 'v',}, {54, 'W',}, {55, 'w',}, {56, 'X',}, {57, 'x',}, {58, 'Y',}, {59, 'y',},
    {60, 'Z',}, {61, 'z',}, {62, '<',}
};

OcrRetinaNet::OcrRetinaNet(const cv::dnn::Net model, cv::Size modelInputSize): Ocr()
{
    SPDLOG_INFO("Creating RetinaNet Model");
    this -> model = model;
    this -> modelInputSize = modelInputSize;
    SPDLOG_INFO("RetinaNet Model Created");
}

cv::Mat OcrRetinaNet::inference(const cv::Mat& image)
{
    SPDLOG_INFO("Running Model");
    try 
    {
        cv::Mat blob = cv::dnn::blobFromImage(image, 1.0, modelInputSize, cv::Scalar(103.939, 116.779, 123.68), true, false);
        model.setInput(blob);
        cv::Mat prediction = model.forward();
        return prediction;
    }
    catch(cv::Exception& e)
    {
        SPDLOG_ERROR("Error Running Model : {}", e.what());
        throw Exception(ErrorCode::GENERAL_ERROR, "Error Running Model : " + std::string(e.what()));
    }
}

std::pair<matrix2D, matrix2D> OcrRetinaNet::adjustModelPredictions(cv::Mat predictions)
{
    // Split box and class predictions
    cv::Range boxRange(0, 4);
    cv::Range classRange(4, NUM_CLASSES + 4);
    cv::Range all(cv::Range::all());
    std::vector<cv::Range> boxRanges = {all, all, all, boxRange};
    std::vector<cv::Range> classRanges = {all, all, all, classRange};

    // Extract box and class predictions
    cv::Mat boxPredCVMat = predictions(boxRanges);
    cv::Mat classPredCVMat = predictions(classRanges);

    // Convert predictions from cv::Mat to matrix2D
    matrix2D boxesPred = CVMatToMatrix2D(boxPredCVMat);
    matrix2D classesPred = CVMatToMatrix2D(classPredCVMat);

    std::vector<float> variance = {VARIANCE_X, VARIANCE_X, VARIANCE_Y, VARIANCE_Y};
    boxesPred =  multVariance(boxesPred, variance);
    //printPred(boxPred);
    classesPred = applySigmoid(classesPred);
    //printPred(classPred);

    return std::make_pair(boxesPred, classesPred);
}

cv::Mat OcrRetinaNet::imagePreprocessing(const cv::Mat& inputImage)
{
    float imgWidth = inputImage.size[1];
    float imgHeight = inputImage.size[0];
    xAlter = modelInputSize.width / imgWidth;
    yAlter = modelInputSize.height / imgHeight;

    cv::Mat imagePreprocessed;
    cv::fastNlMeansDenoising(inputImage, imagePreprocessed, DENOISE_PARAM);
    cv::resize(imagePreprocessed, imagePreprocessed, modelInputSize, 0, 0, cv::INTER_CUBIC);

    return imagePreprocessed;
}

std::vector<Characters> OcrRetinaNet::detect(const cv::Mat image, const float confidenceThreshold)
{
    if(image.empty())
        throw Exception(ErrorCode::BAD_COORDINATES, "Not able to find compliant image with provided metadata");
    cv::imwrite("../../printResults/post_cut.jpg", image);

    SPDLOG_INFO("Preprocessing input image");
    cv::Mat imagePreprocessed = this -> imagePreprocessing(image);

    SPDLOG_INFO("Extracting predictions");
    cv::Mat predictions = this -> inference(imagePreprocessed);
    matrix2D modelBoxes = this -> adjustModelPredictions(predictions).first;
    matrix2D modelClasses = this -> adjustModelPredictions(predictions).second;

    SPDLOG_INFO("Constructing Anchors");
    Anchors *anchors = new Anchors(modelInputSize.width, modelInputSize.height);
    matrix2D anchorBoxes = anchors->anchorsGenerator();

    SPDLOG_INFO("Adjust boxes");
    matrix2D boxes = computeBoxes(modelBoxes, anchorBoxes);

    SPDLOG_INFO("Apply NMS");
    std::vector<Characters> characters = nonMaximaSuppression(boxes, modelClasses, confidenceThreshold);

    return characters;
}

std::vector<Characters> OcrRetinaNet::nonMaximaSuppression(matrix2D boxesPreNMS, matrix2D classPred, float confidenceThreshold)
{
    std::vector<float> maxIndices, maxAll;
    std::vector<float>::iterator maxIt;
    float maxIndex;
    matrix2D boxesNew;
    std::vector<cv::Rect2d> boxesRect;
    for(size_t i = 0; i < classPred.size(); ++i)
    {
        maxIt = std::max_element(classPred[i].begin(), classPred[i].end());
        maxIndex = std::distance(classPred[i].begin(), maxIt);
        if(*maxIt >= confidenceThreshold)
        {
            maxAll.push_back(*maxIt);
            maxIndices.push_back(maxIndex);
            boxesNew.push_back(boxesPreNMS[i]);
            cv::Rect2d rect(boxesPreNMS[i][0], boxesPreNMS[i][1], boxesPreNMS[i][2] - boxesPreNMS[i][0], boxesPreNMS[i][3] - boxesPreNMS[i][1]);
            boxesRect.push_back(rect);
        }
    }
    SPDLOG_INFO("{} Characters Detected pre NMS", boxesRect.size());
    std::vector<int> nmsIndices;
    cv::dnn::NMSBoxes(boxesRect, maxAll, confidenceThreshold, THRESHOLD_NMS, nmsIndices);
    std::vector<Characters> characters;
    for(unsigned idx : nmsIndices)
    {
        Characters currCharacter;
        currCharacter.labelIndex = maxIndices[idx];
        currCharacter.position = reshapeBox(boxesNew[idx], xAlter, yAlter);
        currCharacter.confidence = maxAll[idx];
        currCharacter.label = dictionary[int(maxIndices[idx])];
        characters.push_back(currCharacter);
    }
    SPDLOG_INFO("{} Characters Detected post NMS", nmsIndices.size());
    return characters;
}

matrix2D OcrRetinaNet::computeBoxes(matrix2D modelBoxes, matrix2D anchorBoxes)
{
    // Extract boxes in format (x, y, w, h)
    matrix2D boxes = modelBoxes;
    for(size_t i = 0; i < boxes.size(); ++i)
    {
        boxes[i][0] = modelBoxes[i][0] * anchorBoxes[i][2] + anchorBoxes[i][0];
        boxes[i][1] = modelBoxes[i][1] * anchorBoxes[i][3] + anchorBoxes[i][1];
        boxes[i][2] = std::exp(modelBoxes[i][2]) *  anchorBoxes[i][2];
        boxes[i][3] = std::exp(modelBoxes[i][3]) *  anchorBoxes[i][3]; 
    }

    // Convert boxes in (x1, y1, x2, y2)
    boxes = convertToConers(boxes);
    return boxes;
}