
cmake_minimum_required(VERSION 3.20)

project(MotionSensor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(OpenCV REQUIRED opencv5)

set(ONNXRUNTIME_ROOT "C:/msys64/ucrt64")

include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
)

FetchContent_MakeAvailable(nlohmann_json)

configure_file(
    "${CMAKE_SOURCE_DIR}/config/config.json"
    "${CMAKE_BINARY_DIR}/config/config.json"
    COPYONLY
)

add_executable(MotionSensor
    src/main.cpp
    src/camera/Camera.cpp
    src/pose/PoseEstimator.cpp
    src/pose/PoseRenderer.cpp
    src/app/Application.cpp
    src/tracking/PoseSmoother.cpp
    src/gestures/GestureDetector.cpp
    src/gestures/detectors/LeftHandRaised.cpp
    src/gestures/detectors/RightHandRaised.cpp
    src/controller/InputController.cpp
    src/controller/GameController.cpp
    src/ui/GestureRenderer.cpp
)

target_include_directories(MotionSensor PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    ${ONNXRUNTIME_ROOT}/include/onnxruntime
    src/camera
    src/pose
    src/app
    src/tracking
    src/gestures
    src/controller
    src/ui
)

target_link_directories(MotionSensor PRIVATE
    ${OpenCV_LIBRARY_DIRS}
    ${ONNXRUNTIME_ROOT}/lib
)

target_link_libraries(MotionSensor PRIVATE
    ${OpenCV_LIBRARIES}
    onnxruntime
    nlohmann_json::nlohmann_json
)
