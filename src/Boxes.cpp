#include "../include/Boxes.h"

Boxes::Boxes(Document doc, cv::Mat boxesPred):
    document(doc), pred(boxesPred)
{
    predMatrix = extractPredCVMat(pred);
}


cv::Mat Boxes::getPred()
{
    return pred;
}

matrix2D Boxes::getPredMatrix()
{
    return predMatrix;
}

matrix2D Boxes::getCorners()
{
    return corners;
}

void Boxes::setpredMatrix(matrix2D predictions)
{
    predMatrix = predictions;
}

void Boxes::computeCorners(matrix2D boxes)
{
    corners = boxes;
    for(size_t i = 0; i < boxes.size(); ++i)
    {
        corners[i][0] = boxes[i][0] - boxes[i][2] / 2;
        corners[i][1] = boxes[i][1] - boxes[i][3] / 2;
        corners[i][2] = boxes[i][0] + boxes[i][2] / 2;
        corners[i][3] = boxes[i][1] + boxes[i][3] / 2;  
    }
}

matrix2D Boxes::extractPredCVMat(cv::Mat boxPrediction)
{
    matrix2D boxPred;
    std::vector<float> box;
    cv::Vec<int,4> idx;
    int j = 0;
    int i = 0;
    int count = 0;

    while(count < boxPrediction.size[1] * boxPrediction.size[3])
    {
        if(j >= boxPrediction.size[3])
        {
            i += 1;
            j = 0;
        }
        idx = {0, i, 0, j};
        box.push_back(boxPrediction.at<float>(idx));
        count += 1;
        j += 1;
        if(count % boxPrediction.size[1] == 0)
        {
            boxPred.push_back(box);
            box.clear();
        }
    }
    return boxPred;
}


void Boxes::printPredCVMat()
{
    for(size_t i = 0; i < predMatrix.size(); ++i)
    {
        for(size_t j = 0; j < predMatrix.begin()->size(); ++j)
            if(i < 20)
                std::cout << predMatrix[i][j] << " ";
        if(i < 20)
            std::cout << " " << std::endl;
    }
}