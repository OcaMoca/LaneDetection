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
vector<float> linspace(float start_in, float end_in, int num_in);
Mat polyfit_windows(vector<Window> const&);
void poly_fit_x(vector<float> const&, vector<double>&, Mat const&);

void color_filter(const Mat& image, Mat& filtered_image);
void trapezoid_roi(const Mat&, Mat&, Mat&, float trap_bottom_width, float trap_top_width , float trap_height, float car_hood);
void perspective_transform(const Mat& color_filtered_image, vector<Point2f>&, vector<Point2f>&, Mat&);
void get_histogram(Mat const& binary_warped, Mat& histogram);
void calculate_lane_histogram(const Mat& historam, Point& left_peak, Point& right_peak);
void sliding_window(Mat& binary_warped, Point& left_peak, Point& right_peak, Mat& output_image, vector<Window>& left_boxes, vector<Window>& right_boxes);
void get_inverse_points(vector<float>& plot_y, vector<float>& left_fit_x, vector<float>& right_fit_x, Mat& color_warp);
void inverse_perspective(const Mat& sliding_window_output, vector<Point2f>& src, vector<Point2f>& dst, Mat& Minv, Mat& output_image);
void final_perspective(const Mat& color_warp, const Mat& original_image, Mat& Minv, Mat& output_image);
Mat pipeline(Mat& original_frame);
