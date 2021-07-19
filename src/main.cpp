#include "CarController.hpp"
#include "PrintingMovingStrategy.hpp"
#include "LineDetector.hpp"


double calculate_offset(const cv::Mat & frame) {
    
    const auto lines = LineDetector::DetectDashLines(frame);
    const std::vector<double> distances = LineDetector::GetDistancesToLines(lines);
    
    ///TODO: calculate the offset
    const double offset = distances[0];

    return offset;
}


auto main() -> int {

    CarController controller;
    controller.SetMovingStrategy(std::make_unique<PrintingMovingStrategy>(std::cout));

    // get a frame somehow
    cv::VideoCapture cap(0);
    cv::Mat frame;
    while (cap.read(frame)) {
        const double distance = calculate_offset(frame);
        controller.Move(distance);
    }

    return 0;
}
