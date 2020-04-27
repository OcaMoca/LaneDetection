
#include "functions.hpp"

int main(int, char**) {
    cout << "OpenCV project!\n";

    Mat filtered_image;
    Mat res;
    
    auto file = "C:\\Users\\Olivera\\Desktop\\diplomski\\LaneDetection\\test_data\\test_images\\solidYellowCurve.jpg";
    auto image = cv::imread(file); // tip je cv::Mat

    filtered_image = perspective_transform(image);
    res = sliding_window(filtered_image);
    imshow("image", res);
    waitKey();

}
