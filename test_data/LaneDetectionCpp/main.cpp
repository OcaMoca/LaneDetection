
#include "functions.hpp"

int main(int, char**) {
    cout << "OpenCV project!\n";

    Mat filtered_image;
    Mat res;
    
    const char* file = "C:\\Users\\Olivera\\Desktop\\diplomski\\LaneDetection\\test_data\\test_images\\solidWhiteRight.jpg";
    Mat image = cv::imread(file); 
    vector<double> ploty, left_fit_x, right_fit_x;

    filtered_image = perspective_transform(image);
    res = sliding_window(filtered_image, ploty, left_fit_x, right_fit_x);
    imshow("image", res);
    waitKey();

}
