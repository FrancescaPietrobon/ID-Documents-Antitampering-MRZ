#include "ModelBoxes.h"

ModelBoxes::ModelBoxes(Anchors *anch, Document doc, cv::Mat pred, int n, float tIOU, float tNMS):
    anchors(anch), Boxes(doc), predictions(pred), numClasses(n), thresholdIOU(tIOU), thresholdNMS(tNMS)
    {
        if(anchors == nullptr){
            throw std::invalid_argument("Anchors must not be null");
        }
    }


std::pair<matrix2D, std::vector<float>> ModelBoxes::predict()
{
    // Split box and class predictions
    cv::Range boxRange(0, 4);
    cv::Range classRange(4, numClasses + 4);
    cv::Range all(cv::Range::all());
    std::vector<cv::Range> boxRanges = {all, all, all, boxRange};
    std::vector<cv::Range> classRanges = {all, all, all, classRange};

    // Extract box and class predictions
    cv::Mat boxPredCVMat = predictions(boxRanges);
    cv::Mat classPredCVMat = predictions(classRanges);

    // Convert predictions from cv::Mat to matrix2D
    matrix2D boxPred = CVMatToMatrix2D(boxPredCVMat);
    boxPred = multVariance(boxPred);
    matrix2D classPred = CVMatToMatrix2D(classPredCVMat);
    classPred = applySigmoid(classPred);

    //printBoxPred();
    //printClassPred();

    SPDLOG_INFO("Generate anchors");
    matrix2D anchorBoxes = anchors->anchorsGenerator();

    SPDLOG_INFO("Adjust boxes");
    matrix2D boxesAll = computeCenters(boxPred, anchorBoxes);
    boxesAll = computeBoxes(boxesAll);

    SPDLOG_INFO("Compute NMS");
    std::pair<matrix2D, std::vector<float>> result = computeNMS(boxesAll, classPred, thresholdIOU, thresholdNMS);
    
    return result;
}


matrix2D ModelBoxes::CVMatToMatrix2D(cv::Mat cvMat) //TOO SLOW, TRY TO OPTIMIZE!
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


matrix2D ModelBoxes::multVariance(matrix2D box)
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


matrix2D ModelBoxes::applySigmoid(matrix2D A)
{
    for(size_t i = 0; i < A.size(); ++i)
        for(size_t j = 0; j < A[i].size(); ++j)
            A[i][j] = 1 / (1 + exp(- A[i][j]));
    return A;
}


matrix2D ModelBoxes::computeCenters(matrix2D boxes, matrix2D anchorBoxes)
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


matrix2D ModelBoxes::computeBoxes(matrix2D boxes)
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


std::pair<matrix2D, std::vector<float>> ModelBoxes::computeNMS(matrix2D boxesPreNMS, matrix2D classPred, float threshold, float threshold_nms)
{
    std::vector<float> maxIndices;
    std::vector<float> maxAll;
    std::vector<float>::iterator maxIt;
    float maxIndex;
    matrix2D boxesNew;
    std::vector<cv::Rect2d> boxesRect;

    for(size_t i = 0; i < classPred.size(); ++i)
    {
        maxIt = std::max_element(classPred[i].begin(), classPred[i].end());
        maxIndex = std::distance(classPred[i].begin(), maxIt);
        
        if(*maxIt >= threshold)
        {
            maxAll.push_back(*maxIt);
            maxIndices.push_back(maxIndex);
            boxesNew.push_back(boxesPreNMS[i]);
            cv::Rect2d rect(boxesPreNMS[i][0], boxesPreNMS[i][1], boxesPreNMS[i][2] - boxesPreNMS[i][0], boxesPreNMS[i][3] - boxesPreNMS[i][1]);
            boxesRect.push_back(rect);
        }
    }

    std::cout << "Number of chars detected pre NMS: " << boxesRect.size() << std::endl;
    std::vector<int> nmsIndices;
    cv::dnn::dnn4_v20220524::NMSBoxes(boxesRect, maxAll, threshold, threshold_nms, nmsIndices);
    
    std::cout << "Number of chars detected after NMS: " << nmsIndices.size() << std::endl;

    for(size_t i = 0; i < nmsIndices.size(); ++i)
    {
        boxes.push_back(boxesNew[nmsIndices[i]]);
        classes.push_back(maxIndices[nmsIndices[i]]);
    }
    boxes = reshapeBoxes(boxes); 
    
    return std::make_pair(boxes, classes);
}


matrix2D ModelBoxes::reshapeBoxes(matrix2D boxes)
{
    for(size_t i = 0; i < boxes.size(); ++i)
    {
        boxes[i][0] = boxes[i][0] / document.getXAlter();
        boxes[i][1] = boxes[i][1] / document.getYAlter();
        boxes[i][2] = boxes[i][2] / document.getXAlter();
        boxes[i][3] = boxes[i][3] / document.getYAlter();
    }
    return boxes;
}


void ModelBoxes::printBoxPred(matrix2D boxPred)
{
    for(size_t i = 0; i < boxPred.size(); ++i)
    {
        for(size_t j = 0; j < boxPred.begin()->size(); ++j)
            if(i < 5)
                std::cout << boxPred[i][j] << " ";
        if(i < 5)
            std::cout << " " << std::endl;
    }
}


void ModelBoxes::printClassPred(matrix2D classPred)
{    
    for(size_t i = 0; i < classPred.size(); ++i)
    {
        for(size_t j = 0; j < classPred.begin()->size(); ++j)
            if(i < 5)
                std::cout << classPred[i][j] << " ";
        if(i < 5)
            std::cout << " " << std::endl;
    }
}