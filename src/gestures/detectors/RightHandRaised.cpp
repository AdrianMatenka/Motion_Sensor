
#include "RightHandRaised.h"

namespace
{
    constexpr int RIGHT_SHOULDER = 6;
    constexpr int RIGHT_ELBOW = 8;
    constexpr int RIGHT_WRIST = 10;

    constexpr int VALUES_PER_KEYPOINT = 3;
}

void RightHandRaised::update(
    const std::vector<float>& keypoints)
{
    const bool raised = isHandRaised(keypoints);

    switch (state)
    {
        case GestureState::INACTIVE:
            if (raised)
                state = GestureState::JUST_ACTIVATED;
            break;

        case GestureState::JUST_ACTIVATED:
            state = raised
                ? GestureState::ACTIVE
                : GestureState::JUST_DEACTIVATED;
            break;

        case GestureState::ACTIVE:
            if (!raised)
                state = GestureState::JUST_DEACTIVATED;
            break;

        case GestureState::JUST_DEACTIVATED:
            state = raised
                ? GestureState::JUST_ACTIVATED
                : GestureState::INACTIVE;
            break;
    }
}

GestureState RightHandRaised::getState() const
{
    return state;
}

GestureID RightHandRaised::getID() const
{
    return GestureID::RIGHT_HAND_RAISED;
}

bool RightHandRaised::isHandRaised(
    const std::vector<float>& keypoints) const
{
    if (keypoints.size() != 17 * VALUES_PER_KEYPOINT)
        return false;

    const int shoulderIndex = RIGHT_SHOULDER * VALUES_PER_KEYPOINT;
    const int elbowIndex = RIGHT_ELBOW * VALUES_PER_KEYPOINT;
    const int wristIndex = RIGHT_WRIST * VALUES_PER_KEYPOINT;

    const float shoulderY = keypoints[shoulderIndex];
    const float shoulderConfidence = keypoints[shoulderIndex + 2];

    const float elbowY = keypoints[elbowIndex];
    const float elbowConfidence = keypoints[elbowIndex + 2];

    const float wristY = keypoints[wristIndex];
    const float wristConfidence = keypoints[wristIndex + 2];

    if (shoulderConfidence < confidenceThreshold ||
        elbowConfidence < confidenceThreshold ||
        wristConfidence < confidenceThreshold)
    {
        return false;
    }

    return elbowY < shoulderY - verticalMargin &&
           wristY < shoulderY - verticalMargin;
}
