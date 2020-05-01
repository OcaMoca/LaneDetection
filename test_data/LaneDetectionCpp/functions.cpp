#include "functions.hpp"

void polyfit(const Mat& src_x, const Mat& src_y, Mat& dst, int order)
{
	Mat X;
	X = Mat::zeros(src_x.rows, order + 1, CV_64FC1);

	Mat copy;
	for (int i = 0; i <= order; i++)
	{
		copy = src_x.clone();
		pow(copy, i, copy);
		Mat M1 = X.col(i);
		copy.col(0).copyTo(M1);
	}
	Mat X_t, X_inv;
	transpose(X, X_t);
	Mat temp = X_t * X;
	Mat temp2;
	invert(temp, temp2);
	Mat temp3 = temp2 * X_t;
	Mat W = temp3 * src_y;

	W.copyTo(dst);

}

vector<double> linespace(double start_in, double end_in, int num_in)
{
  vector<double> linspaced;

  double start = static_cast<double>(start_in);
  double end = static_cast<double>(end_in);
  double num = static_cast<double>(num_in);

  if (num == 0) { return linspaced; }
  if (num == 1) 
    {
      linspaced.push_back(start);
      return linspaced;
    }

  double delta = (end - start) / (num - 1);

  for(int i=0; i < num-1; ++i)
    {
      linspaced.push_back(start + delta * i);
    }
  linspaced.push_back(end); 

  return linspaced;
}

Mat polyfit_windows(vector<Window> const& windows)
{
	int n = (int)windows.size();

	vector<Mat> x_mat, y_mat;

	x_mat.reserve(n);
	y_mat.reserve(n);

	Mat x_temp, y_temp;

	for (Window const& window : windows)
  {
		window.get_indices(x_temp, y_temp);
		x_mat.push_back(x_temp);
		y_mat.push_back(y_temp);
	}

	Mat xs, ys;

	vconcat(x_mat, xs);
	vconcat(y_mat, ys);

	Mat fit = Mat::zeros(3, 1, CV_64FC1);

	polyfit(ys, xs, fit, 2);

	return fit;

}


void poly_fit_x(vector<double> const& ploty, std::vector<double>& fit_x, Mat const& line_fit)
{
	for (auto const& y : ploty) {
		double x = line_fit.at<double>(2, 0) * y * y + line_fit.at<double>(1, 0) * y + line_fit.at<double>(0, 0);
		fit_x.push_back(x);
	}

	return;
}


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
    inRange(image_hsv, Scalar(20,90,100), Scalar(40,255,255), mask2);

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
  Mat histogram;
  Mat half_image;

  half_image = binary_warped(Rect(0, binary_warped.rows / 2, binary_warped.cols, binary_warped.rows / 2));
	reduce(half_image / 255, histogram, 0, REDUCE_SUM, CV_32FC2);

	return histogram;
}

Mat sliding_window(Mat binary_warped, Mat& left_fit, Mat& right_fit)
{
   
    int N_windows;
    int window_width;
    int window_height;
    int midpoint;

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

    Window window_left(binary_warped, left_peak.x, binary_warped.rows - window_height, window_width, window_height, 50);
    Window window_right(binary_warped, right_peak.x, binary_warped.rows - window_height, window_width, window_height, 50);

    vector<Window> left_boxes, right_boxes;

    for(int i = 0; i < N_windows; ++i)
    {
        rectangle(output_image, window_left.get_bottom_left_point(), window_left.get_top_right_point(), Scalar(0,255, 0), 2);
        rectangle(output_image, window_right.get_bottom_left_point(), window_right.get_top_right_point(), Scalar(0,255, 0), 2);

        left_boxes.push_back(window_left);
        right_boxes.push_back(window_right);
        
        window_left = window_left.get_next_window(gray_tmp);
        window_right = window_right.get_next_window(gray_tmp);

    }

    left_fit = polyfit_windows(left_boxes);
    right_fit = polyfit_windows(right_boxes);

    return output_image;
 
}

Mat inverse_perspective(Mat binary_warped, Mat original)
{
    Mat sliding_window_output;
    vector<double> left_fit_x, right_fit_x, ploty;
    Mat Minv(2,4,CV_32FC2);
    Mat left_fit, right_fit;
    Mat output_image;
    Mat color_warp = Mat::zeros(original.size(), CV_8UC3);
    vector<Point2f> src,dst;

    int width = binary_warped.size().width;
    int height = binary_warped.size().height;

    double trap_bottom_width = 0.7; 
    double trap_top_width = 0.1; 
    double trap_height = 0.38; 

    sliding_window_output = sliding_window(binary_warped, left_fit, right_fit);

    src.push_back(Point2d( (width * (1 - trap_bottom_width)) / 2, (double)(height - 50)));
    src.push_back(Point2d( (width * (1 - trap_top_width)) / 2, height - height * trap_height));
    src.push_back(Point2d( width - (width * (1 - trap_top_width)) / 2, height - height * trap_height));
    src.push_back(Point2d( width - (width * (1 - trap_bottom_width)) / 2, (double)(height - 50)));

    dst.push_back(Point2d( ( width * (1 - trap_bottom_width)) / 2 , (double)height));
    dst.push_back(Point2d( ( width * (1 - trap_bottom_width)) / 2, 0.0));
    dst.push_back(Point2d( width - (width * (1 - trap_bottom_width)) / 2, 0.0));
    dst.push_back(Point2d( width - (width * (1 - trap_bottom_width)) / 2, (double)height));

    Minv = getPerspectiveTransform(dst,src);

    warpPerspective(sliding_window_output, output_image, Minv, sliding_window_output.size(),INTER_LINEAR);

    ploty = linespace(0, output_image.rows - 1, output_image.rows);

    poly_fit_x(ploty, left_fit_x, left_fit);
    poly_fit_x(ploty, right_fit_x, right_fit);

    int npoints = (int)ploty.size();
    vector<Point> pts_left(npoints);
    vector<Point>  pts_right(npoints);
    vector<Point> pts;

    for (int i = 0; i < npoints; i++) {
      pts_left.push_back(Point2d((double)left_fit_x[i], (double)ploty[i])); 
      pts_right.push_back(Point2d((double)right_fit_x[i], (double)ploty[i]));
    }

    pts.reserve(2 * ploty.size());
    pts.insert(pts.end(), pts_left.begin(), pts_left.end());
    pts.insert(pts.end(), pts_right.rbegin(), pts_right.rend());

    vector<vector<Point>> ptsarray{ pts };

    fillPoly(color_warp, ptsarray, Scalar(0, 255, 0));

    Mat new_warp;

    warpPerspective(color_warp, new_warp, Minv, color_warp.size(),INTER_LINEAR);

    addWeighted(original, 1, new_warp, 0.3, 0, output_image);

    return output_image;

}

