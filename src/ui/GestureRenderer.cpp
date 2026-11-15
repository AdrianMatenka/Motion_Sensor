#include "GestureRenderer.h"

#include <string>

namespace
{
    constexpr int PANEL_WIDTH = 180;
    constexpr int PANEL_PADDING = 8;
    constexpr int LINE_HEIGHT = 24;

    const cv::Scalar PANEL_COLOR(35, 35, 35);
    const cv::Scalar TEXT_COLOR(220, 220, 220);
    const cv::Scalar ACTIVE_COLOR(80, 220, 100);
    const cv::Scalar INACTIVE_COLOR(140, 140, 140);
}

void GestureRenderer::draw(cv::Mat& frame, const GestureDetector& detector)
{
    if (frame.empty() || !visible)
        return;

    drawPanel(frame);
    drawGestures(frame, detector);
}

void GestureRenderer::toggleVisibility()
{
    visible = !visible;
}

void GestureRenderer::drawPanel(cv::Mat& frame)
{
    const int width = std::min(PANEL_WIDTH, frame.cols);

    cv::rectangle(
        frame,
        cv::Point(0, 0),
        cv::Point(width, frame.rows),
        PANEL_COLOR,
        cv::FILLED
    );

    cv::putText(
        frame,
        "GESTURE MONITOR",
        cv::Point(PANEL_PADDING, 30),
        cv::FONT_HERSHEY_SIMPLEX,
        0.4,
        TEXT_COLOR,
        2
    );

    cv::line(
        frame,
        cv::Point(PANEL_PADDING, 45),
        cv::Point(width - PANEL_PADDING, 45),
        cv::Scalar(100, 100, 100),
        1
    );
}

void GestureRenderer::drawGestures(
    cv::Mat& frame,
    const GestureDetector& detector)
{
    const auto gestureIDs = detector.getGestureIDs();

    int y = 78;

    for (const GestureID id : gestureIDs)
    {
        const GestureState state = detector.getGestureState(id);

        const char* name = getGestureName(id);

        const bool active =
            state == GestureState::JUST_ACTIVATED ||
            state == GestureState::ACTIVE;

        const cv::Scalar color = active ? ACTIVE_COLOR : INACTIVE_COLOR;

        cv::putText(
            frame,
            name,
            cv::Point(PANEL_PADDING, y),
            cv::FONT_HERSHEY_SIMPLEX,
            0.42,
            TEXT_COLOR,
            1
        );

        cv::putText(
            frame,
            active ? "ACTIVE" : "INACTIVE",
            cv::Point(120, y),
            cv::FONT_HERSHEY_SIMPLEX,
            0.35,
            color,
            1
        );

        y += LINE_HEIGHT;
    }
}

const char* GestureRenderer::getGestureName(GestureID id)
{
    switch (id)
    {
        case GestureID::LEFT_HAND_RAISED:
            return "Left hand raised";

        case GestureID::RIGHT_HAND_RAISED:
            return "Right hand raised";
    }

    return "Unknown gesture";
}