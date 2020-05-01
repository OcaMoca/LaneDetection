
#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <vector>
#include <numeric>
#include <ostream>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

class Window
{
    private:

        int x_left, x_right;
        int y_top, y_bottom;
        int x_center;
        int width, height;
        int min_pix;
        Mat window;
        vector<Point> non_zero;
        vector<double> non_zero_x;
        vector<double> non_zero_y;

      

    public:

        Window() : x_left(0), x_right(0), y_top(0), y_bottom(0), x_center(0),
                   width(0), height(0), min_pix(0) {}

        Window(Mat& binary_warped, int x_center, int y_top,
		int width, int height, int min_pix);

        vector<double> get_non_zero_x(void) { return non_zero_x; }
        vector<double> get_non_zero_y(void) { return non_zero_y; }

       
        void get_indices(Mat&, Mat&) const;

        double trim_mean(vector<double>& arr, double proportion, int x_margin)
        {
            int trim = (int)(arr.size() * proportion);

            arr.erase(arr.begin(), arr.begin() + trim);
            arr.erase(arr.end() - trim, arr.end());

            double average = (double)(accumulate( arr.begin(), arr.end(), 0.0) + arr.size()*x_margin) / arr.size();

            return average;

        }

        int count_nonzero(void) const { return (int)non_zero.size(); }
        const Point get_bottom_left_point(void) const { return Point(x_left, y_bottom);}
        const Point get_top_right_point(void) const { return Point(x_right, y_top); }
        Window get_next_window(Mat& binary_warped);

};



#endif