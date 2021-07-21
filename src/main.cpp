#include "CarController.hpp"
#include "PrintingMovingStrategy.hpp"
#include "LineDetector.hpp"
#include <opencv2/imgcodecs.hpp>


double calculate_offset(const cv::Mat & frame) {
    
    const auto lines = LineDetector::DetectRoadLines(frame);
    const std::vector<double> distances = LineDetector::GetDistancesToLines(lines);

    ///TODO: calculate the offset
    const auto pmin = std::min(distances.begin(), distances.end()); // warning: this is bullshit
    const double offset = (pmin != distances.end()) ? *pmin : 1337;

    return offset;
}


auto main(int argc, char **argv) -> int {
    if (argc != 2) {
        return -1;
    }

    CarController controller;
    controller.SetMovingStrategy(std::make_unique<PrintingMovingStrategy>(std::cout));

    // get a frame somehow
    // cv::VideoCapture cap(0);
    cv::Mat frame;
    frame = cv::imread(argv[1], cv::IMREAD_COLOR);
    //while (cap.read(frame)) {
        const double distance = calculate_offset(frame);
        controller.Move(distance);
    //}

    return 0;
}
