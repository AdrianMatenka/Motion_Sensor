
/*
 * @file PoseEstimator.cpp
 * @brief Implementation of the PoseEstimator class.
 */

#include "PoseEstimator.h"

#include <array>
#include <cstdint>
#include <iostream>
#include <algorithm>
#include <cmath>

// Model configuration
constexpr int INPUT_WIDTH = 192;
constexpr int INPUT_HEIGHT = 192;
constexpr int INPUT_CHANNELS = 4;
constexpr int KEYPOINT_COUNT = 17;
constexpr int VALUES_PER_KEYPOINT = 3;
constexpr int OUTPUT_SIZE = KEYPOINT_COUNT * VALUES_PER_KEYPOINT;

// Input and output names
constexpr const char* INPUT_NAME = "pixel_values";
constexpr const char* OUTPUT_NAME = "keypoints";


/**
 * @brief Constructs a PoseEstimator object.
 * @param modelPath Path to the ONNX model.
 */
PoseEstimator::PoseEstimator(const ORTCHAR_T* modelPath)
    : env(ORT_LOGGING_LEVEL_WARNING, "MotionSensor"),
      sessionOptions(),
      session(env, modelPath, sessionOptions)
{
    std::cout << "Pose model loaded successfully!" << std::endl;
}

/**
 * @brief Preprocesses a frame before inference.
 * @param frame Input BGR image.
 * @param info Preprocessing information.
 * @return Preprocessed RGBA image.
 */
cv::Mat PoseEstimator::preprocess(
    const cv::Mat& frame,
    PreprocessInfo& info)
{
    cv::Mat resized;
    cv::Mat rgba;
    cv::Mat padded;

    // Store original image dimensions
    info.originalWidth = frame.cols;
    info.originalHeight = frame.rows;

    // Calculate scale while preserving aspect ratio
    const float scale = std::min(
        static_cast<float>(INPUT_WIDTH) / frame.cols,
        static_cast<float>(INPUT_HEIGHT) / frame.rows
    );

    // Calculate resized dimensions
    const int resizedWidth =
        static_cast<int>(std::round(frame.cols * scale));

    const int resizedHeight =
        static_cast<int>(std::round(frame.rows * scale));

    // Resize image
    cv::resize(
        frame,
        resized,
        cv::Size(resizedWidth, resizedHeight)
    );

    // Calculate padding
    info.padX = (INPUT_WIDTH - resizedWidth) / 2;
    info.padY = (INPUT_HEIGHT - resizedHeight) / 2;

    // Store actual scale after rounding
    info.scaleX =
        static_cast<float>(resizedWidth) / frame.cols;

    info.scaleY =
        static_cast<float>(resizedHeight) / frame.rows;

    // Convert BGR to RGBA
    cv::cvtColor(
        resized,
        rgba,
        cv::COLOR_BGR2RGBA
    );

    // Add black padding
    cv::copyMakeBorder(
        rgba,
        padded,
        info.padY,
        INPUT_HEIGHT - resizedHeight - info.padY,
        info.padX,
        INPUT_WIDTH - resizedWidth - info.padX,
        cv::BORDER_CONSTANT,
        cv::Scalar(0, 0, 0, 255)
    );

    return padded;
}

/**
 * @brief Estimates human pose from an image.
 * @param frame Input image from the camera.
 * @param keypoints Vector containing detected keypoints.
 * @return PoseEstimatorError status.
 */
PoseEstimatorError PoseEstimator::estimate(
    const cv::Mat& frame,
    std::vector<float>& keypoints)
{
    keypoints.clear();

    // Validate input frame
    if (frame.empty())
        return PoseEstimatorError::INVALID_FRAME;

    if (frame.type() != CV_8UC3)
        return PoseEstimatorError::INVALID_FRAME;

    
    PreprocessInfo info;

    cv::Mat inputImage;

    try
    {
        inputImage = preprocess(frame, info);
    }
    catch (const cv::Exception&)
    {
        return PoseEstimatorError::PREPROCESSING_FAILED;
    }

    if (inputImage.empty() || inputImage.type() != CV_8UC4)
        return PoseEstimatorError::PREPROCESSING_FAILED;

    // Ensure continuous memory
    if (!inputImage.isContinuous())
        inputImage = inputImage.clone();

    // Input tensor dimensions: [1, 192, 192, 4]
    const std::array<int64_t, 4> inputShape =
    {
        1,
        INPUT_HEIGHT,
        INPUT_WIDTH,
        INPUT_CHANNELS
    };

    try
    {
        // Create ONNX Runtime memory info
        Ort::MemoryInfo memoryInfo =
            Ort::MemoryInfo::CreateCpu(
                OrtArenaAllocator,
                OrtMemTypeDefault
            );

        // Create input tensor
        Ort::Value inputTensor =
            Ort::Value::CreateTensor<uint8_t>(
                memoryInfo,
                inputImage.data,
                inputImage.total() * inputImage.channels(),
                inputShape.data(),
                inputShape.size()
            );

        // Run inference
        auto outputTensors = session.Run(
            Ort::RunOptions{nullptr},
            &INPUT_NAME,
            &inputTensor,
            1,
            &OUTPUT_NAME,
            1
        );

        // Validate output
        if (outputTensors.size() != 1 ||
            !outputTensors[0].IsTensor())
        {
            return PoseEstimatorError::INVALID_OUTPUT;
        }

        auto outputInfo =
            outputTensors[0].GetTensorTypeAndShapeInfo();

        if (outputInfo.GetElementType() != ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT ||
            outputInfo.GetElementCount() != OUTPUT_SIZE)
        {
            return PoseEstimatorError::INVALID_OUTPUT;
        }

        // Read model output
        const float* outputData =
            outputTensors[0].GetTensorData<float>();

        keypoints.assign(
            outputData,
            outputData + OUTPUT_SIZE
        );

        // Map keypoints back to original image coordinates
        for (int i = 0; i < KEYPOINT_COUNT; i++)
        {
            const int index = i * VALUES_PER_KEYPOINT;

            float& y = keypoints[index];
            float& x = keypoints[index + 1];

            // Convert normalized model coordinates to original coordinates
            x = (x * INPUT_WIDTH - info.padX)
                / info.scaleX / info.originalWidth;

            y = (y * INPUT_HEIGHT - info.padY)
                / info.scaleY / info.originalHeight;
        }
    }
    catch (const Ort::Exception&)
    {
        keypoints.clear();
        return PoseEstimatorError::INFERENCE_FAILED;
    }

    return PoseEstimatorError::POSE_OK;
}
