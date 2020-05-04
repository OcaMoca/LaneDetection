#include <thread>
#include <chrono> 
#include <filesystem>
#include <iostream>
#include <algorithm> 

#include <functional> 
#include <opencv2/videoio.hpp>  // Video write
#include "window.hpp"

static float T_BOTTOM_WIDTH = 0.7f; 
static float T_TOP_WIDTH = 0.1f; 
static float T_HEIGHT = 0.38f; 
static float T_CAR_HOOD = 50;

void polyfit(const Mat&, const Mat&, Mat&, int);
vector<float> linespace(float start_in, float end_in, int num_in);
Mat polyfit_windows(vector<Window> const&);
void poly_fit_x(vector<float> const&, vector<double>&, Mat const&);

void trapezoid_roi(const Mat&, Mat&, Mat&, float trap_bottom_width, float trap_top_width , float trap_height, float car_hood);
void color_filter(const Mat& image, Mat& filtered_image);
void perspective_transform(const Mat& image, Mat& binary_warped);
void get_histogram(Mat const& binary_warped, Mat& histogram);
void calculate_lane_histogram(const Mat& historam, Point& left_peak, Point& right_peak);
void sliding_window(Mat& binary_warped, vector<float>& left_fit_x, vector<float>& right_fit_x, vector<float>& ploty, Mat& output_image);
Mat inverse_perspective(Mat&, Mat&);

