/*
 *  EXAMPLE
 */
#include <opencv2/opencv.hpp>


auto main() -> int {

    cv::Mat output = cv::Mat::zeros( 120, 350, CV_8UC3 );

    putText(output,
            "Hello World :)",
            { 15, 70 },
            cv::FONT_HERSHEY_PLAIN,
            3,
            cv::Scalar(0,255,0),
            4);

    cv::imshow("Output", output);

    cv::waitKey(0);

    return 0;
}
