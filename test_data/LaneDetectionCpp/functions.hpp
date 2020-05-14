#include <thread>
#include <filesystem>
#include <iostream>
#include <algorithm> 

#include <functional> 
#include <opencv2/videoio.hpp>  // Video write
#include "window.hpp"

class LaneDetection
{
    private:

        VideoCapture capture;
        VideoWriter video_output;
        Mat frame;

        float trap_bottom_width; 
        float trap_top_width; 
        float trap_height; 
        float car_hood;

        vector<Point2f> src;
        vector<Point2f> dst;

        void polyfit(const Mat&, const Mat&, Mat&, int);
        vector<float> linspace(float, float, int);
        Mat polyfit_windows(vector<Window> const&);
        void poly_fit_x(vector<float> const&, vector<float>&, Mat const&);

        void trapezoid_roi();
        void color_filter(Mat&);
        void perspective_transform(const Mat&, Mat&);
        void get_histogram(Mat const&, Mat&);
        void calculate_lane_histogram(const Mat&, Point&, Point&);
        void sliding_window(Mat&, Point&, Point&, Mat&, vector<Window>&, vector<Window>&);
        void get_inverse_points(vector<float>&, vector<float>&, vector<float>&, Mat&);
        void inverse_perspective(const Mat&, Mat&, Mat&);
        void final_perspective(const Mat&, const Mat&, Mat&, Mat&);
        
    public:

        LaneDetection();
    
        void init(string file_name, string output_file);
        bool frame_processing();
        void release();

};

