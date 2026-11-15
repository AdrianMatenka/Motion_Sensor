/* 
 * @file Gesture.h
 * @brief Header file for the Gesture class.
 * This file contains the definition of the Gesture class, which serves as a base class for implementing gesture recognition based on keypoint data.
 */

#pragma once

#include <vector>

enum class GestureID
{
    LEFT_HAND_RAISED,
    RIGHT_HAND_RAISED
};

/**
 * @enum GestureState
 * @brief Represents the state of a gesture.
 */
enum class GestureState
{
    INACTIVE,
    JUST_ACTIVATED,
    ACTIVE,
    JUST_DEACTIVATED
};

/**
 * @class Gesture
 * @brief Base class for implementing gesture recognition based on keypoint data.
 */
class Gesture
{
public:
    virtual ~Gesture() = default;

    virtual GestureID getID() const = 0;

    virtual void update(
        const std::vector<float>& keypoints
    ) = 0;

    virtual GestureState getState() const = 0;
};
