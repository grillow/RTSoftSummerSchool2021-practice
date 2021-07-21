#pragma once
#include <vector>
#include <utility>
#include <opencv2/opencv.hpp>


namespace LineDetector {
    
    std::vector<std::pair<cv::Point, cv::Point>> DetectRoadLines(const cv::Mat & frame);
    std::vector<double> GetDistancesToLines(
            const std::vector<std::pair<cv::Point, cv::Point>> & lines);
    double GetDistanceToLine(const std::pair<cv::Point, cv::Point> & line);

}
