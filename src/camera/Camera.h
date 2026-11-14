/*
* @file camera.h
* @brief Camera class for capturing video frames
* This file defines the Camera class, which is responsible for capturing video frames from a camera device.
*/

#pragma once

#include <opencv2/opencv.hpp>

enum class CameraError
{
    CAMERA_OK = 0,
    CAMERA_ALREADY_OPEN,
    CAMERA_NOT_OPEN,
    CAMERA_OPEN_FAILED,
    CAMERA_READ_FAILED,
    CAMERA_EMPTY_FRAME
};

/*
* @class Camera
* @brief A class for capturing video frames from a camera device.
* This class provides methods to open the camera, read frames, and release the camera resources.
*/
class Camera
{
public:
    explicit Camera(int cameraIndex = 0);
    ~Camera();

    CameraError open();
    CameraError read(cv::Mat& frame);
    
    void release();
    bool isOpened() const;

private:
    int cameraIndex;
    cv::VideoCapture capture;
};
