
#include "functions.hpp"

int main(int, char**) {
    /*cout << "OpenCV project!\n";

    Mat binary_warped;
    Mat result;
    
    const char* file = "..\\test_images\\curved.jpg";
    Mat image = cv::imread(file); 
    result = pipeline(image);
    imshow("image", result);
    waitKey();*/

    string filename = "..\\test_videos\\challenge.mp4";
    VideoCapture capture(filename);
    Mat frame, result;

    int frame_width = (int)capture.get(CAP_PROP_FRAME_WIDTH);
    int frame_height = (int)capture.get(CAP_PROP_FRAME_HEIGHT);
    VideoWriter video_output("..\\test_videos_output_cpp\\challenge_out_opt.mp4",VideoWriter::fourcc('M', 'P', '4', 'V'),25.0, Size(frame_width,frame_height),true);
    
    if( !capture.isOpened() )
        throw "Error when reading steam_mp4";

    if (!video_output.isOpened())
    {
        std::cout << "!!! Output video could not be opened" << std::endl;
        return 0;
    }

    int cnt = 0;
    while(true)
    {
        if(!capture.read(frame))
            break;


        result = pipeline(frame);
        
        video_output.write(result);
        cnt++;
        cout << cnt << endl;

    }

    cout << "SAVED." << endl;
    capture.release();
	video_output.release();
   
}

