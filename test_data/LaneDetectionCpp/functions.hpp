#include <iostream>
#include <algorithm> 
#include <opencv2/opencv.hpp>
#include <thread>
#include <vector>
#include <ostream>
#include <opencv2/videoio.hpp>  // Video write
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

Mat color_filter(Mat);
Mat perspective_transform(Mat);
Mat get_histogram(Mat);
Mat sliding_window(Mat);

