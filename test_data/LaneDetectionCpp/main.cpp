
#include "functions.hpp"


int main(int, char**) {
    std::cout << "OpenCV project!\n";

    cv::Mat filtered_image;
    
    auto file = "C:\\Users\\Olivera\\Desktop\\diplomski\\LaneDetection\\test_data\\test_images\\solidWhiteCurve.jpg";
    auto image = cv::imread(file); // tip je cv::Mat

    filtered_image = perspective_transform(image);
    cv::imshow("image", filtered_image);
    cv::waitKey();

}
