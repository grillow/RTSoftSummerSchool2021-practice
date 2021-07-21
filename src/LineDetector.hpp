#pragma once
#include <vector>
#include <opencv2/opencv.hpp>


namespace LineDetector {
    
    std::vector<std::vector<cv::Point>> DetectRoadLines(const cv::Mat & frame);
    std::vector<double> GetDistancesToLines(
            const std::vector<std::vector<cv::Point>> & lines);
    double GetDistanceToLine(const std::vector<cv::Point> & line);

}
