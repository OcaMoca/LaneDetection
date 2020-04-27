#include "functions.hpp"

Mat color_filter(Mat image)
{
    Mat mask;
    Mat mask1;
    Mat mask2;
    Mat image_hsv;
    Mat white_image;
    Mat yellow_image;
    Mat filtered_image;

    inRange(image, Scalar(200,200,200), Scalar(255,255,255), mask1);

    cvtColor(image, image_hsv, COLOR_BGR2HSV);
    inRange(image_hsv, Scalar(20,70,100), Scalar(100,255,255), mask2);

    bitwise_and(image, image, white_image, mask1);
    
    bitwise_and(image, image, yellow_image, mask2);
    
    addWeighted(white_image, 1., yellow_image, 1., 0., filtered_image);

    return filtered_image;
}


Mat perspective_transform(Mat image)
{
    Mat color_filtered_image;
    Mat gray;
    Mat binary_threshold;
    Mat M(2,4,CV_32FC2);
    Mat Minv(2,4,CV_32FC2);   
    Mat binary_warped;
    vector<Point2f> src;
    vector<Point2f> dst;

    int width = image.size().width;
    int height = image.size().height;

    double trap_bottom_width = 0.7;
    double trap_top_width    = 0.1;
    double trap_height       = 0.38;

    src.push_back(Point2d( (width * (1 - trap_bottom_width)) / 2, (double)(height - 50)));
    src.push_back(Point2d( (width * (1 - trap_top_width)) / 2, height - height * trap_height));
    src.push_back(Point2d( width - (width * (1 - trap_top_width)) / 2, height - height * trap_height));
    src.push_back(Point2d( width - (width * (1 - trap_bottom_width)) / 2, (double)(height - 50)));

    dst.push_back(Point2d( ( width * (1 - trap_bottom_width)) / 2 , (double)height));
    dst.push_back(Point2d( ( width * (1 - trap_bottom_width)) / 2, 0.0));
    dst.push_back(Point2d( width - (width * (1 - trap_bottom_width)) / 2, 0.0));
    dst.push_back(Point2d( width - (width * (1 - trap_bottom_width)) / 2, (double)height));

    cout << src << endl;
    cout << dst << endl;

    color_filtered_image = color_filter(image);
    
    cvtColor(color_filtered_image, gray, COLOR_RGB2GRAY);
    
    M = getPerspectiveTransform(src, dst);
    
    binary_threshold =  Mat::zeros(gray.rows, gray.cols, gray.type());
    threshold(gray,binary_threshold, 0, 255, THRESH_BINARY);

    warpPerspective(binary_threshold, binary_warped, M, image.size(), INTER_LINEAR);

    return binary_warped;

}

Mat get_histogram(Mat binary_warped)
{
    MatND histogram;
    Mat half_image;

    half_image = binary_warped(Rect(0, binary_warped.rows / 2, binary_warped.cols, binary_warped.rows / 2));
	reduce(half_image / 255, histogram, 0, REDUCE_SUM, CV_32S);

	return histogram;
}

Mat sliding_window(Mat binary_warped)
{
   
    int N_windows;
    int window_width;
    int window_height;
    int midpoint;
    int left_x_current, right_x_current;

    Mat left_x_base;
    Mat right_x_base;
    Point left_peak, right_peak;
    
    Mat histogram;
    Mat output_image;
    Mat gray_tmp;

    N_windows = 9;
    window_width = 80;
    window_height = binary_warped.rows / N_windows;
    
    
    gray_tmp = binary_warped.clone();

    cvtColor(binary_warped, output_image, COLOR_GRAY2BGR);

    histogram = get_histogram(gray_tmp);
    midpoint = histogram.cols / 2;
    left_x_base = histogram.colRange(0,midpoint);
    right_x_base = histogram.colRange(midpoint,histogram.cols);

    minMaxLoc(left_x_base, NULL, NULL, NULL, &left_peak);
	minMaxLoc(right_x_base,NULL, NULL, NULL, &right_peak);

	right_peak = right_peak + Point(midpoint, 0);

    Window window_left(binary_warped, left_peak.x, output_image.rows - window_height, window_width, window_height, 50);
    Window window_right(binary_warped, right_peak.x, output_image.rows - window_height, window_width, window_height, 50);

    for(int i = 0; i < N_windows; ++i)
    {
        rectangle(output_image, window_left.get_bottom_left_point(), window_left.get_top_right_point(), Scalar(0,255, 0), 2);
        rectangle(output_image, window_right.get_bottom_left_point(), window_right.get_top_right_point(), Scalar(0,255, 0), 2);

        window_left = window_left.get_next_window(gray_tmp);
        window_right = window_right.get_next_window(gray_tmp);

    }
    return output_image;
 
}