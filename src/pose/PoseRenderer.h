
/*
 * @file PoseRenderer.h
 * @brief Header file for the PoseRenderer class.
 * This class is responsible for visualizing pose estimation results.
 */

#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

/**
 * @class PoseRenderer
 * @brief A class for rendering pose estimation results on images.
 */
class PoseRenderer
{
public:
    explicit PoseRenderer(float confidenceThreshold = 0.3f);

    void draw(cv::Mat& frame, const std::vector<float>& keypoints);

private:
    float confidenceThreshold;

    static constexpr int KEYPOINT_COUNT = 17;
    static constexpr int VALUES_PER_KEYPOINT = 3;

    void drawKeypoints(cv::Mat& frame, const std::vector<float>& keypoints);
    void drawConnections(cv::Mat& frame, const std::vector<float>& keypoints);
};
