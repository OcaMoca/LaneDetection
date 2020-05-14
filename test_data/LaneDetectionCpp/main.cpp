
#include "functions.hpp"

int main(int, char**) {
    /*cout << "OpenCV project!\n";

    Mat binary_warped;
    Mat result;
    
    const char* file = "..\\test_images\\curved.jpg";
    Mat image = cv::imread(file); 
    //result = pipeline(image);
    imshow("image", result);
    waitKey();*/

    bool processed;

    string file_name = "..\\test_videos\\solidYellowLeft.mp4";
    string output_file = "..\\test_videos_output_cpp\\solidYellowLeft_out_class.mp4";

    LaneDetection ld;

    ld.init(file_name, output_file);

    int cnt = 0;

    while(true)
    {
        processed = ld.frame_processing();
        if(!processed)
            break;
        cnt++;  
        cout << cnt << endl;

    }

    ld.release();
   
}

