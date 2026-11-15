#pragma once

#include "GestureDetector.h"
#include <opencv2/opencv.hpp>

class GestureRenderer
{
public:
    GestureRenderer() = default;

    void draw(cv::Mat& frame, const GestureDetector& detector);

    void toggleVisibility();

private:
    void drawPanel(cv::Mat& frame);
    void drawGestures(cv::Mat& frame, const GestureDetector& detector);

    static const char* getGestureName(GestureID id);

    bool visible = true;
};