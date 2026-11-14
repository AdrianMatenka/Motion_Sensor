/**
 * @file PoseSmoother.h
 * @brief Header file for the PoseSmoother class.
 * This file contains the declaration of the PoseSmoother class, which is responsible for smoothing pose estimation keypoints over time.
 */

#pragma once

#include <vector>

/**
 * @class PoseSmoother
 * @brief A class for smoothing pose estimation keypoints over time.
 */
class PoseSmoother
{
public:
    explicit PoseSmoother(
        float alpha = 0.4f,
        float confidenceThreshold = 0.3f
    );

    void smooth(std::vector<float>& keypoints);

    void reset();

private:
    static constexpr int KEYPOINT_COUNT = 17;
    static constexpr int VALUES_PER_KEYPOINT = 3;

    float alpha;
    float confidenceThreshold;

    std::vector<float> previousKeypoints;
    std::vector<bool> initialized;
};
