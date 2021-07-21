#include "LineDetector.hpp"
#include <algorithm>
#include <iterator>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


namespace LineDetector {

    std::vector<std::pair<cv::Point, cv::Point>> DetectRoadLines(
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

        std::vector<cv::Vec4i> linesP;
        cv::HoughLinesP(masked_edges, linesP, 20, CV_PI / 180, 15, 135, 50);
        std::vector<std::pair<cv::Point, cv::Point>> lines;
        std::transform(linesP.begin(), linesP.end(), std::back_inserter(lines),
            [](const auto & line) {
                return std::pair<cv::Point, cv::Point>
                { {line[0], line[1]}, {line[2], line[3]} };
            }
        );

        /*
        cv::Mat lines_edges(size, CV_8UC3);
        for (const auto & line : lines) {
            std::cout   << line.first.x  << " " << line.first.y
                        << " "
                        << line.second.x << " " << line.second.y
                        << std::endl;

            cv::line(lines_edges, line.first, line.second, { 0, 255, 0 });
        }
        std::cout << lines.size() << std::endl;

        cv::imshow("frame", frame);
        cv::imshow("gray", gray);
        cv::imshow("blurred", blurred);
        cv::imshow("edged", edged);
        cv::imshow("mask", mask);
        cv::imshow("masked_edges", masked_edges);
        cv::imshow("lines_edges", lines_edges);
        cv::waitKey(0);
        */

        return lines;
    }

    std::vector<double> GetDistancesToLines(
            const std::vector<std::pair<cv::Point, cv::Point>> & lines) {

        std::vector<double> distances;
        distances.reserve(lines.size());

        std::transform(lines.begin(), lines.end(), std::back_inserter(distances),
                [](const auto & line) { return GetDistanceToLine(line); });

        return distances;
    }

    ///TODO: do
    double GetDistanceToLine(const std::pair<cv::Point, cv::Point> & line) {
        (void)line;
        return 0;
    }

}
