#include <thread>
#include <iostream>
#include <algorithm> 
#include <opencv2/videoio.hpp>  // Video write
#include "window.hpp"

Mat color_filter(Mat);
Mat perspective_transform(Mat);
Mat get_histogram(Mat);
Mat sliding_window(Mat);

