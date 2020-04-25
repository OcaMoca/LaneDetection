#include "functions.hpp"

Mat color_filter(Mat image)
{
    Mat mask1;
    Mat mask2;
    Mat image_hsv;
    Mat white_image;
    Mat yellow_image;
    Mat filtered_image;

    inRange(image, Scalar(200,200,200), Scalar(255,255,255), mask1);

    cvtColor(image, image_hsv, COLOR_BGR2HSV);
    inRange(image_hsv, Scalar(90,72,100), Scalar(110,255,255), mask2);
    
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
    int min_pix;
    int midpoint;
    Mat output_image;
    Mat histogram;
    Mat left_x_base;
    Mat right_x_base;
    int left_x_current;
    int right_x_current;
    vector<Point2d> non_zero;
    vector<double> non_zero_x;
    vector<double> non_zero_y;
    vector<Point2d> left_lane_inds;
    vector<Point2d> right_lane_inds;
    Point peak_left, peak_right;

    N_windows = 9;
    window_width = 80;
    window_height = binary_warped.cols / N_windows;
    min_pix = 50;
    

    histogram = get_histogram(binary_warped);

    midpoint = histogram.cols / 2;
    left_x_base = histogram.colRange(0,midpoint);
    right_x_base = histogram.colRange(midpoint,histogram.cols);

    minMaxLoc(left_x_base, NULL, NULL, NULL, &peak_left);
	minMaxLoc(right_x_base,NULL, NULL, NULL, &peak_right);

	peak_right = peak_right + Point(midpoint, 0);

    left_x_current = peak_left.x;
    right_x_current = peak_right.x;

	//right_max_loc = right_max_loc + cv::Point(midpoint, 0);
    findNonZero(binary_warped, non_zero);

    for (int i = 0; i < non_zero.size(); i++){
        non_zero_x.push_back(non_zero[i].x);
        non_zero_y.push_back(non_zero[i].y);

    }

    int win_y_low;
    int win_y_high;
    int win_x_left_low;
    int win_x_left_high;
    int win_x_right_low;
    int win_x_right_high;

    for(int i = 0; i < N_windows; i++)
    {
        win_y_low = int(binary_warped.rows - (i + 1) * window_height);
        win_y_high = int(binary_warped.rows- i * window_height);
        win_x_left_low = left_x_current - window_width;
        win_x_left_high = left_x_current + window_width;
        win_x_right_low = right_x_current - window_width;
        win_x_right_high = right_x_current + window_width;

        Point2d bottom_left_lane_left(win_x_left_low,win_y_low);
        Point2d top_right_lane_left(win_x_left_high,win_y_high);
        Point2d bottom_right_lane_right(win_x_right_low,win_y_low);
        Point2d top_right_lane_right(win_x_right_high,win_y_high);

        rectangle(output_image,bottom_left_lane_left,top_right_lane_left,(0,255,0));
        rectangle(output_image,bottom_right_lane_right,top_right_lane_right,(0,255,0));

        /*compare(non_zero_y, win_y_low, good_left_inds, CMP_GE);
        compare()
        findNonZero(((non_zero_y >= win_y_low) & (non_zero_y < win_y_high) & (non_zero_x >= win_x_left_low) & (non_zero_x < win_x_left_high))[0],good_left_inds);
*/
        
        }

    return binary_warped;
 
}