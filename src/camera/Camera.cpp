/*
* @file camera.cpp
* @brief Implementation of the Camera class for capturing video frames
* This file contains the implementation of the Camera class, which is responsible for capturing video frames from a camera device.
*/

#include "Camera.h"

/**
 * @brief Constructs a Camera object.
 * @param cameraIndex The index of the camera to use.
 */
Camera::Camera(int cameraIndex)
    : cameraIndex(cameraIndex)
{
}

/**
 * @brief Destructs a Camera object.
 */
Camera::~Camera()
{
    release();
}

/**
 * @brief Opens the camera.
 * @return A CameraError indicating the result of the operation.
 */
CameraError Camera::open()
{
    if (capture.isOpened())
        return CameraError::CAMERA_ALREADY_OPEN;

    if (!capture.open(cameraIndex))
        return CameraError::CAMERA_OPEN_FAILED;

    return CameraError::CAMERA_OK;
}

/**
 * @brief Checks if the camera is opened.
 * @return True if the camera is opened, false otherwise.
 */
bool Camera::isOpened() const
{
    return capture.isOpened();
}

/**
 * @brief Reads a frame from the camera.
 * @param frame The frame to read.
 * @return A CameraError indicating the result of the operation.
 */
CameraError Camera::read(cv::Mat& frame)
{
    if (!capture.isOpened())
        return CameraError::CAMERA_NOT_OPEN;

    if (!capture.read(frame))
        return CameraError::CAMERA_READ_FAILED;

    if (frame.empty())
        return CameraError::CAMERA_EMPTY_FRAME;

    return CameraError::CAMERA_OK;
}

/**
 * @brief Releases the camera resources.
 */
void Camera::release()
{
    if (capture.isOpened())
        capture.release();
}