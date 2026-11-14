/* 
 * @file GestureDetector.h
 * @brief Header file for the GestureDetector class.
 * This file contains the definition of the GestureDetector class, which is responsible for detecting gestures based on keypoint data.
 */

#pragma once

#include "Gesture.h"

#include <memory>
#include <vector>

/**
 * @class GestureDetector
 * @brief Class for detecting gestures based on keypoint data.
 */
class GestureDetector
{
public:
    void addGesture(std::unique_ptr<Gesture> gesture);

    void update(const std::vector<float>& keypoints);

    GestureState getGestureState(std::size_t index) const;

    std::size_t getGestureCount() const;

private:
    std::vector<std::unique_ptr<Gesture>> gestures;
};
