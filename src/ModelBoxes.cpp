#include "../include/ModelBoxes.h"

ModelBoxes::ModelBoxes(Document doc, cv::Mat pred, int n):
    Boxes(doc), predictions(pred), numClasses(n)
    {
        cv::Range boxRange(0, 4);
        cv::Range classRange(4, numClasses + 4);
        cv::Range all(cv::Range::all());
        std::vector<cv::Range> boxRanges = {all, all, all, boxRange};
        std::vector<cv::Range> classRanges = {all, all, all, classRange};

        boxPredCVMat = predictions(boxRanges);
        classPredCVMat = predictions(classRanges);

        boxPred = extractPredCVMat(boxPredCVMat);
        multBias(boxPred);
        classPred = extractPredCVMat(classPredCVMat);
        applySigmoid(classPred);

        //printBoxPred();
        //printClassPred();
    }

void ModelBoxes::multBias(matrix2D & box)
{
    for(size_t i = 0; i < box.size(); ++i)
    {
        box[i][0] = box[i][0] * 0.1;
        box[i][1] = box[i][1] * 0.1;
        box[i][2] = box[i][2] * 0.2;
        box[i][3] = box[i][3] * 0.2;
    }
}

matrix2D ModelBoxes::getBoxPred()
{
    return boxPred;
}

matrix2D ModelBoxes::getClassPred()
{
    return classPred;
}

cv::Mat ModelBoxes::getBoxPredCVMat()
{
    return boxPredCVMat;
}

cv::Mat ModelBoxes::getClassPredCVMat()
{
    return boxPredCVMat;
}

matrix2D ModelBoxes::getBoxesPreNMS()
{
    return boxesPreNMS;
}

matrix2D ModelBoxes::getBoxesReshaped()
{
    return boxesReshaped;
}


void ModelBoxes::computeBoxes(matrix2D centers)
{
    boxesPreNMS = centers;
    for(size_t i = 0; i < centers.size(); ++i)
    {
        boxesPreNMS[i][0] = centers[i][0] - centers[i][2] / 2;
        boxesPreNMS[i][1] = centers[i][1] - centers[i][3] / 2;
        boxesPreNMS[i][2] = centers[i][0] + centers[i][2] / 2;
        boxesPreNMS[i][3] = centers[i][1] + centers[i][3] / 2;
    }
}


matrix2D ModelBoxes::extractPredCVMat(cv::Mat cvMat)
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


void ModelBoxes::computeNMS(float threshold, float threshold_nms)
{
    std::vector<float> indices_max;
    std::vector<float> all_max;
    std::vector<float>::iterator max_it;
    float index_max;
    matrix2D boxes_new;
    std::vector<cv::Rect2d> boxes_rect;

    for(size_t i = 0; i < classPred.size(); ++i)
    {
        max_it = std::max_element(classPred[i].begin(), classPred[i].end());
        index_max = std::distance(classPred[i].begin(), max_it);
        
        if(*max_it >= threshold)
        {
            all_max.push_back(*max_it);
            indices_max.push_back(index_max);
            boxes_new.push_back(boxesPreNMS[i]);
            cv::Rect2d rect(boxesPreNMS[i][0], boxesPreNMS[i][1], boxesPreNMS[i][2] - boxesPreNMS[i][0], boxesPreNMS[i][3] - boxesPreNMS[i][1]);
            boxes_rect.push_back(rect);
        }
        max_it = classPred[i].end();
        index_max = 0;
    }

    std::cout << "Number of chars pre NMS: " << boxes_rect.size() << std::endl;
    std::vector<int> nmsIndices;
    cv::dnn::dnn4_v20220524::NMSBoxes(boxes_rect, all_max, threshold, threshold_nms, nmsIndices);
    
    std::vector<float> all_max_nms;
    std::cout << "Number of chars after NMS: " << nmsIndices.size() << std::endl;

    for(size_t i = 0; i < nmsIndices.size(); ++i)
    {
        boxesReshaped.push_back(boxes_new[nmsIndices[i]]);
        classes.push_back(indices_max[nmsIndices[i]]);
        all_max_nms.push_back(all_max[nmsIndices[i]]);
    }    
}


void ModelBoxes::reshapeBoxes()
{
    boxes = boxesReshaped;
    for(size_t i = 0; i < boxes.size(); ++i)
    {
        boxes[i][0] = boxesReshaped[i][0] / document.getXAlter();
        boxes[i][1] = boxesReshaped[i][1] / document.getYAlter();
        boxes[i][2] = boxesReshaped[i][2] / document.getXAlter();
        boxes[i][3] = boxesReshaped[i][3] / document.getYAlter();
    }
}

void ModelBoxes::printBoxPred()
{
    
    for(size_t i = 0; i < boxPred.size(); ++i)
    {
        for(size_t j = 0; j < boxPred.begin()->size(); ++j)
            if(i < 10)
                std::cout << boxPred[i][j] << " ";
        if(i < 10)
            std::cout << " " << std::endl;
    }
}


void ModelBoxes::printClassPred()
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


void ModelBoxes::applySigmoid(matrix2D &A)
{
    for(size_t i = 0; i < A.size(); ++i)
        for(size_t j = 0; j < A[i].size(); ++j)
            A[i][j] = 1 / (1 + exp(- A[i][j]));
}