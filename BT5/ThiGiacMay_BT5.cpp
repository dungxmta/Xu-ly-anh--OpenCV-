// 4/11
// Bai3- Area process
// Edge detection - Xac dinh bien

#include <opencv2/opencv.hpp>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
#include <stdlib.h>//2 thu vien dung cho ham random
#include <time.h>
// #define PI 3.14
// const int M=7;
int low = 10;
int high = 168;

IplImage* img_gray=NULL;

void DisplayImage(char* tittle,IplImage* image)
{
    cvNamedWindow(tittle, CV_WINDOW_AUTOSIZE);// create a window
    cvShowImage(tittle, image);
}
// normal funcion with argument
void getThresholdValue_BGT(IplImage *img_in, float &threshold_value);
void getThresholdValue_Otsu(IplImage *img_in, float &threshold_value);
void Threshold(IplImage *img_in, IplImage *img_out, float threshold_value);
void Threshold_double(IplImage *img_in, IplImage *img_out, int low, int high);

void Sobel_core(IplImage *Gx, IplImage *Gy, IplImage *Mag, IplImage *Edge);
void show_Edge_Sobel(float threshold_value);

// callback funcion for Trackbar
void show_Edge_Sobel_Callback(int threshold_value, void *userData);
void show_Edge_Sobel_Callback_doubleThresh(int , void *);

//---------------------------------------------------------------
//-----------------------------------MAIN------------------------
int main()
{
    srand(time(NULL));//change value per time when use rand()
    // image path
    const char* imgPATH1 = "a_pic0.jpg";// paint img
    const char* imgPATH2 = "a_pic2.jpe";// building img
    const char* imgPATH3 = "a_pic3.jpg";// nature img
    const char* imgPATH4 = "a2.jpeg";
    const char* imgPATH5 = "a1.JPG";
    // IplImage* img = cvLoadImage(imgPATH);// source color image
    img_gray = cvLoadImage(imgPATH2, 0);// source gray image
    // IplImage* gray = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);
    
    DisplayImage("SourceImage", img_gray);
// normal funcion
    show_Edge_Sobel(100.0);
    // Threshold_double(img_gray, img_gray, 120, 156);
    // DisplayImage("test", img_gray);
 //----test Thresholding---------------  
    IplImage *clone_gray= cvCloneImage(img_gray);
    // get info of image
    uchar *data_clone=(uchar*)clone_gray->imageData;
    int height = clone_gray->height;
    int width = clone_gray->width;
    int step = clone_gray->widthStep;
    // img output
    IplImage *img_thresh = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);// đạo hàm theo x
    //--------------------Basic-Global-Thresholding
    float test;
    getThresholdValue_BGT(clone_gray, test);
    printf("=>Threshold_value by Basic-Global-Thresholding = %f\n", test);

    Threshold(img_gray, img_thresh, test);
    DisplayImage("=>test Thresh-BGT ", img_thresh);
    //--------------------Otsu-method-Thresholding
    getThresholdValue_Otsu(clone_gray, test);
    printf("\n=>Threshold_value by Otsu's method = %d\n", (int)test);

    Threshold(img_gray, img_thresh, test);
    DisplayImage("=>test Thresh-Otsu ", img_thresh);
//--------------------
//--------------------
// funcion use trackbar
    int threshold_value = 1000;
    // Bluring-trackbar
    cvNamedWindow("Edge_Sobel_trackbar",1);
    cv::createTrackbar("thresh/10", "Edge_Sobel_trackbar", &threshold_value, 2550, show_Edge_Sobel_Callback);
    show_Edge_Sobel_Callback(1000,0);
    // cvShowImage("Bluring_trackbar",img_gray);

    cvNamedWindow("Edge_doubleThresh_trackbar",1);
    cv::createTrackbar("low ", "Edge_doubleThresh_trackbar", &low, 255, show_Edge_Sobel_Callback_doubleThresh);

    cv::createTrackbar("high", "Edge_doubleThresh_trackbar", &high, 255, show_Edge_Sobel_Callback_doubleThresh);
    show_Edge_Sobel_Callback_doubleThresh(100,0);


// wait for a key
    cvWaitKey(0);
// release the img*
    cvReleaseImage(&img_gray);
    // cvReleaseImage(&img);
    // cvReleaseImage(&img_convolution);
    cvDestroyAllWindows();
    return 0;
}
//-----------------------------------END-MAIN--------------------
//---------------------------------------------------------------
// Threshold: phan nguong
// find threshold_value by Basic Global Thresholding method
void getThresholdValue_BGT(IplImage *img_in, float &threshold_value)
{
    uchar *data = (uchar*)img_in->imageData;
    // uchar *data = (uchar*)img_out->imageData;
    int height = img_in->height;
    int width = img_in->width;
    int step = img_in->widthStep;

    float T = (float)(rand() % 256);// T=(m1+m2)/2
    float T0 = 0;// save old T
        
    int arr[256];
    // gán giá trị khởi tạo
    for (int i = 0; i < 256; i++)
    {
        arr[i] = 0;
    }
    //đếm số lượng mỗi mức xám
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            arr[data[i*step + j]]++;
        }
    }
    do
    {
        //tổng số mức xám nhỏ hơn
        float mean1, mean2;
        int sum=0,sum1=0, sum2=0;

        for (int i = 0; i < T+0.5; i++)
        {
            sum1 = sum1 + arr[i];
            sum = sum + i*arr[i];
        }
        mean1 = (float)sum/(float)sum1;

        sum=0;
        for (int i = T+0.5; i < 256; ++i)
        {
            sum2 = sum2 + arr[i];
            sum = sum + i*arr[i];
        }
        mean2 = (float)sum/(float)sum2;
    
        T0 = T;
        T = (mean1 + mean2)/2;
        printf("\tT0=%f\tT=%f\n", T0, T);
    }
    while (fabs(T-T0) > powf(10,-8));

    threshold_value = T;
}
void getThresholdValue_Otsu(IplImage *img_in, float &threshold_value)
{
    uchar *data = (uchar*)img_in->imageData;
    // uchar *data = (uchar*)img_out->imageData;
    int height = img_in->height;
    int width = img_in->width;
    int step = img_in->widthStep;
// đếm số lượng mỗi mức xám
    int amount_grayLevel[256];
    for (int i = 0; i < 256; i++)
    {
        amount_grayLevel[i] = 0;
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            amount_grayLevel[data[i*step + j]]++;
        }
    }
// đếm pixels
    int pixels_Total = height*width;// tổng số pixel toàn ảnh
    int pixels_B = 0;// tổng pixel bên trái
    int pixels_F = 0;// tổng pixel bên phải
// tính tần xuất
    float sum_Total = 0;// tổng số lần xuất hiện của mx toàn ảnh
    for (int i = 0; i < 256; ++i)
    {
        sum_Total += i * amount_grayLevel[i];
    }
    float sum_B = 0;// tổng số lần xuất hiện của mx bên trái
    float sum_F = 0;// tổng số lần xuất hiện của mx bên phải
// max phuong sai
    float vB_max = 0;

// otsu
    for (int grayLevel = 0; grayLevel < 256; ++grayLevel)
    {
        // tính pixels
        pixels_B += amount_grayLevel[grayLevel];
        if (pixels_B == 0)
            continue;
        pixels_F = pixels_Total - pixels_B;
        if (pixels_F == 0)
            break;
    // weight
        float wB = (float)pixels_B/(float)pixels_Total;// weight Background
        float wF = 1.0 - wB;// weight Foreground
        // tính sum
        sum_B += grayLevel * amount_grayLevel[grayLevel];
        sum_F = sum_Total - sum_B;
    // mean
        float mB = sum_B/pixels_B;// mean Background
        float mF = sum_F/pixels_F;// mean Foreground
    // variance Background - phương sai
        float vB = wB*wF*(mB-mF)*(mB-mF);
    // find max variance
        if (vB > vB_max)
        {
            vB_max = vB;
            threshold_value = grayLevel;
        }
    }
}
void Threshold(IplImage *img_in, IplImage *img_out, float threshold_value)
{
    // get info of image
    uchar *data_in = (uchar*)img_in->imageData;
    uchar *data = (uchar*)img_out->imageData;
    int height = img_out->height;
    int width = img_out->width;
    int step = img_out->widthStep;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float temp = (float)data_in[i*step + j];
            if (temp > threshold_value)
            {
                data[i*step + j] = 255;
            }
            else
            {
                data[i*step + j] = 0; 
            }
        }
    }
    // printf("threshold_value = %f\n", threshold_value);
}
void Threshold_double(IplImage *img_in, IplImage *img_out, int low, int high)
{
    // get info of image
    uchar *data_in = (uchar*)img_in->imageData;
    uchar *data = (uchar*)img_out->imageData;
    int height = img_out->height;
    int width = img_out->width;
    int step = img_out->widthStep;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int temp = data_in[i*step + j];
            if (temp < low)
            {
                data[i*step + j] = 0;
            }
            else if (temp > low && temp < high)
            {
                data[i*step + j] = 128;
            }
            else
            {
                data[i*step + j] = 255; 
            }
        }
    }
}
// Edge detection - Sobel mask
void Sobel_core(IplImage *Gx, IplImage *Gy, IplImage *Mag, IplImage *Edge)
{
    IplImage *clone_gray= cvCloneImage(img_gray);
    // get info of image
    uchar *data_clone=(uchar*)clone_gray->imageData;
    int height = clone_gray->height;
    int width = clone_gray->width;
    int step = clone_gray->widthStep;
    // data img output
    uchar *data_Gx=(uchar*)Gx->imageData;
    uchar *data_Gy=(uchar*)Gy->imageData;
    uchar *data_Mag=(uchar*)Mag->imageData;
    uchar *data_Edge=(uchar*)Edge->imageData;
    // operation
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; ++j)
        {
            // process boder
            if (i==0 || j==0 || j==(width-1) || i==(height-1))
            {
                // ignore boder
                data_Gx[i*step+j] = data_clone[i*step+j];
                data_Gy[i*step+j] = data_clone[i*step+j];
                data_Mag[i*step+j] = data_clone[i*step+j];
                data_Edge[i*step+j] = data_clone[i*step+j];
                continue;
            }
            // convolution with mask X
            data_Gx[i*step+j] = abs(data_clone[(i-1)*step+(j-1)]
                                + data_clone[i*step+(j-1)]*2
                                + data_clone[(i+1)*step+(j-1)] 
                                - data_clone[(i-1)*step+(j+1)] 
                                - data_clone[i*step+(j+1)]*2
                                - data_clone[(i+1)*step+(j+1)]
                                );
            // convolution with mask Y
            data_Gy[i*step+j] = abs(- data_clone[(i-1)*step+(j-1)]
                                - data_clone[(i-1)*step+j]*2
                                - data_clone[(i-1)*step+(j+1)] 
                                + data_clone[(i+1)*step+(j-1)] 
                                + data_clone[(i+1)*step+j]*2
                                + data_clone[(i+1)*step+(j+1)]
                                );
            // magnitude of gradient
            data_Mag[i*step+j] = (int)sqrt(data_Gx[i*step+j] 
                                    * data_Gx[i*step+j] 
                                    + data_Gy[i*step+j] 
                                    * data_Gy[i*step+j]
                                    );
        }
    }
}
void show_Edge_Sobel(float threshold_value)
{
    IplImage *clone_gray= cvCloneImage(img_gray);
    // get info of image
    uchar *data_clone=(uchar*)clone_gray->imageData;
    int height = clone_gray->height;
    int width = clone_gray->width;
    int step = clone_gray->widthStep;
    // img output
    IplImage *Gx = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);// đạo hàm theo x
    IplImage *Gy = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);// đạo hàm theo y
    IplImage *Mag = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);// magtitude = sqrt(Gx^2*Gy^2) ~ |Gx|+|Gy|
    IplImage *Edge = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);// ảnh biên
    // operation
    Sobel_core(Gx, Gy, Mag, Edge);
    Threshold(Mag, Edge, threshold_value);
    // show image
    DisplayImage("Gx", Gx);
    cvReleaseImage(&Gx);
    DisplayImage("Gy", Gy);
    cvReleaseImage(&Gy);
    DisplayImage("Magnitude", Mag);
    cvReleaseImage(&Mag);
    DisplayImage("Edge", Edge);
    cvReleaseImage(&Edge);
}

//------------------------------------------------------------
//------------------------------------ funcion_callback from trackbar
void show_Edge_Sobel_Callback(int threshold_value, void *userData)
{

    float thresh = (float)threshold_value/10.0;
 
    IplImage *clone_gray= cvCloneImage(img_gray);
    // get info of image
    uchar *data_clone=(uchar*)clone_gray->imageData;
    int height = clone_gray->height;
    int width = clone_gray->width;
    int step = clone_gray->widthStep;
    // img output
    IplImage *Gx = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);// đạo hàm theo x
    IplImage *Gy = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);// đạo hàm theo y
    IplImage *Mag = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);// magtitude = sqrt(Gx^2*Gy^2) ~ |Gx|+|Gy|
    IplImage *Edge = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);// ảnh biên
    // operation
    Sobel_core(Gx, Gy, Mag, Edge);
    Threshold(Mag, Edge, thresh);
    // show image
    DisplayImage("Edge_Sobel_trackbar", Edge);
    cvReleaseImage(&Gx);
    cvReleaseImage(&Gy);
    cvReleaseImage(&Mag);
    cvReleaseImage(&Edge);
}
void show_Edge_Sobel_Callback_doubleThresh(int , void *)
{

    IplImage *clone_gray= cvCloneImage(img_gray);
    // get info of image
    uchar *data_clone=(uchar*)clone_gray->imageData;
    int height = clone_gray->height;
    int width = clone_gray->width;
    int step = clone_gray->widthStep;
    // img output
    IplImage *Gx = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);// đạo hàm theo x
    IplImage *Gy = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);// đạo hàm theo y
    IplImage *Mag = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);// magtitude = sqrt(Gx^2*Gy^2) ~ |Gx|+|Gy|
    IplImage *Edge = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);// ảnh biên
    // operation
    Sobel_core(Gx, Gy, Mag, Edge);
    Threshold_double(Mag, Edge, low, high);
    // show image
    DisplayImage("Edge_doubleThresh_trackbar", Edge);
    cvReleaseImage(&Gx);
    cvReleaseImage(&Gy);
    cvReleaseImage(&Mag);
    cvReleaseImage(&Edge);
}