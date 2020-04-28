#include "window.hpp"

Window::Window(Mat& binary_warped, int x_center, int y_top, int width, int height, int min_pix)
{
    int margin;
    Mat window;

    this->x_center = x_center;
    this->y_top = y_top;
    this->width = width;
    this->height = height;
    this->min_pix = min_pix;

    margin = this->width / 2;
    this->x_left = this->x_center - margin;
    this->y_bottom = y_top + this->height;
    this->x_right = this->x_center + margin;

    Rect rect(Point(x_left, y_bottom), Point(x_right, y_top));
    window = binary_warped(rect);
    findNonZero(window, non_zero);

}

Window Window::get_next_window(Mat& binary_warped)
{
    int sum;
    int cnt_nonzeros;
    cnt_nonzeros = count_nonzero();

    cout << cnt_nonzeros << endl;
    if(this->y_top == 0) return Window(); //if it's the end of window chain

    int new_y_top = y_top - height;
    int new_x_center = x_center;

    if (cnt_nonzeros >= min_pix)
    {
        sum = 0;
        for(Point const& pt: non_zero)
        {
            sum += (pt.x + x_left);
            non_zero_x.push_back(pt.x);
            non_zero_y.push_back(pt.y);
        }

        new_x_center = sum / (int)non_zero.size();
    }
    

    if(new_x_center + this->width / 2 > binary_warped.cols) return Window();

    Window result(binary_warped, new_x_center, new_y_top, this->width, this->height, this->min_pix);
    
    return result;
}


void Window::get_indices(Mat& x, Mat& y) const
{
	x.release();
	y.release();

	int npoints = count_nonzero();

	x = Mat::zeros(npoints, 1, CV_8UC1);
	y = Mat::zeros(npoints, 1, CV_8UC1);

	for (int i = 0; i < npoints; i++)
    {
		x.at<double>(i, 0) = non_zero[i].x + x_left;
		y.at<double>(i, 0) = non_zero[i].y + y_bottom;
	}

	return;
}



