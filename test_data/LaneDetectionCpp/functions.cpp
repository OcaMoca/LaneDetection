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

    src.push_back(Point2f( (width * (1 - trap_bottom_width)) / 2, height - 50));
    src.push_back(Point2f( (width * (1 - trap_top_width)) / 2,height - height * trap_height));
    src.push_back(Point2f( width - (width * (1 - trap_top_width)) / 2, height - height * trap_height));
    src.push_back(Point2f( width - (width * (1 - trap_bottom_width)) / 2, height - 50));

    dst.push_back(Point2f( ( width * (1 - trap_bottom_width)) / 2, height));
    dst.push_back(Point2f( ( width * (1 - trap_top_width)) / 2,0));
    dst.push_back(Point2f( width - (width * (1 - trap_top_width)) / 2, 0));
    dst.push_back(Point2f( width - (width * (1 - trap_bottom_width)) / 2, height));

    color_filtered_image = color_filter(image);
    
    cvtColor(color_filtered_image, gray, COLOR_RGB2GRAY);
    
    M = getPerspectiveTransform(src, dst);
    
    binary_threshold =  Mat::zeros(gray.rows, gray.cols, gray.type());
    threshold(gray,binary_threshold, 0, 255, THRESH_BINARY);

    warpPerspective(binary_threshold, binary_warped, M, image.size(), INTER_LINEAR);


    return binary_warped;

}

