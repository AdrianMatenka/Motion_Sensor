/*
*/

#pragma once

#include "../Gesture.h"

class RightHandRaised : public Gesture
{
public:
    void update(
        const std::vector<float>& keypoints
    ) override;

    GestureState getState() const override;

    GestureID getID() const override;

private:
    GestureState state = GestureState::INACTIVE;

    float confidenceThreshold = 0.3f;
    float verticalMargin = 0.05f;

    bool isHandRaised(
        const std::vector<float>& keypoints
    ) const;
};
