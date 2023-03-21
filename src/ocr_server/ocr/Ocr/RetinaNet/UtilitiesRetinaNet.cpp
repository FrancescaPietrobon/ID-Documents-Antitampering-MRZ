#include "UtilitiesRetinaNet.hpp"

matrix2D utilsRetinaNet::CVMatToMatrix2D(cv::Mat cvMat)
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

matrix2D utilsRetinaNet::multVariance(matrix2D box, std::vector<float> variance)
{
    for(size_t i = 0; i < box.size(); ++i)
        for(size_t j = 0; j < box[i].size(); ++j)
            box[i][j] = box[i][j] * variance[j];
    return box;
}

matrix2D utilsRetinaNet::applySigmoid(matrix2D A)
{
    for(size_t i = 0; i < A.size(); ++i)
        for(size_t j = 0; j < A[i].size(); ++j)
            A[i][j] = 1 / (1 + exp(- A[i][j]));
    return A;
}

Coordinates utilsRetinaNet::reshapeBox(std::vector<float> box, float xAlter, float yAlter)
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

matrix2D utilsRetinaNet::convertToConers(matrix2D boxes)
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

void utilsRetinaNet::printPredictions(matrix2D pred)
{
    for(size_t i = 0; i < pred.size(); ++i)
    {
        for(size_t j = 0; j < pred.begin()->size(); ++j)
            if(i < 5)
                SPDLOG_DEBUG("{}", pred[i][j]);
        if(i < 5)
            SPDLOG_DEBUG(" ");
    }
}
