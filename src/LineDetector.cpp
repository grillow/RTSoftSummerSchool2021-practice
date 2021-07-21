#include "LineDetector.hpp"
#include <algorithm>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


namespace LineDetector {

    std::vector<std::vector<cv::Point>> DetectRoadLines(
            const cv::Mat & frame) {

        cv::Mat gray;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        cv::Mat blurred;
        cv::GaussianBlur(gray, blurred, {5, 5}, 0);

        cv::Mat edged;
        cv::Canny(blurred, edged, 100, 180);

        const auto size = edged.size();
        cv::Mat mask = cv::Mat::zeros(size, edged.type());

        const std::vector<std::vector<cv::Point>> vertices = {{
            cv::Point { 0, size.height },
            cv::Point { int(size.width * 0.45), int(size.height * 0.60) },
            cv::Point { int(size.width * 0.55), int(size.height * 0.60) },
            cv::Point { size.width, size.height }
        }};
        cv::fillPoly(mask, vertices, cv::Scalar {255, 255, 255});

        cv::Mat masked_edges;
        cv::bitwise_and(edged, mask, masked_edges);


        cv::imshow("frame", frame);
        cv::imshow("gray", gray);
        cv::imshow("blurred", blurred);
        cv::imshow("edged", edged);
        cv::imshow("mask", mask);
        cv::imshow("masked_edges", masked_edges);
        cv::waitKey();
        return {}; ///TODO: temp

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
