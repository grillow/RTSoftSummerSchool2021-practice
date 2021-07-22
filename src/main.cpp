#include "CarController.hpp"
#include "PrintingMovingStrategy.hpp"
#include "LineDetection.hpp"
#include <opencv2/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>


struct Visualization {
    Visualization() {
        cv::namedWindow("visualization");
    }

    void SetFrame(const cv::Mat & frame_) {
        cv::addWeighted(frame_, 0.3, cv::Mat::zeros(frame_.size(), frame_.type()), 1, 0, frame);
    }

    void DrawDetectedLines(const std::vector<LineDetection::line_t> lines) {
        for (const auto & line : lines) {
            cv::line(frame, line.first, line.second, {255, 255, 0});
        }
    }
    void DrawBestLines(const std::pair<LineDetection::line_t, LineDetection::line_t> & lines) {
        const auto & left = lines.first;
        const auto & right = lines.second;
        cv::line(frame, left.first, left.second, {0, 255, 0}, 3);
        cv::line(frame, right.first, right.second, {0, 255, 0}, 3);
    }

    void DrawOffset(const int position) {
        const int w_2 = frame.size().width / 2;
        const int h = frame.size().height;

        cv::circle(frame, {w_2, h}, 15, {255, 255, 255}, 1);
        cv::circle(frame, {position, h}, 12, {255, 0, 255}, 1);
    }

    void Show() {
        cv::imshow("visualization", frame);
    }

    void Wait() {
        cv::waitKey();
    }

    void Wait(int delay) {
        cv::waitKey(delay);
    }

private:
    cv::Mat frame;
};


cv::Point line_intersection(const LineDetection::line_t & a, const LineDetection::line_t & b) {
    const cv::Point2f x = b.first - a.first;
    const cv::Point2f d1 = a.second - a.first;
    const cv::Point2f d2 = b.second - b.first;

    const double cross = d1.x * d2.y - d1.y * d2.x;
    const double t1 = (x.x * d2.y - x.y * d2.x) / cross;
    if (abs(cross) < 1e-8)
        throw std::runtime_error("could not calculate line intersection");

    return cv::Point2f(a.first) + d1 * t1;
}


int calculate_offset(const cv::Mat & frame, Visualization & visualization) try {
    const auto lines = LineDetection::DetectRoadLines(frame);
    const auto main_lines = LineDetection::GetMainLines(lines, frame.size());

    const auto position = line_intersection(main_lines.first, main_lines.second).x;
    const auto offset = position - frame.size().width / 2;

    visualization.DrawDetectedLines(lines);
    visualization.DrawBestLines(main_lines);
    visualization.DrawOffset(position);

    return offset;
} catch (const std::exception & e) {
    std::cout << e.what() << std::endl;
    return 0;
}


auto main(int argc, char **argv) -> int {
    if (argc != 2) {
        return -1;
    }

    Visualization visualization;
    CarController controller;
    controller.SetMovingStrategy(std::make_unique<PrintingMovingStrategy>(std::cout));

    // get a frame somehow
    cv::VideoCapture cap(argv[1]);
    cv::Mat frame;
    // frame = cv::imread(argv[1], cv::IMREAD_COLOR);
    while (cap.read(frame)) {
        visualization.SetFrame(frame);
        const double distance = calculate_offset(frame, visualization);
        controller.Move(distance);
        visualization.Show();
        visualization.Wait();
    }

    visualization.Wait();

    return 0;
}
