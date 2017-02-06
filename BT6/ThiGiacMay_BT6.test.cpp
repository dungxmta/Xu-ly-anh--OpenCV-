#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "highgui.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;
using namespace cv;

/// Global variables
Mat src, erosion_dst, dilation_dst, opening_dst, closing_dst, BE_dst;

int SE_type = 0;// type of Structuring Element
int erosion_size = 0;// size of Erosion SE
int dilation_size = 0;
int opening_size = 0;
int closing_size = 0;
int BE_size = 0;
int const max_SE_type = 2;
int const max_SE_size = 100;

Mat img;// temp img for process
int img_type = 0;
int threshold_value = 0;

/** Function Headers */
void Load_image ( int, void*);
void Erosion ( int, void* );
void Dilation ( int, void* );
void Opening ( int, void* );
void Closing ( int, void* );
void Boundary_extraction (int, void*);

void Hole_filling (int, void*);
/** @function main */
int main( int argc, char** argv )
{
/// Load an image
  src = imread( argv[1] );

  if( !src.data )
    return -1;

/// Create windows
  namedWindow ("MODE", WINDOW_NORMAL);// can be resize
  namedWindow ("Erosion trackbar", CV_WINDOW_AUTOSIZE);// fit with img
  namedWindow ("Dilation trackbar", CV_WINDOW_AUTOSIZE);// and no-resize
  namedWindow ("Opening trackbar", CV_WINDOW_AUTOSIZE);
  namedWindow ("Closing trackbar", CV_WINDOW_AUTOSIZE);
  namedWindow ("Boundary extraction trackbar", CV_WINDOW_AUTOSIZE);
  namedWindow ("Hole filling trackbar", CV_WINDOW_AUTOSIZE);
  
  // cvMoveWindow( "Dilation trackbar", src.cols, 0 );

/// Create Load_type_img Trackbar
  // createTrackbar( "change_value_name", "window_name", &change_value, max_value, Callback_funcion );
  createTrackbar( "Image-type:        \n   0: source \n   1: Grayscale \n   2: Binary", "MODE", &img_type, 2, Load_image);

  createTrackbar( "Threshold-value\n(only-for-Binary)","MODE", &threshold_value,255, Load_image );

  createTrackbar( "SE-type:                \n   0: Rect \n   1: Cross \n   2: Ellipse", "MODE", &SE_type, max_SE_type, Load_image );

  /// Create Erosion Trackbar
  createTrackbar( "      SE-size            \n[2n+1]x[2n+1]", "Erosion trackbar", &erosion_size, max_SE_size, Erosion );

  /// Create Dilation Trackbar
  createTrackbar( "      SE size\n[2n+1]x[2n+1]", "Dilation trackbar", &dilation_size, max_SE_size, Dilation );

  /// Create Opening Trackbar
  createTrackbar( "      SE-size            \n[2n+1]x[2n+1]", "Opening trackbar", &opening_size, max_SE_size, Opening );
  /// Create Closing Trackbar
  createTrackbar( "      SE-size            \n[2n+1]x[2n+1]", "Closing trackbar", &closing_size, max_SE_size, Closing );
  /// Create Boundary extraction Trackbar
  createTrackbar( "      SE-size            \n[2n+1]x[2n+1]", "Boundary extraction trackbar", &BE_size, max_SE_size, Boundary_extraction );

/// Default start
  Load_image (0, 0);
  // Erosion (0, 0);
  // Dilation (0, 0);

// imwrite( "../../images/Gray_Image.jpg", gray_image );
  waitKey(0);
  return 0;
}

/**  @function Load_type_image  */
void Load_image( int, void*)
{
  if (img_type == 0)
    img = src.clone();
  else if (img_type == 1)
    //img = src.clone();//imread (src, CV_LOAD_IMAGE_GRAYSCALE);// 0
    cvtColor( src, img, CV_BGR2GRAY );
  else if (img_type == 2)
  {
    Mat gray;
    cvtColor( src, gray, CV_BGR2GRAY );
    // threshold( src_gray, dst, threshold_value, max_BINARY_value,threshold_type );
    threshold (gray, img, threshold_value, 255, 0);// 0: Binary
    Hole_filling(threshold_value, 0);
  }
  // SE_size = 0;
  // imshow( "Erosion trackbar", img);
  // imshow( "Dilation trackbar", img);
  Erosion (SE_type, 0);
  Dilation (SE_type, 0);
  Opening (SE_type, 0);
  Closing (SE_type, 0);
  Boundary_extraction (SE_type, 0);
}
/**  @function Erosion  */
void Erosion( int, void* )
{
  int erosion_type;
  if( SE_type == 0 )
    erosion_type = MORPH_RECT;
  else if( SE_type == 1 )
    erosion_type = MORPH_CROSS;
  else if( SE_type == 2)
    erosion_type = MORPH_ELLIPSE;
  // Mat getStructuringElement(int shape, Size ksize, Point anchor=Point(-1,-1))
  Mat element = getStructuringElement( erosion_type, Size( 2*erosion_size + 1, 2*erosion_size+1 ), Point( erosion_size, erosion_size ) );

  /// Apply the erosion operation
  erode( img, erosion_dst, element );
  imshow( "Erosion trackbar", erosion_dst );

  //test
  printf("\n");
  // for (int i = 0; i < 2*erosion_size+1; ++i)
  // {
  //   for (int j = 0; j < 2*erosion_size+1; ++j)
  //   {
  //     // printf("%3d ", element.at<uchar>(i,j));
  //     printf("%3d ", element.at<uchar>(i,j));
  //   }
  //   printf("\n");
  // }
  cout<<element;
  printf("\n");

}

/** @function Dilation */
void Dilation( int, void* )
{
  int dilation_type;
  if( SE_type == 0 )
    dilation_type = MORPH_RECT;
  else if( SE_type == 1 )
    dilation_type = MORPH_CROSS;
  else if( SE_type == 2)
    dilation_type = MORPH_ELLIPSE;

  Mat element = getStructuringElement( dilation_type, Size( 2*dilation_size + 1, 2*dilation_size+1 ), Point( dilation_size, dilation_size ) );
  /// Apply the dilation operation
  dilate( img, dilation_dst, element );
  imshow( "Dilation trackbar", dilation_dst );
}

/** @function Opening */
void Opening( int, void* )
{
  int opening_type;
  if( SE_type == 0 )
    opening_type = MORPH_RECT;
  else if( SE_type == 1 )
    opening_type = MORPH_CROSS;
  else if( SE_type == 2)
    opening_type = MORPH_ELLIPSE;

  Mat element = getStructuringElement( opening_type, Size( 2*opening_size + 1, 2*opening_size+1 ), Point( opening_size, opening_size ) );
  /// Apply the dilation operation
  morphologyEx( img, opening_dst, 2, element );// 2: MORPH_OPEN
  imshow( "Opening trackbar", opening_dst );
}

/** @function Closing */
void Closing( int, void* )
{
  int closing_type;
  if( SE_type == 0 )
    closing_type = MORPH_RECT;
  else if( SE_type == 1 )
    closing_type = MORPH_CROSS;
  else if( SE_type == 2)
    closing_type = MORPH_ELLIPSE;

  Mat element = getStructuringElement( closing_type, Size( 2*closing_size + 1, 2*closing_size+1 ), Point( closing_size, closing_size ) );
  /// Apply the dilation operation
  morphologyEx( img, closing_dst, 3, element );// 3: MORPH_CLOSE
  imshow( "Closing trackbar", closing_dst );
}

/** @function Boundary_extraction */
void Boundary_extraction (int, void*)
{
  int erosion_type;
  if( SE_type == 0 )
    erosion_type = MORPH_RECT;
  else if( SE_type == 1 )
    erosion_type = MORPH_CROSS;
  else if( SE_type == 2)
    erosion_type = MORPH_ELLIPSE;
  // Mat getStructuringElement(int shape, Size ksize, Point anchor=Point(-1,-1))
  Mat element = getStructuringElement( erosion_type, Size( 2*BE_size + 1, 2*BE_size+1 ), Point( BE_size, BE_size ) );

  Mat img_after_erosion;
  /// Apply the erosion operation
  erode( img, img_after_erosion, element );
  // subtract(img, img_after_erosion, BE_dts);
  BE_dst = img - img_after_erosion;
  imshow( "Boundary extraction trackbar", BE_dst);
}

/** @function Hole_filling */
void Hole_filling (int, void*)
{
    Mat img_thresh;
    threshold(img, img_thresh, threshold_value, 255, THRESH_BINARY_INV);
     
    // Floodfill from point(0,0)
    Mat img_floodfill = img_thresh.clone();
    floodFill(img_floodfill, Point(0,0), Scalar(255));
     
    // Invert floodfilled image
    Mat img_floodfill_invert;
    bitwise_not(img_floodfill, img_floodfill_invert);// OR: inverts every bit of an array.
     
    // Combine the two images to get the foreground.
    Mat HL_dst = (img_thresh | img_floodfill_invert);
 
    // Display images
    // imshow("Thresholded Image", im_th);
    // imshow("Floodfilled Image", im_floodfill);
    // imshow("Inverted Floodfilled Image", im_floodfill_inv);
    imshow("Hole filling trackbar", HL_dst);
}