#include "CarController.hpp"
#include "PrintingMovingStrategy.hpp"
#include "LineDetection.hpp"
#include <opencv2/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <string_view>

struct Visualization {
    Visualization() {
        cv::namedWindow("visualization");
    }

    virtual ~Visualization() = default;

    void SetFrame(const cv::Mat & frame_) {
        cv::addWeighted(frame_, 0.3, cv::Mat::zeros(frame_.size(), frame_.type()), 1, 0, frame);
    }

    cv::Mat GetFrame() const {
        return frame;
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

    virtual void DrawDetectedLines(const std::vector<LineDetection::line_t> lines) = 0;

    virtual void DrawMainLines(const std::pair<LineDetection::line_t, LineDetection::line_t> & lines) = 0;

    virtual void DrawCenterLine(const LineDetection::line_t & line) = 0;

    virtual void DrawOffset(const int position) = 0;

protected:
    cv::Mat frame;
};

struct DefaultVisualization : Visualization {

    void DrawDetectedLines(const std::vector<LineDetection::line_t> lines) override {
        for (const auto & line : lines) {
            cv::line(frame, line.first, line.second, {255, 255, 0}, 2);
        }
    }

    void DrawMainLines(const std::pair<LineDetection::line_t, LineDetection::line_t> & lines) override {
        const auto & left = lines.first;
        const auto & right = lines.second;
        cv::Mat mask = cv::Mat::zeros(frame.size(), frame.type());
        cv::line(mask, left.first, left.second, {0, 255, 0}, 7);
        cv::line(mask, right.first, right.second, {0, 255, 0}, 7);
        cv::addWeighted(frame, 1, mask, 0.5, 0, frame);
    }

    void DrawCenterLine(const LineDetection::line_t & line) override {
        cv::line(frame, line.first, line.second, {255, 255, 255});
    }

    void DrawOffset(const int position) override {
        // const int w_2 = frame.size().width / 2;
        const int h = frame.size().height;

        // const auto rw = 90;
        // const auto rh = 90;
        // const cv::Rect ok_rect = cv::Rect(w_2 - rw / 2, h - rh / 2, rw, rh);
        // cv::rectangle(frame, ok_rect, {255, 255, 255});
        cv::circle(frame, {position, h}, 12, {255, 0, 255}, 1);
    }

};


// cv::Point line_intersection(const LineDetection::line_t & a, const LineDetection::line_t & b) {
//     const cv::Point2f x = b.first - a.first;
//     const cv::Point2f d1 = a.second - a.first;
//     const cv::Point2f d2 = b.second - b.first;
//
//     const double cross = d1.x * d2.y - d1.y * d2.x;
//     const double t1 = (x.x * d2.y - x.y * d2.x) / cross;
//     if (abs(cross) < 1e-8)
//         throw std::runtime_error("could not calculate line intersection");
//
//     return cv::Point2f(a.first) + d1 * t1;
// }


double calculate_offset(const cv::Mat & frame, Visualization & visualization) try {
    const auto lines = LineDetection::DetectRoadLines(frame);
    visualization.DrawDetectedLines(lines);

    const auto main_lines = LineDetection::GetMainLines(lines, frame.size());
    visualization.DrawMainLines(main_lines);

    const auto left_line = main_lines.first;
    const auto right_line = main_lines.second;

    const LineDetection::line_t center_line = {
         { (left_line.first.x + right_line.first.x) / 2, (left_line.first.y + right_line.first.y) / 2 },
         { (left_line.second.x + right_line.second.x) / 2, (left_line.second.y + right_line.second.y) / 2}
    };
    visualization.DrawCenterLine(center_line);

    const int road_center = center_line.first.x;
    const auto offset_abs= road_center - frame.size().width / 2;
    const double offset = offset_abs / (double)frame.size().width;
    visualization.DrawOffset(road_center);

    return offset;
} catch (const std::exception & e) {
    std::cout << e.what() << std::endl;
    return 0;
}


auto main(int argc, char **argv) -> int {
    if (argc != 2) {
        return -1;
    }

    DefaultVisualization visualization;
    CarController controller;
    controller.SetMovingStrategy(std::make_unique<PrintingMovingStrategy>(std::cout));

    // get a frame somehow
    cv::VideoCapture cap(argv[1]);

    cv::Mat frame;
    while (cap.read(frame)) {
        cv::resize(frame, frame, cv::Size(1280, 720), 0, 0);
        visualization.SetFrame(frame);
        const double distance = calculate_offset(frame, visualization);
        controller.Move(distance);
        visualization.Show();
        visualization.Wait(30);
    }

    // visualization.Wait();

    return 0;
}
