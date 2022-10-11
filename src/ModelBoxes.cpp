#include "../include/ModelBoxes.h"
#include<algorithm>

ModelBoxes::ModelBoxes(Document doc, cv::Mat pred, int n):
    Boxes(doc), predictions(pred), numClasses(n)
    {
        cv::Range boxRange(0, 4);
        cv::Range classRange(4, numClasses + 4);
        cv::Range all(cv::Range::all());
        std::vector<cv::Range> boxRanges = {all, all, all, boxRange};
        std::vector<cv::Range> classRanges = {all, all, all, classRange};

        std::cout << predictions.size[0] << " x " << predictions.size[1] << " x " << predictions.size[2] << " x " << predictions.size[3] << std::endl;
        boxPredCVMat = predictions(boxRanges);
        std::cout << boxPredCVMat.size[0] << " x " << boxPredCVMat.size[1] << " x " << boxPredCVMat.size[2] << " x " << boxPredCVMat.size[3] << std::endl;
        classPredCVMat = predictions(classRanges);
        std::cout << classPredCVMat.size[0] << " x " << classPredCVMat.size[1] << " x " << classPredCVMat.size[2] << " x " << classPredCVMat.size[3] << std::endl;

        boxPred = extractPredCVMat(boxPredCVMat);
        multBias(boxPred);
        classPred = extractPredCVMat(classPredCVMat);
        //std::cout << "Pre sigmoid" << std::endl;
        //printClassPred();
        //printBoxPred();
        applySigmoid(classPred);
        //std::cout << "Post sigmoid" << std::endl;
        printClassPred();
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
        //if(i < 5)
        //    std::cout << boxesPreNMS[i][0] << " " << boxesPreNMS[i][1] << " " << boxesPreNMS[i][2] << " " << boxesPreNMS[i][3] << std::endl;
    }
}
/*
matrix2D ModelBoxes::extractPredCVMat(cv::Mat cvMat)
{
    matrix2D matrix;
    std::vector<float> pred;
    cv::Vec<int,4> idx;
    int j = 0;
    int i = 0;
    int acc = 0;
    int count = 0;
    int countStep = 0;

    //std::cout << "size: " << cvMat.size[1] * cvMat.size[3] << std::endl;
    std::cout << "size 3: " << cvMat.size[3] << std::endl; //4
    std::cout << "size 1: " << cvMat.size[1] << std::endl; //200145

    std::vector<float> dims = {150000., 37500., 9375., 2535., 735.,};
    while(count <= cvMat.size[3])
    {
        for(size_t depth = 0; depth < dims.size(); ++depth)
        {
            for(size_t step = 0; step < cvMat.size[1]; ++step)
            {
                std::cout << "Step: " << step << "   depth: " << depth << "   j: " << j << std::endl;
                countStep = 0;
                
                while (countStep < dims[depth])
                {
                    if(i > cvMat.size[3])
                    {
                        j += 1;
                        i = 0;
                        std::cout << "i: " << i << " j: " << j << "   ";
                    }
                    idx = {0, i, 0, j};
                    pred.push_back(cvMat.at<float>(idx));
                    count += 1;
                    countStep += 1;
                    i += 1;
                    if(count % cvMat.size[1] == 0)
                    {
                        matrix.push_back(pred);
                        //if(count < 50)
                        //    std::cout << pred[0] << " " << pred[1] << " " << pred[2] << " " << pred[3] << std::endl;
                        pred.clear();
                    }  
                }
                std::cout << "j pre: " << j;
                j = acc + (step+1) * std::accumulate(dims.begin(), dims.end(), 0) / cvMat.size[1];
                std::cout << "   j post: " << j << std::endl;
            }
            acc = std::accumulate(dims.begin(), dims.end()-(dims.size()-depth-1), 0) / cvMat.size[1];
            std::cout << "acc: " << acc << std::endl;
            j = acc;
        }
    }

    std::cout << "i: " << i << " j: " << j << "   " << std::endl;
    std::cout << matrix[0].size() << " " << matrix.size() << std::endl;
    return matrix;
}
*/

/*
matrix2D ModelBoxes::extractPredCVMat(cv::Mat cvMat)
{
    matrix2D matrix;
    std::vector<float> pred;
    cv::Vec<int,4> idx;
    int j = 0;
    int i = 0;
    int iPrec = 0;
    int count = 0;
    int countStep = 0;

    //std::cout << "size: " << cvMat.size[1] * cvMat.size[3] << std::endl;
    std::cout << "size 3: " << cvMat.size[3] << std::endl; //4
    std::cout << "size 1: " << cvMat.size[1] << std::endl; //200145

    std::vector<float> dims = {150000., 37500., 9375., 2535., 735.,};
    while(count <= cvMat.size[3])
    {
        j = 0;
        for(size_t step = 0; step < cvMat.size[1]; ++step)
        {
            std::cout << "Step: " << step << "  dim: " << dims[step] << std::endl;
            j = std::max(0, static_cast<int>(((step+1) * std::accumulate(dims.begin(), dims.end(), 0) - dims[step]) / cvMat.size[1]));
            std::cout << "acc: " << std::accumulate(dims.begin(), dims.end(), 0) - dims[step] << "   div: " << (((step+1) * std::accumulate(dims.begin(), dims.end(), 0) - dims[step]) / cvMat.size[1]) << "   j: " << j << std::endl;

            //std::cout << "acc: " << std::accumulate(dims.begin(), dims.end(), 0) - dim << std::endl;
            
            countStep = 0;
            
            while (countStep < dims[step])
            {
                if(j >= cvMat.size[3])
                {
                    i += 1;
                    j = 0;
                    //std::cout << "i: " << i << " j: " << j << "   ";
                }
                idx = {0, i, 0, j};
                pred.push_back(cvMat.at<float>(idx));
                count += 1;
                countStep += 1;
                j += 1;
                if(count % cvMat.size[1] == 0)
                {
                    matrix.push_back(pred);
                    //if(count < 50)
                    //    std::cout << pred[0] << " " << pred[1] << " " << pred[2] << " " << pred[3] << std::endl;
                    pred.clear();
                }  
            }
            
        }
    }
    

    std::cout << "i: " << i << " j: " << j << "   " << std::endl;
    std::cout << matrix[0].size() << " " << matrix.size() << std::endl;
    return matrix;
}
*/


matrix2D ModelBoxes::extractPredCVMat(cv::Mat cvMat)
{
    matrix2D matrix;
    std::vector<float> pred;
    cv::Vec<int,4> idx;
    int j = 0;
    int i = 0;
    int count = 0;

    //std::cout << "size: " << cvMat.size[1] * cvMat.size[3] << std::endl;
    std::cout << "size 3: " << cvMat.size[3] << std::endl;
    std::cout << "size 1: " << cvMat.size[1] << std::endl;
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
            //if(count < 20)
            //    std::cout << pred[0] << " " << pred[1] << " " << pred[2] << " " << pred[3] << std::endl;
            pred.clear();
        }
        else
           j += 1; 
    }
    std::cout << "i: " << i << " j: " << j << "   " << std::endl;
    std::cout << matrix[0].size() << " " << matrix.size() << std::endl;
    return matrix;
}


static bool abs_compare(float a, float b)
{
    return (a < b);
}

void ModelBoxes::computeNMS(float threshold, float threshold_nms)
{
    std::vector<float> indices_max;
    std::vector<float> all_max;
    std::vector<float>::iterator max_it;
    float index_max;
    float max;
    matrix2D boxes_new;
    std::vector<cv::Rect2d> boxes_rect;
    //std::cout << "size: " << classPred.size() << " x " << classPred[0].size() << std::endl;

    //printClassPred();
    //std::cout << "begin: " << *classPred[0].begin() << " end: " << *(classPred[0].end()-1) << std::endl;
    std::cout << "Pred 37400: " << std::endl;
    printBoxPred();
    //printClassPred();
    //std::cout << "classPred.size(): " << classPred.size() << std::endl;
    for(size_t i = 0; i < classPred.size(); ++i)
    {
        //max = *std::max_element(classPred[i].begin(), std::prev(classPred[i].end()));
        max_it = std::max_element(classPred[i].begin(), classPred[i].end(), abs_compare);
        index_max = std::distance(classPred[i].begin(), max_it);

        //if(i == 2380)
        if((i >= 38000) and (i < 38100))
            std::cout << "For i: " << i << "  max: " << *max_it << "  index_max: " << index_max << std::endl;
        if(*max_it >= threshold)
        {
            //std::cout << "i: " << i << "  index_max: " << index_max << "  max: " << *max_it << std::endl;
            //all_max.push_back(max);
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
    //printClassPred();
    for(size_t i = 0; i < nmsIndices.size(); ++i)
    {
        boxesReshaped.push_back(boxes_new[nmsIndices[i]]);
        //if(i < 30)
        //std::cout << "  class: " << indices_max[nmsIndices[i]];
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
    /*
    for(size_t i = 0; i < boxPred.size(); ++i)
    {
        for(size_t j = 0; j < boxPred.begin()->size(); ++j)
            //if(i < 10)
            if(i > classPred.size() - 5)
                std::cout << boxPred[i][j] << " ";
        //if(i < 10)
        if(i > classPred.size() - 5)
            std::cout << " " << std::endl;
    }
    */

   for(size_t i = 0; i < boxPred.size(); ++i)
    {
        for(size_t j = 0; j < boxPred.begin()->size(); ++j)
            //if(i < 5)
            //if(i > boxPred.size() - 5)
            //if(i == 2380)
            if(i == 0)
                std::cout << boxPred[i][j] << " ";
        //if(i < 5)
        //if(i > boxPred.size() - 5)
        //if(i == 2380) //dal terzultimo sono diversi rispetto a python perchè finisce il primo da 150000
        if(i == 0)
            std::cout << "   i:" << i << std::endl;
    }
}


void ModelBoxes::printClassPred()
{
    /*
    for(size_t i = 0; i < classPred.size(); ++i)
    {
        if( (i == 184988))
        {
            for(size_t j = 0; j < classPred.begin()->size(); ++j)
                std::cout << classPred[i][j] << " ";
        }
        
    }
    std::cout << std::endl;
    */
    
    for(size_t i = 0; i < classPred.size(); ++i)
    {
        for(size_t j = 0; j < classPred.begin()->size(); ++j)
            //if(i < 5)
            //if(i > classPred.size() - 5)
            //if(i == 2380)
            if(i == 200133)
                std::cout << classPred[i][j] << " ";
        //if(i < 5)
        //if(i > classPred.size() - 5)
        //if(i == 2380) //dal terzultimo sono diversi rispetto a python perchè finisce il primo da 150000
        if(i == 200133)
            std::cout << " " << std::endl;
    }
    
}


void ModelBoxes::applySigmoid(matrix2D &A)
{
    for(size_t i = 0; i < A.size(); ++i)
        for(size_t j = 0; j < A[i].size(); ++j)
            A[i][j] = 1 / (1 + exp(- A[i][j]));
}