#include <thread>
#include <iostream>
#include <algorithm> 
#include <numeric>
#include <opencv2/videoio.hpp>  // Video write
#include "window.hpp"


void polyfit(const Mat&, const Mat&, Mat&, int);
vector<double> linespace(double, double, int);
Mat polyfit_windows(vector<Window> const&);
void poly_fit_x(vector<double> const&, vector<double>&, Mat const&);

Mat color_filter(Mat);
Mat perspective_transform(Mat);
Mat get_histogram(Mat);
Mat sliding_window(Mat, Mat&, Mat&);
Mat inverse_perspective(Mat, Mat);

