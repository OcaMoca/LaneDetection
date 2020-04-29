
#include "functions.hpp"

int main(int, char**) {
    cout << "OpenCV project!\n";

    Mat filtered_image;
    Mat result;
    
    const char* file = "C:\\Users\\Olivera\\Desktop\\diplomski\\LaneDetection\\test_data\\test_images\\curved.jpg";
    Mat image = cv::imread(file); 
    vector<double> ploty, left_fit_x, right_fit_x;

    filtered_image = perspective_transform(image);
    result = inverse_perspective(filtered_image, image);
    imshow("image", result);
    waitKey();

}
