
cmake_minimum_required(VERSION 3.20)

project(MotionSensor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(OpenCV REQUIRED opencv5)

set(ONNXRUNTIME_ROOT "C:/msys64/ucrt64")

add_executable(MotionSensor
    src/main.cpp
    src/camera/Camera.cpp
    src/pose/PoseEstimator.cpp
    src/pose/PoseRenderer.cpp
    src/app/Application.cpp
    src/tracking/PoseSmoother.cpp
    src/gestures/GestureDetector.cpp
    src/gestures/detectors/LeftHandRaised.cpp
)

target_include_directories(MotionSensor PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    ${ONNXRUNTIME_ROOT}/include/onnxruntime
    src/camera
    src/pose
    src/app
    src/tracking
    src/gestures
)

target_link_directories(MotionSensor PRIVATE
    ${OpenCV_LIBRARY_DIRS}
    ${ONNXRUNTIME_ROOT}/lib
)

target_link_libraries(MotionSensor PRIVATE
    ${OpenCV_LIBRARIES}
    onnxruntime
)
