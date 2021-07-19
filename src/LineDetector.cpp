#include "LineDetector.hpp"
#include <algorithm>


namespace LineDetector {
    
    ///TODO: do
    std::vector<std::vector<cv::Point>> DetectDashLines(
            const cv::Mat & frame) {
        
        (void)frame;
        // return { {{0, 0}, {1, 1}}, {{2, 2}} };
        return {};
    }

    std::vector<double> GetDistancesToLines(
            const std::vector<std::vector<cv::Point>> & lines) {
       
        std::vector<double> distances;
        distances.reserve(lines.size());

        std::transform(lines.begin(), lines.end(), std::back_inserter(distances),
                [](const auto & line) { return GetDistanceToLine(line); });

        return distances;
    }

    ///TODO: do
    double GetDistanceToLine(const std::vector<cv::Point> & line) {
        (void)line;
        return 0;
    }

}
