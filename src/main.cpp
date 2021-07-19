#include "CarController.hpp"
#include "PrintingMovingStrategy.hpp"
#include "LineDetector.hpp"


auto main() -> int {

    CarController controller;
    controller.SetMovingStrategy(std::make_unique<PrintingMovingStrategy>(std::cout));

    // get a frame somehow
    cv::Mat frame;
    
    // for this frame:
    const auto line = LineDetector::DetectDashLine(frame);
    const double distance = LineDetector::GetDistanceToLine(line);
    controller.Move(distance);

    return 0;
}
