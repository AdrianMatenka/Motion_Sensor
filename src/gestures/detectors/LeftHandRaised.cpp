
#include "LeftHandRaised.h"

namespace
{
    constexpr int LEFT_SHOULDER = 5;
    constexpr int LEFT_ELBOW = 7;
    constexpr int LEFT_WRIST = 9;

    constexpr int VALUES_PER_KEYPOINT = 3;
}

void LeftHandRaised::update(
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

GestureState LeftHandRaised::getState() const
{
    return state;
}

bool LeftHandRaised::isHandRaised(
    const std::vector<float>& keypoints) const
{
    if (keypoints.size() != 17 * VALUES_PER_KEYPOINT)
        return false;

    const int shoulderIndex = LEFT_SHOULDER * VALUES_PER_KEYPOINT;
    const int elbowIndex = LEFT_ELBOW * VALUES_PER_KEYPOINT;
    const int wristIndex = LEFT_WRIST * VALUES_PER_KEYPOINT;

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
