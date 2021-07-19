#include "CarController.hpp"
#include "PrintingMovingStrategy.hpp"
#include "LineDetector.hpp"


double distance_to_dash_line(const cv::Mat & frame) {
    
    const auto line = LineDetector::DetectDashLine(frame);
    const double distance = LineDetector::GetDistanceToLine(line);
    
    return distance;
}


auto main() -> int {

    CarController controller;
    controller.SetMovingStrategy(std::make_unique<PrintingMovingStrategy>(std::cout));

    // get a frame somehow
    cv::VideoCapture cap(0);
    cv::Mat frame;
    while (cap.read(frame)) {
        const double distance = distance_to_dash_line(frame);
        controller.Move(distance);
    }

    return 0;
}
