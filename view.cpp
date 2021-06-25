
#include "highgui.hpp"

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    vector<double> testVector;

    for( int i = 0; i < 65536; i++)
    {
        testVector.push_back( i % 256 );
    }

    Mat testMat = Mat( testVector ).reshape( 0, 256 );

    testMat.convertTo( testMat, CV_8UC1 );

    imshow("testMat",testMat);
    waitKey();
}

#include "opencv2/highgui.hpp"
#include <vector>
#include <iostream>
#include <ctype.h>
#include <cstdlib>

int main()
{
    //  create a uint8_t array, can be unsigned char too
    uint8_t myArray[120*160];

    //  fill values
    srand(time(0));
    for (int i = 0; i < 120*160; ++i)
    {
        myArray[i] = (rand() % 255) + 1;
    }

    //  create grayscale image
    cv::Mat imgGray(120, 160, CV_8UC1, myArray);

    cv::namedWindow("test", cv::WINDOW_AUTOSIZE);
    cv::imshow("test", imgGray);
    cv::waitKey(0);

    return 0;
}
