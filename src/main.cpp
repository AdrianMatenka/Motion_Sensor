
#include <iostream>

#include "Application.h"

int main()
{
    std::cout << "Starting Motion Sensor..." << std::endl;

    try
    {
        Application app;
        return app.run();
    }
    catch (const Ort::Exception& e)
    {
        std::cerr << "ONNX Runtime error: "
                  << e.what() << std::endl;
        return 1;
    }
    catch (const cv::Exception& e)
    {
        std::cerr << "OpenCV error: "
                  << e.what() << std::endl;
        return 1;
    }
}
