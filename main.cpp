#include <iostream>
#include <vector>
#include <numeric>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "src/DBSCAN.h"
#include "src/Characters.h"
#include "src/Anchors.h"

#define FEATURE_WIDTH 800
#define FEATURE_HEIGHT 800
#define NUM_CLASSES 64

typedef std::vector<std::vector<float>> matrix2D;
typedef std::vector<std::vector<std::vector<float>>> matrix3D;
typedef std::vector<std::vector<std::vector<std::vector<float>>>> matrix4D;

typedef std::deque<std::deque<float>> deque2D;

matrix2D extractPredCVMat(cv::Mat boxPrediction)
{
    matrix2D boxPred;
    std::vector<float> box;
    cv::Vec<int,4> idx;
    float val;

    for(int i = 0; i < boxPrediction.size[3]; ++i)
    {
        for(int j = 0; j < boxPrediction.size[1]; ++j)
        {
            idx = {0, j, 0, i};
            val = boxPrediction.at<float>(idx);
            //if(i < 5)
            if(i > 200140)
                std::cout << val << " ";
            box.push_back(val);
        }
        //if(i < 5)
        if(i > 200140)
            std::cout << " " << std::endl;
        boxPred.push_back(box);
        box.clear();
        
    }
    return boxPred;
}


matrix2D extractPredCVMatInv(cv::Mat boxPrediction)
{
    matrix2D boxPred;
    std::vector<float> box;
    cv::Vec<int,4> idx;
    float val;

    for(int i = 0; i < boxPrediction.size[1]; ++i)
    {
        for(int j = 0; j < boxPrediction.size[2]; ++j)
        {
            idx = {0, i, j, 0};
            val = boxPrediction.at<float>(idx);
            //if(i < 5)
            if(i > 200140)
                std::cout << val << " ";
            box.push_back(val);
        }
        //if(i < 5)
        if(i > 200140)
            std::cout << " " << std::endl;
        boxPred.push_back(box);
        box.clear();
        
    }
    return boxPred;
}


deque2D extractPredCVMatNew(cv::Mat boxPrediction)
{
    deque2D boxPred;
    std::deque<float> box;
    cv::Vec<int,4> idx;
    float val;

    std::cout << boxPrediction.size[3] << std::endl;
    std::cout << boxPrediction.size[1] << std::endl;
    int n = 0;
    for(int i = boxPrediction.size[3]; i > 0; i--)
    {
        for(int j = boxPrediction.size[1]; j > 0; j--)
        {
            idx = {0, j-1, 0, i-1};
            val = boxPrediction.at<float>(idx);
            if(n < 10)
            {
                std::cout << "j-1: " << j-1 << " i-1: " << i-1 << " val: " << val << std::endl;
                n += 1;
            }
            
            //if(i < 5)
            //if(i > 200140)
            //    std::cout << val << " ";
            box.push_front(val);
        }
        //if(i < 5)
        //if(i > 200140)
        //    std::cout << " " << std::endl;
        boxPred.push_front(box);
        box.clear();
        
    }
    return boxPred;
}

std::vector<float> flatten(cv::Mat boxPrediction)
{
    cv::Vec<int,4> idx;
    float val;
    std::vector<float> values;
    for(int i = 0; i < boxPrediction.size[3]; ++i)
    {
        for(int j = 0; j < boxPrediction.size[1]; ++j)
        {
            idx = {0, j, 0, i};
            val = boxPrediction.at<float>(idx);
            values.push_back(val);
        }
    }
    return values;
}

deque2D extractPredCVMatFlat(cv::Mat boxPrediction)
{
    deque2D boxPred;
    std::deque<float> box;
    float val;
    std::vector<float> values = flatten(boxPrediction);

    std::cout << values.size() << std::endl;
    int n = 0;

    for(int i = values.size(); i > 0; i = i - 196)
    {
        val = values.at(i-1);
        if(n < 1000)
        {
            std::cout << " i-1: " << i-1 << " val: " << val << std::endl;
            
        }
        box.push_front(val);
        n += 1;
        if(n % 4 == 0)
        {
            boxPred.push_front(box);
            box.clear();
        }          
    }
    return boxPred;
}


void printPredCVMatNew(deque2D boxPrediction)
{
    std::cout << boxPrediction.size() << std::endl;
    std::cout << boxPrediction.begin()->size() << std::endl;
    for(size_t i = 0; i != boxPrediction.size(); ++i)
    {
        for(size_t j = 0; j != boxPrediction.begin()->size(); ++j)
        {
            //if(i > 200140)
                std::cout << boxPrediction.at(j).at(i) << " ";
        }
        //if(i < 5)
        //if(i > 200140)
        std::cout << " " << std::endl;
    }

}


void applySigmoid(matrix2D &A)
{
    for(size_t i = 0; i < A.size(); ++i)
    {
        for(size_t j = 0; j < A[0].size(); ++j)
        {
            //std::cout << A[i][j] << "\t\t";
            A[i][j] = 1 / (1 + exp(- A[i][j]));
            //A[i][j] = (float)1 / ( 1 + exp(- A[i][j]));
            //std::cout << A[i][j] << std::endl;
        }
    }
}


matrix2D computeCenters(matrix2D boxes, matrix2D anchorBoxes)
{
    matrix2D center = boxes;
    for(size_t i = 0; i < boxes.size(); ++i)
    {
        center[i][0] = boxes[i][2] * anchorBoxes[i][0] + anchorBoxes[i][2];
        center[i][1] = boxes[i][3] * anchorBoxes[i][2] + anchorBoxes[i][3];
        center[i][2] = std::exp(boxes[i][0]) *  anchorBoxes[i][0];
        center[i][3] = std::exp(boxes[i][1]) *  anchorBoxes[i][1];
        //if(i <= 10)
            //std::cout << boxes[i][0] << " " << boxes[i][1] << " " << boxes[i][2] << " " << boxes[i][3] << " " << std::endl;
            // std::cout << center[i][0] << " " << center[i][1] << " " << center[i][2] << " " << center[i][3] << " " << std::endl;
    }
    return center;
}

float IoU(std::vector<float> boxA, std::vector<float> boxB)
{
    // https://pyimagesearch.com/2016/11/07/intersection-over-union-iou-for-object-detection/
    // determine the (x, y)-coordinates of the intersection rectangle
    float xA = std::max(boxA[0], boxB[0]);
	float yA = std::max(boxA[1], boxB[1]);
	float xB = std::min(boxA[2], boxB[2]);
	float yB = std::min(boxA[3], boxB[3]);
    
    // compute the area of intersection rectangle
    float interArea = std::max(static_cast<float>(0.), xB - xA + 1) * std::max(static_cast<float>(0.), yB - yA + 1);

    // compute the area of both the prediction and ground-truth rectangles
	float boxAArea = (boxA[2] - boxA[0] + 1) * (boxA[3] - boxA[1] + 1);
	float boxBArea = (boxB[2] - boxB[0] + 1) * (boxB[3] - boxB[1] + 1);

    // compute the intersection over union by taking the intersection
	// area and dividing it by the sum of prediction + ground-truth
	// areas - the interesection area
	float iou = interArea / (boxAArea + boxBArea - interArea);

    return iou;
}


/*
void NonMaxSuppression(matrix2D boxes, float threshold, matrix2D)
{
    std::vector<bool> nms;
    bool discard;
    for(size_t i = 0; i < boxes.size(); ++i)
    {
        discard = false;
        for(size_t j = 0; j < boxes.size(); ++j)
        {
            if(IoU(boxes[i], boxes[j]) > threshold)
            {
                //if(score)
            }
        }  
    }
}
*/


/*
boxes = tf.concat(
        [
            boxes[:, :, :2] * anchor_boxes[:, :, 2:] + anchor_boxes[:, :, :2],
            tf.math.exp(boxes[:, :, 2:]) * anchor_boxes[:, :, 2:],
        ],
        axis=-1,
    )
*/

static void hwc_to_chw(cv::InputArray src, cv::OutputArray dst) {
  std::vector<cv::Mat> channels;
  cv::split(src, channels);

  // Stretch one-channel images to vector
  for (auto &img : channels) {
    img = img.reshape(1, 1);
  }

  // Concatenate three vectors to one
  cv::hconcat( channels, dst );
}

int main()
{
    // Load network
    std::string networkWeights = "/home/f_pietrobon/thesis/MRZ_Antitampering/models/Frozen_graph_test_0.25_new3.pb";

    cv::dnn::Net network = cv::dnn::readNetFromTensorflow(networkWeights);
    
    cv::Mat document = cv::imread("/home/f_pietrobon/thesis/MRZ_Antitampering/AFG_AO_01001_FRONT.JPG", cv::IMREAD_COLOR);
    std::cout << document.size[0] << " x " << document.size[1] << std::endl;
   
    //cv::cvtColor(document, document, cv::COLOR_BGR2RGB);
    cv::resize(document, document, cv::Size(800, 800), 0, 0, cv::INTER_CUBIC);
    /*
    //cv::resize(img, img, input_size);
    document.convertTo(document, CV_32FC3, 1.f / 255.f);
    std::vector<float> chw_buffer(document.elemSize() * document.total());
    std::vector<cv::Mat> chw;
    for (size_t n = 0; n < 3; ++n)
        chw.emplace_back(cv::Mat(cv::Size(800, 800), CV_32FC1, chw_buffer.data() + n * 800 * 800));
    cv::split(document, chw);
    */
    //cudaMemcpyAsync(gpu_buffers[inputIndex], chw_buffer.size(), cudaMemcpyHostToDevice, stream);
    //document = (2.0/255.0) * document - 1.0;
    //document = document.t();

    std::cout << document.size[0] << " x " << document.size[1] << std::endl;
    
    
    cv::imwrite("/home/f_pietrobon/thesis/MRZ_Antitampering/image.jpeg", document);

    // network prediction
    cv::Mat blob = cv::dnn::blobFromImage(document, 1.0, cv::Size(800, 800), cv::Scalar(0,0,0), true, false);
    //hwc_to_chw(blob, blob);
    //std::vector<int> order = {0, 2, 1, 3};
    //blob = blob.t();
    //cv::transpose(blob, blob);
    
    /*
    //cv::Mat document = cv::imread("/home/f_pietrobon/thesis/MRZ_Antitampering/AFG_AO_01001_FRONT.JPG", cv::IMREAD_COLOR);
    cv::Mat document = cv::imread("/home/f_pietrobon/thesis/MRZ_Antitampering/BGR_AO_02001_FRONT.jpeg", cv::IMREAD_COLOR);
    //cv::Mat document = cv::imread("/home/f_pietrobon/thesis/MRZ_Antitampering/BGR_AO_02001_FRONT.jpeg", cv::COLOR_BGR2RGB);
    cv::cvtColor(document, document, cv::COLOR_BGR2RGB);
    //cv::fastNlMeansDenoising(document, document, 10);
    cv::resize(document, document, cv::Size(FEATURE_WIDTH, FEATURE_HEIGHT), 0, 0, cv::INTER_CUBIC); 
    cv::imwrite("/home/f_pietrobon/thesis/MRZ_Antitampering/image.jpeg", document);

    // network prediction
    //int sz[] = {1, document.rows, document.cols, 3};
    //cv::Mat blob(4, sz, CV_32F, document.data);
    //cv::Mat blob = cv::dnn::blobFromImage(document, 1.0, cv::Size(FEATURE_WIDTH, FEATURE_HEIGHT));
    cv::Mat blob = cv::dnn::blobFromImage(document, 1.0, cv::Size(FEATURE_WIDTH, FEATURE_HEIGHT), cv::Scalar(0,0,0), false, false, CV_32F);
    std::cout << blob.size[0] << " x " << blob.size[1] << " x " << blob.size[2] << " x " << blob.size[3] << std::endl;
    */

    network.setInput(blob);

    cv::Mat prediction = network.forward();
    
    std::cout << "Predictions:\t";
    std::cout << prediction.size[0] << " x " << prediction.size[1] << " x " << prediction.size[2] << " x " << prediction.size[3] << std::endl;
    //matrix2D pred = extractPredCVMat(prediction);
    //deque2D pred = extractPredCVMatNew(prediction);
    
    cv::Range boxRange(0, 4);
    cv::Range classRange(4, NUM_CLASSES + 5);
    cv::Range all(cv::Range::all());
    // if 1 69 1 200145
    std::vector<cv::Range> boxRanges = {all, boxRange, all, all};
    std::vector<cv::Range> classRanges = {all, classRange, all, all};
    // if 1 200145 69 1 
    //std::vector<cv::Range> boxRanges = {all, all, boxRange, all};
    //std::vector<cv::Range> classRanges = {all, all, classRange, all};

    cv::Mat boxPrediction = prediction(boxRanges);
    std::cout << "Box pred:\t";
    std::cout << boxPrediction.size[0] << " x " << boxPrediction.size[1] << " x " << boxPrediction.size[2] << " x " << boxPrediction.size[3] << std::endl;
    
    cv::Mat classPrediction = prediction(classRanges);
    std::cout << "Class pred:\t";
    std::cout << classPrediction.size[0] << " x " << classPrediction.size[1] << " x " << classPrediction.size[2] << " x " << classPrediction.size[3] << std::endl;

    //matrix2D boxPred = extractPredCVMatInv(boxPrediction);
    matrix2D boxPred = extractPredCVMat(boxPrediction);
    //deque2D boxPred = extractPredCVMatNew(boxPrediction);
    //deque2D boxPred = extractPredCVMatFlat(boxPrediction);
    //printPredCVMatNew(boxPred);
    //printPredCVMatNew(boxPred);
    std::cout << "New box pred:\t";
    std::cout << boxPred.size() << " x " << boxPred.begin()->size() << std::endl;

    matrix2D classPred = extractPredCVMat(classPrediction);
    //deque2D classPred = extractPredCVMatNew(classPrediction);
    /*
    deque2D classPred = extractPredCVMatFlat(classPrediction);
    //printPredCVMatNew(classPred);
    std::cout << "New class pred:\t";
    std::cout << classPred.size() << " x " << classPred.begin()->size() << std::endl;
    */

    /*
    applySigmoid(classPred);
    std::cout << "Sig class pred:\t";
    std::cout << classPred.size() << " x " << classPred.begin()->size() << std::endl;

    Anchors anchors;
    matrix2D anchorBoxes = anchors.anchorsGenerator();

    matrix2D centers = computeCenters(boxPred, anchorBoxes);
    */
    //Anchors anchors;
    //anchors.anchorsGenerator();

    // NON MAX SUPPRESSION WITH cv::NMSBoxes()

    // Uncomment to see the time needed to load the network
    //auto start_network = std::chrono::high_resolution_clock::now();
    //cv::Mat detections = network.forward();
    //auto stop_network = std::chrono::high_resolution_clock::now();
    //auto duration_network = std::chrono::duration_cast<std::chrono::milliseconds>(stop_network - start_network);
    //std::cout << "Time taken to predict: " << duration_network.count() << " milliseconds" << std::endl;
    
    //std::cout << detections.size[0] << " " << detections.size[1] << " " << detections.size[2] << " " << detections.size[3] << std::endl;
    //cv::Mat output(detections.size[2], detections.size[3], CV_32F, detections.ptr<float>());
    //std::cout << output << std::endl;    

    return 0;
}