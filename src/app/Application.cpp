
/*
 * @file Application.cpp
 * @brief Implementation of the Application class.
 */

#include "Application.h"

#include <iostream>

/*
* @brief Constructs an Application object.
 */
Application::Application()
    : camera(0),
      poseEstimator(L"models\\pose_model.onnx"),
      poseRenderer(0.3f),
      gameController(inputController)
{
    gestureDetector.addGesture(
        std::make_unique<LeftHandRaised>()
    );

    gestureDetector.addGesture(
        std::make_unique<RightHandRaised>()
    );

    // Gesture mappings
    gameController.addMapping({
        GestureID::LEFT_HAND_RAISED,
        VK_SPACE,
        ActionMode::ON_ACTIVATION
    });

    gameController.addMapping({
        GestureID::RIGHT_HAND_RAISED,
        VK_SHIFT,
        ActionMode::WHILE_ACTIVE
    });
}

/*
* @brief Destructs an Application object.
 */
Application::~Application()
{
    shutdown();
}

/**
 * @brief Initializes the application.
 * @return true if initialization is successful, false otherwise.
 */
bool Application::initialize()
{
    CameraError cameraError = camera.open();

    if (cameraError != CameraError::CAMERA_OK)
    {
        std::cerr << "Camera initialization failed. Error: "
                  << static_cast<int>(cameraError) << std::endl;
        return false;
    }

    initialized = true;
    return true;
}

/**
 * @brief Runs the application.
 * @return 0 if successful, 1 otherwise.
 */
int Application::run()
{
    if (!initialize())
        return 1;

    cv::Mat frame;
    std::vector<float> keypoints;

    while (true)
    {
        CameraError cameraError = camera.read(frame);

        if (cameraError != CameraError::CAMERA_OK)
        {
            std::cerr << "Camera read failed. Error: "
                      << static_cast<int>(cameraError) << std::endl;
            break;
        }

        PoseEstimatorError poseError =
            poseEstimator.estimate(frame, keypoints);

        if (poseError != PoseEstimatorError::POSE_OK)
        {
            std::cerr << "Pose estimation failed. Error: "
                      << static_cast<int>(poseError) << std::endl;
            continue;
        }

        poseSmoother.smooth(keypoints);

        gestureDetector.update(keypoints);

        gameController.update(gestureDetector);

        poseRenderer.draw(frame, keypoints);

        cv::imshow("Motion Sensor", frame);

        if (cv::waitKey(1) == 27)
            break;
    }

    shutdown();

    return 0;
}

/**
 * @brief Shuts down the application.
 */
void Application::shutdown()
{
    gameController.reset();

    if (initialized)
    {
        camera.release();
        initialized = false;
    }

    cv::destroyAllWindows();
}
