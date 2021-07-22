#pragma once
#include <vector>
#include <utility>
#include <opencv2/opencv.hpp>


namespace LineDetection {

    using line_t = std::pair<cv::Point, cv::Point>;

    std::vector<line_t> DetectRoadLines(const cv::Mat & frame);

    std::pair<line_t, line_t> GetMainLines
            (const std::vector<line_t> & lines, cv::Size size);

}
