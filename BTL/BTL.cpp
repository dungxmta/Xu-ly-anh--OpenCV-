#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "highgui.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

// -----------------------------
#include "THRESHOLD.cpp"
// -----------------------------

using namespace std;
using namespace cv;

/** Function Headers */
Mat Erosion (Mat src, Mat element);
Mat Dilation (Mat src, Mat element);
Mat Opening (Mat src, Mat element);
Mat Closing (Mat src, Mat element);
Mat Boundary_extraction (Mat src, Mat element);
Mat HitOrMiss (Mat src);

/** @function main */
int main( int argc, char** argv )
{
/// Load an image
    Mat src = imread( argv[1] );
    if( !src.data )
    { return -1; }
/// Create windows
    // namedWindow( "Gray_Demo", CV_WINDOW_AUTOSIZE );
    // namedWindow( "Zero_Demo", CV_WINDOW_AUTOSIZE );
    // namedWindow( "Erosion_Demo", CV_WINDOW_AUTOSIZE );
    // namedWindow( "Dilation_Demo", CV_WINDOW_AUTOSIZE );
    // namedWindow( "Opening_Demo", CV_WINDOW_AUTOSIZE );
    // namedWindow( "Closing_Demo", CV_WINDOW_AUTOSIZE );
    // namedWindow( "BoundaryExtraction_Demo", CV_WINDOW_AUTOSIZE );
    // namedWindow( "HitOrMiss_Demo", CV_WINDOW_AUTOSIZE );
/// Convert to binary
    // int threshold_value = 100;
    float threshold_value = 0;
    Mat gray, zero;
    cvtColor( src, gray, CV_BGR2GRAY );
    // threshold (gray, zero, threshold_value, 255, 0);// 0: Binary, CV_THRESH_BINARY
    getThresholdValue_Otsu (gray, threshold_value);
    cout<<"threshold_value = "<<threshold_value<<endl;
    zero = Threshold (gray, threshold_value);
// SE
    int SE_size = 5;
    Mat element = getStructuringElement( MORPH_CROSS, Size(2*SE_size+1, 2*SE_size+1), Point( SE_size, SE_size ) );
    // MORPH_RECT - MORPH_CROSS - MORPH_ELLIPSE
    cout<<"element"<<endl<<element<<endl;
    // SE tam = 0
    // uchar bits[] = {0,1,0,1,0,1,0,1,0};
    // uchar bits[] = {0,0,0,1,0,1,0,0,0};
    // uchar bits[] = {1,1,1,1,1,1,1,1,1};
    // uchar bits[] = {0,0,0,1,1,1,0,0,0};
    // Mat element_test( 3,3, CV_8UC1, bits );
    // Mat element_2x2 = getStructuringElement(MORPH_RECT, Size(10,10));
// function call
    Mat dst_ero = Erosion (zero, element);
    Mat dst_dila = Dilation (zero, element);
    Mat dst_open = Opening (zero, element);
    Mat dst_close = Closing (zero, element);
    Mat dst_BE = Boundary_extraction (zero, element);
    
    // for Hit-or-miss transform
    Mat gray2 = imread("HitOrMiss.jpg",0);
    float threshold_value_2 = 0;
    getThresholdValue_Otsu (gray2, threshold_value_2);
    cout<<"threshold_value_2= "<<threshold_value_2<<endl;
    Mat zero2 = Threshold (gray2, threshold_value_2);

    Mat dst_HoM = HitOrMiss (zero2);
// show img
    imshow ("Gray_Demo", gray);
    imshow ("Zero_Demo", zero);
    imshow ("Erosion_Demo", dst_ero);
    imshow ("Dilation_Demo", dst_dila);
    imshow ("Opening_Demo", dst_open);
    imshow ("Closing_Demo", dst_close);
    imshow ("BoundaryExtraction_Demo", dst_BE);

    imshow ("Img_for_HitOrMiss", zero2);
    imshow ("HitOrMiss_Demo", dst_HoM);

//-----------test-DILATION
    // Mat test_dila;
    // dilate( zero, test_dila, element );
    // namedWindow( "test_dila", CV_WINDOW_AUTOSIZE );
    // imshow ("test_dila", test_dila);

    // Mat output_dila(src.size(), CV_8UC1);
    // for (int i = 0; i < src.rows; ++i)
    // {
    //     for (int j = 0; j < src.cols; ++j)
    //     {
    //         if (test_dila.at<uchar>(i,j) != dst_dila.at<uchar>(i,j))
    //         {
    //             printf("\nfail[%3d][%3d] - src=%3d - Dtz=%3d - dila=%3d",i,j,zero.at<uchar>(i,j), dst_dila.at<uchar>(i,j), test_dila.at<uchar>(i,j));
    //             output_dila.at<uchar>(i,j) = 255;
    //         }
    //         else
    //             output_dila.at<uchar>(i,j) = 0;
    //     }
    // }
    // namedWindow( "test", CV_WINDOW_AUTOSIZE );
    // imshow ("test", output_dila);

//-----------test-EROSION
    // Mat test_ero;
    // erode( zero, test_ero, element_test );
    // namedWindow( "test_ero", CV_WINDOW_AUTOSIZE );
    // imshow ("test_ero", test_ero);

    // Mat output_ero(src.size(), CV_8UC1);
    // for (int i = 0; i < src.rows; ++i)
    // {
    //     for (int j = 0; j < src.cols; ++j)
    //     {
    //         if (test_ero.at<uchar>(i,j) != dst_ero.at<uchar>(i,j))
    //         {
    //             printf("\nfail[%3d][%3d] - src=%3d - Dtz=%3d - dila=%3d",i,j,zero.at<uchar>(i,j), dst_ero.at<uchar>(i,j), test_ero.at<uchar>(i,j));
    //             output_ero.at<uchar>(i,j) = 255;
    //         }
    //         else
    //             output_ero.at<uchar>(i,j) = 0;
    //     }
    // }
    // namedWindow( "test", CV_WINDOW_AUTOSIZE );
    // imshow ("test", output_ero);

//------------------test-new
    // uchar bits[] = {0,1,0,1,0,1,0,1,0};
    // Mat element( 3,3, CV_8UC1, bits );
    // cout<<"test_element"<<endl<<element<<endl;

    // uchar bits2[] = {1,1,1,1,1,1,1,1,1};
    // Mat element_full( 3,3, CV_8UC1, bits2 );
    // cout<<"test_element_full_1"<<endl<<element_full<<endl;

    // uchar data[] = {  0,255,  0,255,  0,255,  0,255,255,
    //                 255,  0,255,255,255,  0,255,255,255,
    //                 255,255,255,  0,255,255,  0,255,255,
    //                   0,255,  0,255,255,255,255,  0,255,
    //                 255,  0,255,255,255,255,  0,  0,255,
    //                 255,255,255,  0,255,255,  0,255,255,
    //                 255,  0,  0,255,255,255,  0,255,255,
    //                   0,  0,255,255,255,  0,255,255,255,
    //                 255,255,  0,255,255,255,  0,255,255,};

    // Mat input( 9,9, CV_8UC1, data );
    // cout<<"input"<<endl<<input<<endl;

    // // cout<<"src"<<endl<<src<<endl;

    // Mat output_ero;
    // erode( input, output_ero, element_full );
    // cout<<"output_ero"<<endl<<output_ero<<endl;

    // Mat output_my_ero=Erosion( input, element_full );
    // cout<<"output_my_ero"<<endl<<output_my_ero<<endl;

    // Mat output_dila;
    // dilate( input, output_dila, element );
    // cout<<"output_dila"<<endl<<output_dila<<endl;

    // Mat output_dila_2;
    // dilate( src, output_dila_2, element );
    // cout<<"dilate"<<endl<<output_dila_2<<endl;

    // Mat output_dila_3;
    // output_dila_3=Dilation( src, element );
    // cout<<"Dilation"<<endl<<output_dila_3<<endl;

    // namedWindow( "test", CV_WINDOW_AUTOSIZE );
    // imshow ("test", output);

//-----------------------

    waitKey(0);
    return 0;
}

/**  @function Erosion  */
Mat Erosion (Mat src, Mat element)
{
    Mat dst(src.size(),CV_8UC1);

    int sz_row = (int)(element.rows / 2);
    int sz_col = (int)(element.cols / 2);

    int tam;
    if (element.at<uchar>(sz_row, sz_col) == 0)
        tam = 0;
    else
        tam = 255;

    int Fit=0;
    // đếm số bit 1 trong SE
    for (int m = 0; m < element.rows; ++m)
    {
        for (int n = 0; n < element.cols; ++n)
        {
            if(element.at<uchar>(m,n)==1)
                Fit++;
        }
    }
    // duyệt từng pixel src
    for (int i = 0; i < src.rows; ++i)
    {
        for (int j = 0; j < src.cols; ++j)
        {
                int trang_thai = 0;
                // duyệt từng pixel SE
                for (int m = 0; m < element.rows; ++m)
                {
                    bool Miss = false;// trạng thái lặp
                    for (int n = 0; n < element.cols; ++n)
                    {
                        
                        // bỏ qua biên src
                        // if (srcX >= src.rows || srcY >= src.cols || srcX < 0 || srcY < 0)
                        //     continue;
                        // else
                        // {
                            if (element.at<uchar>(m,n)==1)
                            {
                                //tọa độ điểm ảnh gốc ứng với SE tại (m,n)
                                int srcX = i + m - sz_row;
                                int srcY = j + n - sz_col;
                                // các điểm ngoài src luôn đúng
                                if (srcX >= src.rows || srcY >= src.cols || srcX < 0 || srcY < 0)
                                    trang_thai++;
                                else if (src.at<uchar>(srcX,srcY)==255)// hit
                                    trang_thai++;
                                else if (src.at<uchar>(srcX,srcY)==0)// miss
                                {
                                    Miss = true;
                                    break;
                                }
                            }
                        // }
                    }
                    if (Miss == true)
                        break;
                }

                if (trang_thai == Fit)
                    dst.at<uchar>(i,j) = 255;
                else
                    dst.at<uchar>(i,j) = 0;
        }
    }
    return dst;
}

/**  @function Dilation  */
Mat Dilation (Mat src, Mat element)
{
    Mat dst = src.clone();

    int sz_row = (int)(element.rows / 2);
    int sz_col = (int)(element.cols / 2);

    // duyệt từng pixel src
    for (int i = 0; i < src.rows; ++i)
    {
        for (int j = 0; j < src.cols; ++j)
        {
            if (src.at<uchar>(i,j)==0)
            {
                // duyệt từng pixel SE
                for (int m = 0; m < element.rows; ++m)
                {
                    bool Hit = false;// trạng thái lặp
                    for (int n = 0; n < element.cols; ++n)
                    {
                        //tọa độ điểm ảnh gốc ứng với SE tại (m,n)
                        int srcX = i + m - sz_row;
                        int srcY = j + n - sz_col;
                        // bỏ qua pixel ngoài biên src
                        if (srcX >= src.rows || srcY >= src.cols || srcX < 0 || srcY < 0)
                            continue;
                        else
                        {
                            if (element.at<uchar>(m,n)==1 && src.at<uchar>(srcX,srcY)==255)
                            {
                                // hit
                                dst.at<uchar>(i,j) = 255;
                                Hit = true;
                                break;
                            }
                        }
                    }
                    if (Hit == true)
                        break;
                }
            }
        }
    }

    return dst;
}

/** @function Opening */
Mat Opening (Mat src, Mat element)
{
    Mat dst_ero = Erosion (src, element);
    Mat dst = Dilation (dst_ero, element);
    return dst;
}

/** @function Opening */
Mat Closing (Mat src, Mat element)
{
    Mat dst_dila = Dilation (src, element);
    Mat dst = Erosion (dst_dila, element);
    return dst;
}

/** @function Boundary_extraction */
Mat Boundary_extraction (Mat src, Mat element)
{
    Mat dst_ero = Erosion (src, element);
    Mat dst = src - dst_ero;
    return dst;
}

/** @function Hit-or-miss Transform */
Mat HitOrMiss (Mat src)
{
    // int size = 1;
    // 2 structing element: w > B
    // Mat element_B = getStructuringElement( MORPH_CROSS, Size( 2*size + 1, 2*size+1 ), Point( size, size ) );
    // uchar bits[] = {0,0,0,1,1,1,0,0,0};
    // Mat element_B( 3,3, CV_8UC1, bits );
    // Mat element_w = getStructuringElement( MORPH_RECT, Size( 2*size + 1, 2*size+1 ), Point( size, size ) );

    // Mat element_wB = element_w - element_B;

    // int size = 11;
    // 2 structing element: w > B
    // Mat element_w = getStructuringElement( MORPH_CROSS, Size( 2*size + 1, 2*size+1 ), Point( size, size ) );

    Mat element_B(27, 27, CV_8UC1, Scalar::all(0));
    for (int i = 0; i < 27; i++)
    {
        for (int j = 0; j < 27; j++)
        {
            if ((i >= 1 && j >= 1) && (j <= 25 && i <= 25))
            {
                element_B.at<uchar>(i, j) = 1;
            }
        }
    }

    Mat element_wB(27, 27, CV_8UC1, Scalar::all(1));
    for (int i = 0; i < 27; i++)
    {
        for (int j = 0; j < 27; j++)
        {
            if ((i >= 1 && j >= 1) && (j <= 25 && i <= 25))
            {
                element_wB.at<uchar>(i, j) = 0;
            }
        }
    }

    // cout<<"element_B"<<endl<<element_B<<endl;
    // cout<<"element_w"<<endl<<element_w<<endl;
    // cout<<"element_wB"<<endl<<element_wB<<endl;
    // get negative src
    Mat Ac(src.size(), CV_8UC1);
    for (int i = 0; i < src.rows; ++i)
    {
        for (int j = 0; j < src.cols; ++j)
        {
            Ac.at<uchar>(i,j) = 255 - src.at<uchar>(i,j);
        }
    }
    // cal
    Mat A_ero_B = Erosion (src,element_B);
    Mat Ac_ero_wB = Erosion (Ac, element_wB);
    // giao 2 ảnh
    Mat dst(src.size(), CV_8UC1);
    for (int i = 0; i < src.rows; ++i)
    {
        for (int j = 0; j < src.cols; ++j)
        {
            if (A_ero_B.at<uchar>(i,j) == 255 && Ac_ero_wB.at<uchar>(i,j) == 255)
                dst.at<uchar>(i,j) = 255;
            else
                dst.at<uchar>(i,j) = 0;
        }
    }
    // test
    // Mat erosion_B;
    // erode( src, erosion_B, element_B );
    // Mat erosion_wB;
    // erode( erosion_B, erosion_wB, element_wB );

    // namedWindow( "erosion_B", CV_WINDOW_AUTOSIZE );
    // imshow ("erosion_B", erosion_B);
    // namedWindow( "erosion_wB", CV_WINDOW_AUTOSIZE );
    // imshow ("erosion_wB", erosion_wB);

    // namedWindow( "A_ero_B", CV_WINDOW_AUTOSIZE );
    // imshow ("A_ero_B", A_ero_B);

    // namedWindow( "Ac_ero_wB", CV_WINDOW_AUTOSIZE );
    // imshow ("Ac_ero_wB", Ac_ero_wB);

    return dst;
}