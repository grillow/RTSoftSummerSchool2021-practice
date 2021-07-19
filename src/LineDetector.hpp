#pragma once
#include <vector>
#include <opencv2/opencv.hpp>


namespace LineDetector {
    
    std::vector<cv::Point> DetectDashLine(const cv::Mat & frame);
    double GetDistanceToLine(const std::vector<cv::Point> & line);

}
