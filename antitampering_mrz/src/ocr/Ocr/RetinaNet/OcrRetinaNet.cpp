#include "OcrRetinaNet.hpp"
#include <iostream>
#include <vector>

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
        throw OcrException(OcrErrorCode::GENERAL_ERROR, "Error Running Model : " + std::string(e.what()));
    }
}

std::pair<matrix2D, matrix2D> OcrRetinaNet::adjustPredictions(cv::Mat predictions)
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
    boxesPred = multVariance(boxesPred);
    //printPred(boxPred);
    matrix2D classesPred = CVMatToMatrix2D(classPredCVMat);
    classesPred = applySigmoid(classesPred);
    //printPred(classPred);

    return std::make_pair(boxesPred, classesPred);
}

matrix2D OcrRetinaNet::CVMatToMatrix2D(cv::Mat cvMat) //TOO SLOW, TRY TO OPTIMIZE!
{
    matrix2D matrix;
    std::vector<float> pred;
    cv::Vec<int,4> idx;
    int j = 0;
    int i = 0;
    int count = 0;
    while(count < cvMat.size[1] * cvMat.size[3])
    {
        idx = {0, i, 0, j};
        pred.push_back(cvMat.at<float>(idx));
        count += 1;
        if(count % cvMat.size[3] == 0)
        {
            i += 1;
            j = 0;
            matrix.push_back(pred);
            pred.clear();
        }
        else
           j += 1; 
    }
    return matrix;
}

matrix2D OcrRetinaNet::multVariance(matrix2D box)
{
    for(size_t i = 0; i < box.size(); ++i)
    {
        box[i][0] = box[i][0] * 0.1;
        box[i][1] = box[i][1] * 0.1;
        box[i][2] = box[i][2] * 0.2;
        box[i][3] = box[i][3] * 0.2;
    }
    return box;
}

matrix2D OcrRetinaNet::applySigmoid(matrix2D A)
{
    for(size_t i = 0; i < A.size(); ++i)
        for(size_t j = 0; j < A[i].size(); ++j)
            A[i][j] = 1 / (1 + exp(- A[i][j]));
    return A;
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

std::vector<OcrData> OcrRetinaNet::detect(const cv::Mat image, const float confidenceThreshold)
{
    SPDLOG_INFO("Preprocessing input image");
    cv::Mat imagePreprocessed = this -> imagePreprocessing(image);

    SPDLOG_INFO("Extracting predictions");
    cv::Mat predictions = this -> inference(imagePreprocessed);
    matrix2D boxesPred = this -> adjustPredictions(predictions).first;
    matrix2D classesPred = this -> adjustPredictions(predictions).second;

    SPDLOG_INFO("Constructing Anchors");
    Anchors *anchors = new Anchors(modelInputSize.width, modelInputSize.height);
    matrix2D boxesAnchor = anchors->anchorsGenerator();

    SPDLOG_INFO("Adjust boxes");
    matrix2D boxes = computeCenters(boxesPred, boxesAnchor);
    boxes = computeBoxes(boxes);

    SPDLOG_INFO("Apply NMS");
    std::vector<OcrData> characters = nonMaximaSuppression(boxes, classesPred, confidenceThreshold);

    SPDLOG_INFO("Printing result");
    savePredictionImage(image, characters);

    return characters;
}

std::vector<OcrData> OcrRetinaNet::nonMaximaSuppression(matrix2D boxesPreNMS, matrix2D classPred, float confidenceThreshold)
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
    cv::dnn::dnn4_v20220524::NMSBoxes(boxesRect, maxAll, confidenceThreshold, THRESHOLD_NMS, nmsIndices);
    extern std::unordered_map<unsigned, char> dictionary;
    std::vector<OcrData> characters;
    for(unsigned idx : nmsIndices)
    {
        OcrData currCharacter;
        currCharacter.labelIndex = maxIndices[idx];
        currCharacter.position = reshapeBox(boxesNew[idx]);
        currCharacter.confidence = maxAll[idx];
        currCharacter.label = dictionary[int(maxIndices[idx])];
        characters.push_back(currCharacter);
    }
    SPDLOG_INFO("{} Characters Detected post NMS", nmsIndices.size());
    return characters;
}

Coordinates OcrRetinaNet::reshapeBox(std::vector<float> box)
{
    box[0] = box[0] / xAlter;
    box[1] = box[1] / yAlter;
    box[2] = box[2] / xAlter;
    box[3] = box[3] / yAlter;

    Coordinates boxNew;
    boxNew.topLeftX = box[0];
    boxNew.topLeftY = box[1];
    boxNew.bottomRightX = box[2];
    boxNew.bottomRightY = box[3];

    return boxNew;
}

matrix2D OcrRetinaNet::computeCenters(matrix2D boxes, matrix2D anchorBoxes)
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

matrix2D OcrRetinaNet::computeBoxes(matrix2D boxes)
{
    matrix2D newBoxes = boxes;
    for(size_t i = 0; i < boxes.size(); ++i)
    {
        newBoxes[i][0] = boxes[i][0] - boxes[i][2] / 2;
        newBoxes[i][1] = boxes[i][1] - boxes[i][3] / 2;
        newBoxes[i][2] = boxes[i][0] + boxes[i][2] / 2;
        newBoxes[i][3] = boxes[i][1] + boxes[i][3] / 2;
    }
    return newBoxes;
}

void OcrRetinaNet::printPredictions(matrix2D pred)
{
    for(size_t i = 0; i < pred.size(); ++i)
    {
        for(size_t j = 0; j < pred.begin()->size(); ++j)
            if(i < 5)
                std::cout << pred[i][j] << " ";
        if(i < 5)
            std::cout << " " << std::endl;
    }
}

void OcrRetinaNet::savePredictionImage(cv::Mat img, std::vector<OcrData> characters)
{
    cv::Mat new_image = img;
    int lineType = cv::LINE_8;
    int fontFace = cv::FONT_HERSHEY_SIMPLEX;
    float fontScale = 0.3;
    int thickness = 1;
    int baseline=0;
    for(OcrData character: characters)
    {
        cv::rectangle(new_image,
                      cv::Point(character.position.topLeftX, character.position.topLeftY),
                      cv::Point(character.position.bottomRightX, character.position.bottomRightY),
                      cv::Scalar(255, 0, 0),
                      thickness, cv::LINE_8);

        std::string label, conf;
        label.append(1, character.label);
        conf = fmt::format("{:.1f}", character.confidence);

        // Print labels
        cv::Size labelSize = cv::getTextSize(label, fontFace, fontScale, thickness, &baseline);
        cv::Point labelOrg(character.position.topLeftX + labelSize.width/4, character.position.topLeftY - labelSize.height/4);
        cv::putText(new_image, label, labelOrg, fontFace, fontScale, cv::Scalar(0,0,0), thickness, cv::LINE_AA);

        // Print confidences
        cv::Size confSize = cv::getTextSize(conf, fontFace, fontScale*0.6, thickness, &baseline);
        cv::Point confOrg(character.position.topLeftX, character.position.bottomRightY + confSize.height);
        cv::putText(new_image, conf, confOrg, fontFace, fontScale*0.6, cv::Scalar(0,0,0), thickness, cv::LINE_AA);
    }
    cv::imwrite("../OCR.jpg", new_image);
}