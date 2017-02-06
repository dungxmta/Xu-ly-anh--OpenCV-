#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

/** @function main */
int main( int argc, char** argv )
{

  Mat src, src_gray;
  Mat grad;
  // char* window_name = "Sobel Demo - Simple Edge Detector";
  int scale = 1;
  int delta = 0;
  int ddepth = CV_16S;// to avoid overflow; The depth of the output image

  int threshold = 30000;

  /// Load an image
  src = imread( argv[1] );

  if( !src.data )
  { return -1; }

  // GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );

  /// Convert it to gray
  cvtColor( src, src_gray, CV_BGR2GRAY );

  /// Create window
  // namedWindow( window_name, CV_WINDOW_AUTOSIZE );
  namedWindow( "Iy", CV_WINDOW_AUTOSIZE );
  namedWindow( "Ix", CV_WINDOW_AUTOSIZE );
  namedWindow( "img_out", CV_WINDOW_AUTOSIZE );
  // namedWindow( "grad_x", CV_WINDOW_AUTOSIZE );

  /// Generate grad_x and grad_y
  Mat grad_x, grad_y;// CV_16S
  Mat Ix, Iy;// CV_8U

  /// Gradient X
  //Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
  Sobel( src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
  convertScaleAbs( grad_x, Ix );// convert CV_16S back to CV_8U

  /// Gradient Y
  //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
  Sobel( src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
  convertScaleAbs( grad_y, Iy );// convert CV_16S back to CV_8U


  
  // calc IxIx, IyIy
  Mat IxIx(src_gray.size(), CV_16S), IyIy(src_gray.size(), CV_16S), IxIy(src_gray.size(), CV_16S);// CV_16S - short −32,768 to 32,767
  // float IxIx, IyIy, IxIy, R;
  for (int i = 0; i < src_gray.cols; ++i)
  {
    for (int j = 0; j < src_gray.rows; ++j)
    {
      IxIx.at<short>(i,j) = Ix.at<uchar>(i,j) * Ix.at<uchar>(i,j);
      IyIy.at<short>(i,j) = Iy.at<uchar>(i,j) * Iy.at<uchar>(i,j);
      IxIy.at<short>(i,j) = Ix.at<uchar>(i,j) * Iy.at<uchar>(i,j);
      // IxIx = Ix.at<uchar>(i,j) * Ix.at<uchar>(i,j);
      // IyIy = Iy.at<uchar>(i,j) * Iy.at<uchar>(i,j);
      // IxIy = Ix.at<uchar>(i,j) * Iy.at<uchar>(i,j);

      // float det_H = IxIx*IyIy - IxIy*IxIy;
      // float trace_H = IxIx + IyIy;
      // R = det_H - 0.04*trace_H*trace_H;
      // if (R > 0)
      //     printf("%.2f ", R);
    }
    // printf("\n");
  }
  Mat Ix2, Iy2, IxIy2;
  GaussianBlur( IxIx, Ix2, Size(3,3), 0, 0, BORDER_DEFAULT );
  GaussianBlur( IyIy, Iy2, Size(3,3), 0, 0, BORDER_DEFAULT );
  GaussianBlur( IxIy, IxIy2, Size(3,3), 0, 0, BORDER_DEFAULT );

  Mat img_out(src_gray.size(), CV_8U);
  // float Gxx, Gyy, Gxy, R;
  Mat Gxx, Gyy, Gxy;
  float R;    
  convertScaleAbs( Ix2, Gxx );
  convertScaleAbs( Iy2, Gyy );
  convertScaleAbs( IxIy2, Gxy );

  for (int i = 0; i < src_gray.cols; ++i)
  {
    for (int j = 0; j < src_gray.rows; ++j)
    {
      float Gxx_ = (float)Gxx.at<uchar>(i,j);
      float Gyy_ = (float)Gyy.at<uchar>(i,j);
      float Gxy_ = (float)Gxy.at<uchar>(i,j);
      // IxIx.at<short>(i,j) = Ix.at<uchar>(i,j) * Ix.at<uchar>(i,j);
      // IyIy.at<short>(i,j) = Iy.at<uchar>(i,j) * Iy.at<uchar>(i,j);
      // IxIy.at<short>(i,j) = Ix.at<uchar>(i,j) * Iy.at<uchar>(i,j);

      // Gxx = Ix2.at<short>(i,j);// * Ix2.at<short>(i,j);
      // Gyy = Iy2.at<short>(i,j);// * Iy2.at<short>(i,j);
      // Gxy = IxIy2.at<short>(i,j);// * Iy2.at<short>(i,j);

      float det_H = Gxx_*Gyy_ - Gxy_*Gxy_;
      float trace_H = Gxx_ + Gyy_;
      R = det_H - 0.04*trace_H*trace_H;
      if (R > threshold)
      {
          // printf("%.2f ", R);
          img_out.at<uchar>(i,j) = 255;
      }
      else
        img_out.at<uchar>(i,j) = 0;
    }
    // printf("\n");
  }
  // printf("%.2f - %.2f - %.2f", IxIx, IyIy, IxIy);

  //test
  // for (int i = 0; i < 20; ++i)
  // {
  //   for (int j = 0; j < 20; ++j)
  //   {
  //     printf("%3.2f ", (float)grad_x.at<uchar>(i,j));
  //   }
  //   printf("\n");
  // }
  // printf("\n\n");
  // for (int i = 0; i < 20; ++i)
  // {
  //   for (int j = 0; j < 20; ++j)
  //   {
  //     printf("%3.2f ", (float)Ix.at<uchar>(i,j));
  //   }
  //   printf("\n");
  // }
  /// Total Gradient (approximate)
  // addWeighted( Ix, 0.5, Iy, 0.5, 0, grad );
  imshow( "Iy", Iy );
  imshow( "Ix", Ix );
  imshow( "img_out", img_out );
  // imshow( "grad_y", grad_y );
  // imshow( "grad_x", grad_x );
  // imshow( window_name, grad );

  waitKey(0);

  return 0;
  }