
/*
 * @file PoseRenderer.cpp
 * @brief Implementation of the PoseRenderer class.
 * This file contains the implementation of the PoseRenderer class, which is responsible for visualizing pose estimation results on images.
 */

#include "PoseRenderer.h"

#include <array>

// MoveNet keypoint connections
static constexpr std::array<std::pair<int, int>, 16> POSE_CONNECTIONS =
{{
    {0, 1}, {0, 2},
    {1, 3}, {2, 4},
    {5, 6},
    {5, 7}, {7, 9},
    {6, 8}, {8, 10},
    {5, 11}, {6, 12},
    {11, 12},
    {11, 13}, {13, 15},
    {12, 14}, {14, 16}
}};


/**
 * @brief Constructs a PoseRenderer object.
 * @param confidenceThreshold Minimum confidence required to draw a keypoint.
 */
PoseRenderer::PoseRenderer(float confidenceThreshold)
    : confidenceThreshold(confidenceThreshold)
{
}


/**
 * @brief Draws pose estimation results on a frame.
 * @param frame Image on which the pose will be drawn.
 * @param keypoints Vector containing MoveNet keypoints.
 */
void PoseRenderer::draw(
    cv::Mat& frame,
    const std::vector<float>& keypoints)
{
    if (frame.empty())
        return;

    if (keypoints.size() != KEYPOINT_COUNT * VALUES_PER_KEYPOINT)
        return;

    drawConnections(frame, keypoints);
    drawKeypoints(frame, keypoints);
}


/**
 * @brief Draws detected keypoints.
 */
void PoseRenderer::drawKeypoints(
    cv::Mat& frame,
    const std::vector<float>& keypoints)
{
    const int width = frame.cols;
    const int height = frame.rows;

    for (int i = 0; i < KEYPOINT_COUNT; i++)
    {
        const int index = i * VALUES_PER_KEYPOINT;

        const float y = keypoints[index];
        const float x = keypoints[index + 1];
        const float score = keypoints[index + 2];

        if (score < confidenceThreshold)
            continue;

        const int px = static_cast<int>(x * width);
        const int py = static_cast<int>(y * height);

        cv::circle(
            frame,
            cv::Point(px, py),
            5,
            cv::Scalar(0, 0, 255),
            -1
        );
    }
}


/**
 * @brief Draws connections between detected keypoints.
 */
void PoseRenderer::drawConnections(
    cv::Mat& frame,
    const std::vector<float>& keypoints)
{
    const int width = frame.cols;
    const int height = frame.rows;

    for (const auto& connection : POSE_CONNECTIONS)
    {
        const int start = connection.first;
        const int end = connection.second;

        const int startIndex = start * VALUES_PER_KEYPOINT;
        const int endIndex = end * VALUES_PER_KEYPOINT;

        const float score1 = keypoints[startIndex + 2];
        const float score2 = keypoints[endIndex + 2];

        if (score1 < confidenceThreshold ||
            score2 < confidenceThreshold)
            continue;

        const int x1 = static_cast<int>(keypoints[startIndex + 1] * width);
        const int y1 = static_cast<int>(keypoints[startIndex] * height);

        const int x2 = static_cast<int>(keypoints[endIndex + 1] * width);
        const int y2 = static_cast<int>(keypoints[endIndex] * height);

        cv::line(
            frame,
            cv::Point(x1, y1),
            cv::Point(x2, y2),
            cv::Scalar(0, 255, 0),
            2
        );
    }
}
