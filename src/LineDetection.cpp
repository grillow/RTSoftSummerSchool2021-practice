#include "LineDetection.hpp"
#include <algorithm>
#include <iterator>
#include <opencv2/core/matx.hpp>
#include <stdexcept>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


namespace LineDetection {

    namespace {
        line_t ContinueLine(const line_t & line, cv::Size size) {
            cv::Point down, upper;
            if (line.first.y > line.second.y) {
                down = line.first;
                upper = line.second;
            } else {
                down = line.second;
                upper = line.first;
            }
            cv::Vec2d dir = cv::Vec2i(down) - cv::Vec2i(upper);
            const double t = (size.height - upper.y) / dir[1];
            down = cv::Vec2i(cv::Vec2d(cv::Vec2i(upper)) + cv::Vec2d(t * dir));

            return {down, upper};
        }
    }

    std::vector<line_t> DetectRoadLines(
            const cv::Mat & frame) {

        cv::Mat gray;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        cv::Mat blurred;
        cv::GaussianBlur(gray, blurred, {3, 3}, 0);

        cv::Mat edged;
        cv::Canny(blurred, edged, 100, 180);

        const auto size = edged.size();
        cv::Mat mask = cv::Mat::zeros(size, edged.type());

        const std::vector<std::vector<cv::Point>> vertices = {{
            cv::Point { int(size.width * 0.25), int(size.height * 1.00) },
            cv::Point { int(size.width * 0.42), int(size.height * 0.75) },
            cv::Point { int(size.width * 0.58), int(size.height * 0.75) },
            cv::Point { int(size.width * 0.75), int(size.height * 1.00) }
        }};
        cv::fillPoly(mask, vertices, cv::Scalar {255, 255, 255});

        cv::Mat masked_edges;
        cv::bitwise_and(edged, mask, masked_edges);

        std::vector<cv::Vec4i> linesP;
        // more lines variant
        cv::HoughLinesP(masked_edges, linesP, 20, CV_PI / 180, 15);
        // cv::HoughLinesP(masked_edges, linesP, 20, CV_PI / 180, 15, 135, 50);
        std::vector<line_t> lines;
        std::transform(linesP.begin(), linesP.end(), std::back_inserter(lines),
            [](const auto & line) {
                return line_t
                { {line[0], line[1]}, {line[2], line[3]} };
            }
        );

#ifdef DEBUG
        // cv::Mat lines_edges(size, CV_8UC3);
        // for (const auto & line : lines) {
        //     std::cout   << line.first.x  << " " << line.first.y
        //                 << " "
        //                 << line.second.x << " " << line.second.y
        //                 << std::endl;
        //
        //     cv::line(lines_edges, line.first, line.second, { 0, 255, 0 });
        // }
        // std::cout << lines.size() << std::endl;
        //
        // cv::imshow("frame", frame);
        // cv::imshow("gray", gray);
        // cv::imshow("blurred", blurred);
        // cv::imshow("edged", edged);
        // cv::imshow("mask", mask);
        // cv::imshow("masked_edges", masked_edges);
        // cv::imshow("lines_edges", lines_edges);
        // cv::waitKey(0);
#endif

        return lines;
    }

    std::pair<line_t, line_t> GetMainLines
            (const std::vector<line_t> & lines, cv::Size size) {

        const int w = size.width / 2;

        cv::Point   lu, ld, ru, rd;
        bool        flu, fld, fru, frd;
        flu = fld = fru = frd = false;

        for (const auto & line : lines) {
            const cv::Point a = line.first;
            const cv::Point b = line.second;
            if (a.x < w && b.x < w) {
                if (!flu || lu.y < a.y) {
                    lu = a;
                    flu = true;
                }
                if (!flu || lu.y < b.y) {
                    lu = b;
                    flu = true;
                }
                if (!fld || ld.y > a.y) {
                    ld = a;
                    fld = true;
                }
                if (!fld || ld.y > b.y) {
                    ld = b;
                    fld = true;
                }
            } else if (a.x > w && b.x > w) {
                if (!fru || ru.y < a.y) {
                    ru = a;
                    fru = true;
                }
                if (!fru || ru.y < b.y) {
                    ru = b;
                    fru = true;
                }
                if (!frd || rd.y > a.y) {
                    rd = a;
                    frd = true;
                }
                if (!frd || rd.y > b.y) {
                    rd = b;
                    frd = true;
                }
            } else {
                continue;
            }

        }

        if (!flu || !fld || !fru || !frd) {
            throw std::runtime_error("could not get suitable lines");
        }
        return {ContinueLine({ld, lu}, size), ContinueLine({rd, ru}, size)};
    }

}
