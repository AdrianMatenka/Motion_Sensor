/* 
 * @file PoseSmoother.cpp
 * @brief Implementation file for the PoseSmoother class.
 * This file contains the implementation of the PoseSmoother class, which is responsible for smoothing pose estimation keypoints over time.
 */

#include "PoseSmoother.h"
#include <algorithm>

/**
 * @brief Constructor for the PoseSmoother class.
 * @param alpha The smoothing factor for the exponential moving average.
 * @param confidenceThreshold The minimum confidence threshold for keypoints.
 */
PoseSmoother::PoseSmoother(
    float alpha,
    float confidenceThreshold
)
    : alpha(alpha),
      confidenceThreshold(confidenceThreshold),
      previousKeypoints(KEYPOINT_COUNT * VALUES_PER_KEYPOINT, 0.0f),
      initialized(KEYPOINT_COUNT, false)
{
}

/**
 * @brief Smooths the pose keypoints over time.
 * @param keypoints The vector of keypoints to smooth.
 */
void PoseSmoother::smooth(std::vector<float>& keypoints)
{
    if (keypoints.size() != KEYPOINT_COUNT * VALUES_PER_KEYPOINT)
        return;

    for (int i = 0; i < KEYPOINT_COUNT; i++)
    {
        const int index = i * VALUES_PER_KEYPOINT;

        float& y = keypoints[index];
        float& x = keypoints[index + 1];
        const float confidence = keypoints[index + 2];

        // Ignore points with low confidence
        if (confidence < confidenceThreshold)
            continue;

        // Initialize point on first detection
        if (!initialized[i])
        {
            previousKeypoints[index] = y;
            previousKeypoints[index + 1] = x;
            initialized[i] = true;
            continue;
        }

        // Apply exponential moving average
        y = alpha * y + (1.0f - alpha) * previousKeypoints[index];
        x = alpha * x + (1.0f - alpha) * previousKeypoints[index + 1];

        // Save smoothed coordinates
        previousKeypoints[index] = y;
        previousKeypoints[index + 1] = x;
    }
}

/**
 * @brief Resets the pose smoother.
 */
void PoseSmoother::reset()
{
    std::fill(initialized.begin(), initialized.end(), false);
}
