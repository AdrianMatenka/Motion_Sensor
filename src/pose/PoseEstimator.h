/*
* @file PoseEstimator.h
* @brief Header file for the PoseEstimator class
* This file defines the PoseEstimator class, which is responsible for loading a pose estimation model,
*/
#pragma once

#include <vector>

#include <onnxruntime_cxx_api.h>
#include <opencv2/opencv.hpp>

enum class PoseEstimatorError
{
    POSE_OK = 0,
    INVALID_FRAME,
    PREPROCESSING_FAILED,
    INFERENCE_FAILED,
    INVALID_OUTPUT
};

/**
 * @class PoseEstimator
 * @brief A class for estimating pose from video frames.
 * This class provides methods for loading a pose estimation model and estimating pose from video frames.
 */
class PoseEstimator
{
public:
    explicit PoseEstimator(const ORTCHAR_T* modelPath);

    PoseEstimatorError estimate(
        const cv::Mat& frame,
        std::vector<float>& keypoints
    );

private:

    struct PreprocessInfo
    {
        float scaleX;
        float scaleY;

        int padX;
        int padY;

        int originalWidth;
        int originalHeight;
    };

    cv::Mat preprocess(
        const cv::Mat& frame,
        PreprocessInfo& info
    );

    Ort::Env env;
    Ort::SessionOptions sessionOptions;
    Ort::Session session;
};