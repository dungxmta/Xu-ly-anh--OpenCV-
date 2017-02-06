#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "highgui.h"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

/** @function main */
int main( int argc, char** argv )
{
/// load anh?
  Mat src = imread( argv[1] );
  /*
    Load anh?
    - imread("image_name.jpg");
    
    Khai bao' anh?
    - Mat src;
    Chi?ro~ chieu dai'+rong=voi' anh? src & kieu?gia tri.moi~ muc xam la' CV_8U
    - Mat img_out(src.size(), CV_8U);
  */
  if( !src.data )
    return -1;

/// tao.khung cua?so^? de^?load anh? co' ten^ la "source img", "gray img"
  namedWindow ("source img", CV_WINDOW_AUTOSIZE);
  /*
    options
    - WINDOW_NORMAL : can be resize
    - CV_WINDOW_AUTOSIZE : fit with img and no-resize
  */

/// vi du convert anh?mau -> anh?xam'
    Mat gray;
    // ham' convert tu anh? BGR->xam'
    cvtColor( src, gray, CV_BGR2GRAY );
    // tao.cua?so^?load anh?
    namedWindow ("gray img", CV_WINDOW_AUTOSIZE);
    // show anh? len^ cua?so^? "gray img"
    imshow( "gray img", gray );

/// vi du thredshold
    int thredshold_vaule = 100;
    Mat gray_clone = gray.clone();// tao. anh? clone cua?anh? gray
    Mat img_out;// output

    for (int i = 0; i < gray_clone.cols; ++i)
      {
        for (int j = 0; j < gray_clone.rows; ++j)
        {
            int value_of_pixel = (int) img_out.at<uchar>(i,j);
            /* 
                - img_out.at<uchar>(i,j) : muc' xam tai. vi.tri' (i,j)
                - uchar: unsigned char ~ 0->255
            */
            if (value_of_pixel > thredshold_vaule)
                value_of_pixel = 255;
            else
                value_of_pixel = 0;
        }
    }
    namedWindow ("thredshold demo", CV_WINDOW_AUTOSIZE);
    imshow( "thredshold demo", img_out );

/// end
    waitKey(0);// bam' nut' bat ky de^? stop chuong trinh
    return 0;
}








