#include "OcrYolov8.hpp"
#include "common/utils/utils.hpp"

OcrYolov8::OcrYolov8(std::string& modelPath, const cv::Size modelInputSize, const cv::Scalar meanValue, double confThreshold, double iouThreshold)
    : session(Ort::Session(env, modelPath.c_str(), Ort::SessionOptions {})), modelInputSize(modelInputSize), meanValue(meanValue), confThreshold(confThreshold),
      iouThreshold(iouThreshold) {}

cv::Rect2f OcrYolov8::scaleCoords(const cv::Size& imageShape, cv::Rect2f coords, const cv::Size& imageOriginalShape, bool p_Clip = false)
{
    cv::Rect2f rect;
    float gain = std::min((float)imageShape.height / (float)imageOriginalShape.height, (float)imageShape.width / (float)imageOriginalShape.width);
    int pad[2] = {(int)std::round((((float)imageShape.width - (float)imageOriginalShape.width * gain) / 2.0f) - 0.1f),
                  (int)std::round((((float)imageShape.height - (float)imageOriginalShape.height * gain) / 2.0f) - 0.1f)};
    rect.x = (int)std::round(((float)(coords.x - pad[0]) / gain));
    rect.y = (int)std::round(((float)(coords.y - pad[1]) / gain));
    rect.width = (int)std::round(((float)coords.width / gain));
    rect.height = (int)std::round(((float)coords.height / gain));
    if (p_Clip)
    {
        rect.x = clip(rect.x, (float)0, (float)imageOriginalShape.width);
        rect.y = clip(rect.y, (float)0, (float)imageOriginalShape.height);
        rect.width = clip(rect.width, (float)0, (float)(imageOriginalShape.width - rect.x));
        rect.height = clip(rect.height, (float)0, (float)(imageOriginalShape.height - rect.y));
    }
    return rect;
}

std::tuple<int, float> OcrYolov8::getBestClassInfo(const cv::Mat& p_Mat, const int& numClasses)
{
    int bestClassId = 0;
    float bestConf = 0;
    if (p_Mat.rows && p_Mat.cols)
    {
        for (int i = 0; i < numClasses; i++)
        {
            if (p_Mat.at<float>(0, i + 4) > bestConf)
            {
                bestConf = p_Mat.at<float>(0, i + 4);
                bestClassId = i;
            }
        }
    }
    return std::make_tuple(bestClassId, bestConf);
}

std::vector<Character> OcrYolov8::nonMaximaSuppression(std::vector<Character> detections, double confThreshold, double iouThreshold)
{
    std::vector<cv::Rect> boxes;
    std::vector<float> confs;
    std::vector<int> classIds;
    std::vector<char*> classes;
    for (Character detection : detections)
    {
        cv::Rect rect = utils::fromCoordinatesToCvRect(detection.position);
        boxes.push_back(rect);
        confs.push_back(detection.confidence);
        classIds.push_back(detection.labelIndex);
        classes.push_back(detection.label);
    }
    std::vector<Character> detectionsPostNMS;
    if (detections.size() != 0)
    {
        std::vector<int> indices;
        cv::dnn::NMSBoxes(boxes, confs, confThreshold, iouThreshold, indices);
        for (unsigned idx : indices)
        {
            Character detection;
            Coordinates position = utils::fromCvRectToCoordinates(boxes[idx]);
            detection.position = position;
            detection.confidence = confs[idx];
            detection.labelIndex = classIds[idx];
            detection.label = classes[idx];
            detectionsPostNMS.push_back(detection);
        }
        SPDLOG_INFO("{} Documents Detected post NMS", detectionsPostNMS.size());
    }
    return detectionsPostNMS;
}

std::vector<Character> OcrYolov8::postprocessing(const cv::Size& resizedImageShape, const cv::Size& originalImageShape, std::vector<Ort::Value>& outputTensors,
                                              double confThreshold, double iouThreshold)
{
    std::vector<Character> detections;
    auto* rawOutput = outputTensors[0].GetTensorData<float>();
    std::vector<int64_t> outputShape = outputTensors[0].GetTensorTypeAndShapeInfo().GetShape();
    cv::Mat mat = cv::Mat(outputShape[1], outputShape[2], CV_32FC1, (void*)rawOutput);
    cv::Mat matT = mat.t();
    int numClasses = matT.cols - 4;
    for (int row = 0; row < matT.rows; row++)
    {
        cv::Mat matRow = matT.row(row);
        std::tuple<int, float> confResults = getBestClassInfo(matRow, numClasses);
        int classId = std::get<0>(confResults);
        float objConf = std::get<1>(confResults);
        if (objConf > confThreshold)
        {
            float centerX = (matRow.at<float>(0, 0));
            float centerY = (matRow.at<float>(0, 1));
            float width = (matRow.at<float>(0, 2));
            float height = (matRow.at<float>(0, 3));
            float left = centerX - width / 2;
            float top = centerY - height / 2;
            float confidence = objConf;
            cv::Rect2f scaled = scaleCoords(resizedImageShape, cv::Rect2f(left, top, width, height), originalImageShape, true);
            cv::Rect rectangle = cv::Rect((int)std::round(scaled.x), (int)std::round(scaled.y), (int)std::round(scaled.width), (int)std::round(scaled.height));
            Character detection;
            detection.position = utils::fromCvRectToCoordinates(rectangle);
            detection.confidence = confidence;
            detection.labelIndex = classId;
            detection.label = utils::convertStringtoCharPtr(utils::dictionary[int(classId)]);
            detections.push_back(detection);
        }
    }
    SPDLOG_INFO("{} Documents Detected pre NMS", detections.size());
    return detections;
}

cv::Mat OcrYolov8::letterbox(const cv::Mat image, const cv::Size& modelInputSize, const cv::Scalar& meanValue, bool auto_ = true, bool scaleFill = false,
                          bool scaleUp = true, int stride = 32)
{
    cv::Mat outImage;
    cv::Size shape = image.size();
    float r = std::min((float)modelInputSize.height / (float)shape.height, (float)modelInputSize.width / (float)shape.width);
    if (!scaleUp)
        r = std::min(r, 1.0f);
    float ratio[2] {r, r};
    int newUnpad[2] {(int)std::round((float)shape.width * r), (int)std::round((float)shape.height * r)};
    auto dw = (float)(modelInputSize.width - newUnpad[0]);
    auto dh = (float)(modelInputSize.height - newUnpad[1]);
    if (auto_)
    {
        dw = (float)((int)dw % stride);
        dh = (float)((int)dh % stride);
    }
    else if (scaleFill)
    {
        dw = 0.0f;
        dh = 0.0f;
        newUnpad[0] = modelInputSize.width;
        newUnpad[1] = modelInputSize.height;
        ratio[0] = (float)modelInputSize.width / (float)shape.width;
        ratio[1] = (float)modelInputSize.height / (float)shape.height;
    }
    dw /= 2.0f;
    dh /= 2.0f;
    if (shape.width == newUnpad[0] && shape.height == newUnpad[1])
        outImage = image;
    else
        cv::resize(image, outImage, cv::Size(newUnpad[0], newUnpad[1]));
    int top = int(std::round(dh - 0.1f));
    int bottom = int(std::round(dh + 0.1f));
    int left = int(std::round(dw - 0.1f));
    int right = int(std::round(dw + 0.1f));
    cv::copyMakeBorder(outImage, outImage, top, bottom, left, right, cv::BORDER_CONSTANT, meanValue);
    return outImage;
}

std::vector<float> OcrYolov8::blobImage(cv::Mat image)
{
    cv::Mat resizedImage, floatImage;
    cv::cvtColor(image, resizedImage, cv::COLOR_BGR2RGB);
    cv::Mat preprocessedImage = letterbox(resizedImage, modelInputSize, meanValue, false, false, true, 32);
    preprocessedImage.convertTo(floatImage, CV_32FC3, 1 / 255.0);

    cv::Size floatImageSize {floatImage.cols, floatImage.rows};
    std::vector<cv::Mat> chw(floatImage.channels());

    std::unique_ptr<float[]> blob(new float[floatImage.cols * floatImage.rows * floatImage.channels()]);
    for (int i = 0; i < floatImage.channels(); ++i)
        chw[i] = cv::Mat(floatImageSize, CV_32FC1, blob.get() + i * floatImageSize.width * floatImageSize.height);
    cv::split(floatImage, chw);

    std::vector<float> inputTensorValues(blob.get(), blob.get() + 3 * floatImageSize.width * floatImageSize.height);
    return inputTensorValues;
}

std::vector<Character> OcrYolov8::resultImage(const cv::Mat frame)
{
    SPDLOG_INFO("Extract boxes from OcrYolov8 model");
    std::vector<float> inputTensorValues = blobImage(frame);
    Ort::MemoryInfo memoryInfo = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);
    std::vector<int64_t> inputShape = session.GetInputTypeInfo(0).GetTensorTypeAndShapeInfo().GetShape();
    Ort::Value inputTensors = Ort::Value::CreateTensor<float>(memoryInfo, inputTensorValues.data(), inputTensorValues.size(), inputShape.data(), inputShape.size());

    SPDLOG_INFO("Run session");
    Ort::AllocatedStringPtr inputName = session.GetInputNameAllocated(0, Ort::AllocatorWithDefaultOptions());
    std::vector<const char*> inputNames = {inputName.get()};
    std::vector<const char*> outputNames = {"output0"};
    std::vector<Ort::Value> output_tensors = session.Run(Ort::RunOptions {nullptr}, inputNames.data(), &inputTensors, 1, outputNames.data(), 1);

    std::vector<Character> detections = postprocessing(modelInputSize, frame.size(), output_tensors, confThreshold, iouThreshold);

    SPDLOG_INFO("Boxes extracted from OcrYolov8 model");
    return detections;
}

std::vector<Character> OcrYolov8::detect(const cv::Mat image, const float confidenceThreshold)
{
    std::vector<Character> detections = resultImage(image);
    std::vector<Character> characters = nonMaximaSuppression(detections, confidenceThreshold, iouThreshold);
    return characters;
}