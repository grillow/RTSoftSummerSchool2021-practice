#include "CarController.hpp"
#include "PrintingMovingStrategy.hpp"
#include "LineDetection.hpp"
#include <opencv2/core.hpp>
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
        cv::line(frame, left.first, left.second, {0, 255, 0}, 5);
        cv::line(frame, right.first, right.second, {0, 255, 0}, 5);
    }

    void DrawOffset(const int position) {
        const int w_2 = frame.size().width / 2;
        const int h = frame.size().height;

        cv::circle(frame, {w_2, h}, 15, {255, 255, 255}, 1);
        cv::circle(frame, {position, h}, 15, {255, 255, 0}, 1);
    }

    void Show() {
        cv::imshow("visualization", frame);
    }

    void Wait() {
        cv::waitKey();
    }

private:
    cv::Mat frame;
};


int calculate_offset(const cv::Mat & frame, Visualization & visualization) try {
    const auto lines = LineDetection::DetectRoadLines(frame);
    const auto best_lines = LineDetection::GetMainLines(lines, frame.size());

    const auto position = (best_lines.first.first.x + best_lines.second.first.x) / 2;
    const auto offset = position - frame.size().width / 2;

    visualization.DrawDetectedLines(lines);
    visualization.DrawBestLines(best_lines);
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
    // cv::VideoCapture cap(0);
    cv::Mat frame;
    frame = cv::imread(argv[1], cv::IMREAD_COLOR);
    //while (cap.read(frame)) {
        visualization.SetFrame(frame);
        const double distance = calculate_offset(frame, visualization);
        visualization.Show();
        controller.Move(distance);
    //}

    visualization.Wait();


    return 0;
}
