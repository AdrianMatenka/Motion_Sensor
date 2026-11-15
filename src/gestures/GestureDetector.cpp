/* 
 * @file GestureDetector.cpp
 * @brief Implementation file for the GestureDetector class.
 * This file contains the implementation of the GestureDetector class methods.
 */

#include "GestureDetector.h"

#include <stdexcept>
#include <utility>

/**
 * @brief Adds a gesture to the detector.
 * @param gesture The gesture to add.
 */
void GestureDetector::addGesture(std::unique_ptr<Gesture> gesture)
{
    if (!gesture)
        throw std::invalid_argument("Cannot add a null gesture");

    gestures.push_back(std::move(gesture));
}

/**
 * @brief Updates the gesture detector with new keypoint data.
 * @param keypoints The keypoint data to use for gesture detection.
 */
void GestureDetector::update(const std::vector<float>& keypoints)
{
    for (const auto& gesture : gestures)
    {
        gesture->update(keypoints);
    }
}

/**
 * @brief Gets the state of a specific gesture.
 * @param index The index of the gesture.
 * @return The state of the gesture.
 */

GestureState GestureDetector::getGestureState(GestureID id) const
{
    for (const auto& gesture : gestures)
    {
        if (gesture->getID() == id)
            return gesture->getState();
    }

    throw std::out_of_range("Gesture ID not registered");
}


/**
 * @brief Gets the number of gestures in the detector.
 * @return The number of gestures.
 *
 */
std::size_t GestureDetector::getGestureCount() const
{
    return gestures.size();
}

std::vector<GestureID> GestureDetector::getGestureIDs() const
{
    std::vector<GestureID> ids;
    ids.reserve(gestures.size());

    for (const auto& gesture : gestures)
    {
        ids.push_back(gesture->getID());
    }

    return ids;
}
