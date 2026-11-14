
/*
 * @file Application.h
 * @brief Main application controller.
 */

#pragma once

#include "Camera.h"
#include "PoseEstimator.h"
#include "PoseRenderer.h"
#include "PoseSmoother.h"
#include "GestureDetector.h"
#include "detectors/LeftHandRaised.h"

/**
 * @class Application
 * @brief The main application class that orchestrates the camera, pose estimation, and rendering.
 */
class Application
{
public:
    Application();
    ~Application();

    int run();

private:
    bool initialize();
    void shutdown();

    Camera camera;
    PoseEstimator poseEstimator;
    PoseSmoother poseSmoother;
    PoseRenderer poseRenderer;
    GestureDetector gestureDetector;

    bool initialized = false;
};
