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


matrix2D extractPredCVMat(cv::Mat boxPrediction)
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


void applySigmoid(matrix2D &A)
{
    for(size_t i = 0; i < A.size(); ++i)
        for(size_t j = 0; j < A[0].size(); ++j)
            A[i][j] = 1 / (1 + exp(- A[i][j]));
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


matrix2D computeCorners(matrix2D boxes)
{
    matrix2D corners = boxes;
    for(size_t i = 0; i < boxes.size(); ++i)
    {
        corners[i][0] = boxes[i][0] - boxes[i][2] / 2;
        corners[i][1] = boxes[i][1] - boxes[i][3] / 2;
        corners[i][2] = boxes[i][0] + boxes[i][2] / 2;
        corners[i][3] = boxes[i][1] + boxes[i][3] / 2;  
    }
    return corners;
}


void extractImageWithPrediction(cv::Mat img, matrix2D boxes, matrix2D pred, float threshold, float threshold_nms)
{
    std::vector<float> indices_max;
    std::vector<float> all_max;
    std::vector<float>::iterator max_it;
    float index_max;
    float max;
    matrix2D boxes_new;
    std::vector<cv::Rect2d> boxes_rect;
    
    for(size_t i = 0; i < pred.size(); ++i)
    {
        max = *std::max_element(pred[i].begin(), pred[i].end());
        max_it = std::max_element(pred[i].begin(), pred[i].end());
        index_max = std::distance(pred[i].begin(), max_it);

        if(max > threshold)
        {
            all_max.push_back(max);
            indices_max.push_back(index_max);
            boxes_new.push_back(boxes[i]);
            cv::Rect2d rect(boxes[i][0], boxes[i][1], boxes[i][2] - boxes[i][0], boxes[i][3] - boxes[i][1]);
            boxes_rect.push_back(rect);
        }
    }

    std::vector<int> nmsIndices;
    cv::dnn::dnn4_v20220524::NMSBoxes(boxes_rect, all_max, threshold, threshold_nms, nmsIndices);
    
    matrix2D boxes_nms;
    std::vector<float> indices_max_nms;
    std::vector<float> all_max_nms;
    //std::cout << "Number of chars after NMS: " << nmsIndices.size() << std::endl;

    for(size_t i = 0; i < nmsIndices.size(); ++i)
    {
        boxes_nms.push_back(boxes_new[nmsIndices[i]]);
        indices_max_nms.push_back(indices_max[nmsIndices[i]]);
        all_max_nms.push_back(all_max[nmsIndices[i]]);
    }

    savePredictionImage(img, boxes_nms, indices_max_nms, "../pred_post_nms.jpg");
    
}