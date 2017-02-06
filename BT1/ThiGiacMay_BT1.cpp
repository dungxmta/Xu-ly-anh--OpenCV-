// OpenCV_ThiGiacMay.cpp : Defines the entry point for the console application.
//

#include <opencv2/opencv.hpp>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>

IplImage* img_gray=NULL;

void DisplayImage(char* tittle,IplImage* image)
{
    cvNamedWindow(tittle, CV_WINDOW_AUTOSIZE);// create a window
    cvShowImage(tittle, image);
}
// default funcion with argument
void showGrayImage(IplImage *img);
void HistogramEqualization_gray();

void mapping(IplImage *clone_gray, float H[]);
void HistogramSpecification_gray(const char *imgPATH2);

void EndInSearch(int high, int low);
void Negative();
void PowerLaw(int constant_value, double gamma);
void GammaCorrection(double gamma);
// funcion_callback from trackbar
void EndInSearch_Callback_high(int high, void *userData);
void EndInSearch_Callback_low(int low, void *userData);
void PowerLaw_Callback_gamma(int gamma, void *userData);
void PowerLaw_Callback_constant(int constant_value, void *userData);
void GammaCorrection_Callback(int gamma, void *userData);
//---------------------------------------------------------------------------------
//-----------------------------------MAIN------------------------------------------
int main()
{
    const char* imgPATH = "a2.jpeg";
    const char* imgPATH2 = "a1.JPG";
    IplImage* img = cvLoadImage(imgPATH);// source color image
    img_gray = cvLoadImage(imgPATH, 0);// source gray image
    // // IplImage* gray = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);
    // if (!img)
    // {
    //     printf("Could not load image file: %s\n", imgPATH);
    //     exit(0);
    // }
    //data_img = (uchar *)img->imageData;
    // printf("Processing a %dx%d image with %d channels\n", height, width, channels);
    DisplayImage("SourceImage", img);
// normal funcion
    showGrayImage(img);
    // GammaCorrection(0.5);// gamma
    // PowerLaw(1, 1.2);// constant_value, gamma (try in 0.5-1.5)
    Negative();
    // EndInSearch(245, 1);// high, low: 0-255
    HistogramSpecification_gray(imgPATH2);
    HistogramEqualization_gray();
    
// funcion use trackbar
    //------------End-in-search
    int high=150;
    int low=10;
    cvNamedWindow("End-in-search_trackbar",1);
    cv::createTrackbar("High", "End-in-search_trackbar", &high, 255, EndInSearch_Callback_high, &low);
    cv::createTrackbar("Low ", "End-in-search_trackbar", &low, 255, EndInSearch_Callback_low, &high);
    cvShowImage("End-in-search_trackbar",img_gray);
    //------------PowerLaw
    int gamma=5;
    int constant_value=10;
    cvNamedWindow("PowerLaw_trackbar",1);
    cv::createTrackbar("gamma / 10           ", "PowerLaw_trackbar", &gamma, 100, PowerLaw_Callback_gamma, &constant_value);
    cv::createTrackbar("constant value / 10", "PowerLaw_trackbar", &constant_value, 100, PowerLaw_Callback_constant, &gamma);
    cvShowImage("PowerLaw_trackbar",img_gray);
    //------------GammaCorrection
    cvNamedWindow("GammaCorrection_trackbar",1);
    cv::createTrackbar("gamma / 10", "GammaCorrection_trackbar", &gamma, 100, GammaCorrection_Callback);
    GammaCorrection_Callback(5,0);

// wait for a key
    cvWaitKey(0);
// release the img*
    cvReleaseImage(&img_gray);
    cvReleaseImage(&img);
    cvDestroyAllWindows();
    return 0;
}
//-----------------------------------END-MAIN--------------------------------------
//---------------------------------------------------------------------------------
void showGrayImage(IplImage *img)
{
    IplImage *clone_img=cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 1);
    //cvCreateImage(cvGetSize(img), img->depth, img->nChannels); //cvCreateImage(cv::Size(img->width, img->height), IPL_DEPTH_8U, 1);
    // get info of image
    uchar *data_clone=(uchar*)clone_img->imageData;
    uchar *data_img = (uchar *)img->imageData;
    int height = img->height;
    int width = img->width;
    int step = img->widthStep;
    int channels = img->nChannels;

    int _step = clone_img->widthStep;

    int r, g, b, gr;
    // invert the image
    for (int i = 0; i<height; i++)
    {
        for (int j = 0; j<width; j++)
        {
            b = data_img[i*step + j*channels + 0];
            g = data_img[i*step + j*channels + 1];
            r = data_img[i*step + j*channels + 2];

            gr = (r + g + b) / 3;

            data_clone[i*_step + j] = gr; 
            // data_clone[i*step + j*channels + 1] = gr; 
            // data_clone[i*step + j*channels + 2] = gr;
        }
    }
    DisplayImage("GrayImage", clone_img);
    cvReleaseImage(&clone_img);
}
// Cân bằng lược đồ mức xám
void HistogramEqualization_gray()
{
    IplImage *clone_gray=cvCloneImage(img_gray);//make clone of gray img
    // get info of image
    uchar *data_clone=(uchar*)clone_gray->imageData;
    int height = clone_gray->height;
    int width = clone_gray->width;
    int step = clone_gray->widthStep;

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
            arr[data_clone[i*step + j]]++;
        }
    }
    //tổng số mức xám nhỏ hơn
    for (int i = 1; i < 256; i++)
    {
        arr[i] = arr[i] + arr[i - 1];
    }
    //chuẩn hóa
    for (int i = 0; i < 256; i++)
    {
        arr[i] = (int)((float)arr[i] * 255 / (float)(height*width) + 0.5);
    }
    //cập nhật lại mức xám
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            data_clone[i*step + j] = arr[data_clone[i*step + j]];
            // data_clone[i*step + j*channels + 1] = arr[data_clone[i*step + j*channels + 1]];
            // data_clone[i*step + j*channels + 2] = arr[data_clone[i*step + j*channels + 2]];
        }
    }

    DisplayImage("HistogramEqualization", clone_gray);
    //cvNamedWindow("Histogram equalization", CV_WINDOW_AUTOSIZE);
    //cvShowImage("Histogram equalization", img_gray);
    cvReleaseImage(&clone_gray);
}
// Histogram Specification/ mapping/ chỉ định mức xám
void mapping(IplImage *clone_gray, float H[])//tính xác suất mỗi mức xám
{
    uchar *data_clone = (uchar*)clone_gray->imageData;
    int width = clone_gray->width;
    int height = clone_gray->height;
    int step = clone_gray->widthStep;
    // gán giá trị mặc định
    for (int i = 0; i < 256; ++i)
    {
        H[i]=0;
    }
    // đếm số lượng mỗi mx
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            H[data_clone[i*step+j]]++;
        }
    }
    // tính tổng
    for (int i = 1; i < 256; ++i)
    {
        H[i] = H[i] + H[i-1];
    }
    // xác suất
    for (int i = 0; i < 256; ++i)
    {
        H[i] = H[i] / (height*width);
    }
}
void HistogramSpecification_gray(const char *imgPATH2)
{
    IplImage *clone_mapping = cvLoadImage(imgPATH2,0);

    IplImage *clone_gray = cvCloneImage(img_gray);
    uchar *data_clone = (uchar*)clone_gray->imageData;
    int width = clone_gray->width;
    int height = clone_gray->height;
    int step = clone_gray->widthStep;
    // xác suất mỗi mx từng ảnh
    float Hx[256], Hz[256];
    mapping(clone_gray, Hx);
    mapping(clone_mapping, Hz);
    // look-up table: chỉ số là giá trị mx ảnh ban đầu, giá trị là giá trị mx mới
    int lookup[256];
    float min, temp;
    for (int i = 0; i < 256; ++i)
    {
        min = fabsf(Hx[i] - Hz[0]);
        for (int j = 0; j < 256; ++j)
        {
            if (i == j)
                continue;
            temp = fabsf(Hx[i] - Hz[j]);
            if (min >= temp)
            {
                min = temp;
                lookup[i] = j;
            }
        }
    }
    // cập nhật lại mx mới theo look-up table
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            data_clone[i*step+j] = lookup[data_clone[i*step+j]];
        }
    }

    DisplayImage("HistogramSpecification", clone_gray);
    cvReleaseImage(&clone_gray);
}
// End-in-search, [high,low]: 0-255
void EndInSearch(int high, int low)
{
    IplImage *clone_gray=cvCloneImage(img_gray);//make clone of gray img
    // get info of image
    uchar *data_clone=(uchar*)clone_gray->imageData;
    int height = clone_gray->height;
    int width = clone_gray->width;
    int step = clone_gray->widthStep;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (data_clone[i*step + j] <= low)
            {
                data_clone[i*step + j] = 0; 
                // data_clone[i*step + j*channels + 1] = 0; 
                // data_clone[i*step + j*channels + 2] = 0;
            }
            else if (data_clone[i*step + j] >= high)
            {
                data_clone[i*step + j] = 255; 
                // data_clone[i*step + j*channels + 1] = 255; 
                // data_clone[i*step + j*channels + 2] = 255;
            }
            else
            {
                data_clone[i*step + j] = 255 * (data_clone[i*step + j] - low) / (high - low);
                // data_clone[i*step + j*channels + 1] = 255 * (data_clone[i*step + j*channels + 1] - low) / (high - low);
                // data_clone[i*step + j*channels + 2] = 255 * (data_clone[i*step + j*channels + 2] - low) / (high - low);
            }
        }
    }
    DisplayImage("End-in-search", clone_gray);
    cvReleaseImage(&clone_gray);
}
// Âm bản
void Negative()
{
    IplImage *clone_gray=cvCloneImage(img_gray);//make clone of gray img
    // get info of image
    uchar *data_clone=(uchar*)clone_gray->imageData;
    int height = clone_gray->height;
    int width = clone_gray->width;
    int step = clone_gray->widthStep;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            data_clone[i*step + j] = 255 - data_clone[i*step + j];
            // data_clone[i*step + j*channels + 1] = 255 - data_clone[i*step + j*channels + 1];
            // data_clone[i*step + j*channels + 2] = 255 - data_clone[i*step + j*channels + 2];
        }
    }
    DisplayImage("Negative", clone_gray);
    cvReleaseImage(&clone_gray);
}
// Hàm mũ s=c*r^gamma, c: constant_value
void PowerLaw(int constant_value, double gamma)
{
    IplImage *clone_gray=cvCloneImage(img_gray);//make clone of gray img
    // get info of image
    uchar *data_clone=(uchar*)clone_gray->imageData;
    int height = clone_gray->height;
    int width = clone_gray->width;
    int step = clone_gray->widthStep;

    double temp;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp = (double)constant_value * pow((double)data_clone[i*step + j], gamma);
            //nếu ngoài khoảng [0,255]
            if ( temp > 255)
            {
                temp = 255;
            }
            else if (temp < 0)
            {
                temp = 0;
            }
            data_clone[i*step + j] = (uchar)temp;
            // data_clone[i*step + j*channels + 1] = (uchar)temp;
            // data_clone[i*step + j*channels + 2] = (uchar)temp;
        }
    }
    
    DisplayImage("PowerLaw", clone_gray);
    cvReleaseImage(&clone_gray);
}
// Gamma
void GammaCorrection(double gamma)
{
    IplImage *clone_gray=cvCloneImage(img_gray);//make clone of gray img
    // get info of image
    uchar *data_clone=(uchar*)clone_gray->imageData;
    int height = clone_gray->height;
    int width = clone_gray->width;
    int step = clone_gray->widthStep;

    double temp;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp = 255 * pow((double)data_clone[i*step + j] / 255, gamma);
            if (temp > 255)
            {
                temp = 255;
            }
            data_clone[i*step + j] = (uchar)temp;
            // data_clone[i*step + j*channels + 1] = (uchar)temp;
            // data_clone[i*step + j*channels + 2] = (uchar)temp;
        }
    }
    DisplayImage("GammaCorrection", clone_gray);
    cvReleaseImage(&clone_gray);
}
//---------------------------------------------------------------------------------------
//---------------------------------------------funcion_callback from trackbar
void EndInSearch_Callback_high(int high, void *userData)
{
    IplImage *clone_gray=cvCloneImage(img_gray);//make clone of gray img
    // get info of image
    uchar *data_clone=(uchar*)clone_gray->imageData;
    int height = clone_gray->height;
    int width = clone_gray->width;
    int step = clone_gray->widthStep;
    // get low value
    int low = *( static_cast<int*>(userData) );
    if (high<low)
        return;
    // end-in-search
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (data_clone[i*step + j] <= low)
            {
                data_clone[i*step + j] = 0; 
                // data_clone[i*step + j*channels + 1] = 0; 
                // data_clone[i*step + j*channels + 2] = 0;
            }
            else if (data_clone[i*step + j] >= high)
            {
                data_clone[i*step + j] = 255; 
                // data_clone[i*step + j*channels + 1] = 255; 
                // data_clone[i*step + j*channels + 2] = 255;
            }
            else
            {
                data_clone[i*step + j] = 255 * (data_clone[i*step + j] - low) / (high - low);
                // data_clone[i*step + j*channels + 1] = 255 * (data_clone[i*step + j*channels + 1] - low) / (high - low);
                // data_clone[i*step + j*channels + 2] = 255 * (data_clone[i*step + j*channels + 2] - low) / (high - low);
            }
        }
    }
    DisplayImage("End-in-search_trackbar", clone_gray);
    cvReleaseImage(&clone_gray);
}
void EndInSearch_Callback_low(int low, void *userData)
{
    IplImage *clone_gray=cvCloneImage(img_gray);//make clone of gray img
    // get info of image
    uchar *data_clone=(uchar*)clone_gray->imageData;
    int height = clone_gray->height;
    int width = clone_gray->width;
    int step = clone_gray->widthStep;
    // get high value
    int high = *( static_cast<int*>(userData) );
    if (high<low)
        return;
    // end-in-search
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (data_clone[i*step + j] <= low)
            {
                data_clone[i*step + j] = 0; 
                // data_clone[i*step + j*channels + 1] = 0; 
                // data_clone[i*step + j*channels + 2] = 0;
            }
            else if (data_clone[i*step + j] >= high)
            {
                data_clone[i*step + j] = 255; 
                // data_clone[i*step + j*channels + 1] = 255; 
                // data_clone[i*step + j*channels + 2] = 255;
            }
            else
            {
                data_clone[i*step + j] = 255 * (data_clone[i*step + j] - low) / (high - low);
                // data_clone[i*step + j*channels + 1] = 255 * (data_clone[i*step + j*channels + 1] - low) / (high - low);
                // data_clone[i*step + j*channels + 2] = 255 * (data_clone[i*step + j*channels + 2] - low) / (high - low);
            }
        }
    }
    DisplayImage("End-in-search_trackbar", clone_gray);
    cvReleaseImage(&clone_gray);
}
void PowerLaw_Callback_gamma(int gamma, void *userData)
{
    IplImage *clone_gray=cvCloneImage(img_gray);//make clone of gray img
    // get info of image
    uchar *data_clone=(uchar*)clone_gray->imageData;
    int height = clone_gray->height;
    int width = clone_gray->width;
    int step = clone_gray->widthStep;
    // get constant value
    int constant_value = *( static_cast<int*>(userData) );
    // int constant_value = *((int *)userdata)
    /* 
    trackbar only use gamma.input "int" value [0-max] not "double"
    gamma.output [0-10.0]
    constant value.output [0.0-10.0]
    */
    double gamma_temp = (double)gamma/10;
    double constant_temp = (double)constant_value/10;
    printf("Power law gamma = %.2f\n", gamma_temp);

    double temp;
    // power law
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp = constant_temp * pow((double)data_clone[i*step + j], gamma_temp);
            //out of [0,255]
            if ( temp > 255)
            {
                temp = 255;
            }
            else if (temp < 0)
            {
                temp = 0;
            }
            data_clone[i*step + j] = (uchar)temp;
            // data_clone[i*step + j*channels + 1] = (uchar)temp;
            // data_clone[i*step + j*channels + 2] = (uchar)temp;
        }
    }
    cvShowImage("PowerLaw_trackbar", clone_gray);
    cvReleaseImage(&clone_gray);
}
void PowerLaw_Callback_constant(int constant_value, void *userData)
{
    IplImage *clone_gray=cvCloneImage(img_gray);//make clone of gray img
    // get info of image
    uchar *data_clone=(uchar*)clone_gray->imageData;
    int height = clone_gray->height;
    int width = clone_gray->width;
    int step = clone_gray->widthStep;
    // get gamma value
    int gamma = *( static_cast<int*>(userData) );
    // int gamma = *((int *)userdata)
    /* 
    trackbar only use gamma.input "int" value [0-max] not "double"
    gamma.output [0-10.0]
    constant value.output [0.0-10.0]
    */
    double gamma_temp = (double)gamma/10;
    double constant_temp = (double)constant_value/10;
    // printf("Power law gamma = %.2f\n", gamma_temp);

    double temp;
    // power law
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp = constant_temp * pow((double)data_clone[i*step + j], gamma_temp);
            //out of [0,255]
            if ( temp > 255)
            {
                temp = 255;
            }
            else if (temp < 0)
            {
                temp = 0;
            }
            data_clone[i*step + j] = (uchar)temp;
            // data_clone[i*step + j*channels + 1] = (uchar)temp;
            // data_clone[i*step + j*channels + 2] = (uchar)temp;
        }
    }
    cvShowImage("PowerLaw_trackbar", clone_gray);
    cvReleaseImage(&clone_gray);
}
void GammaCorrection_Callback(int gamma, void *userData)
{
    IplImage *clone_gray=cvCloneImage(img_gray);//make clone of gray img
    // get info of image
    uchar *data_clone=(uchar*)clone_gray->imageData;
    int height = clone_gray->height;
    int width = clone_gray->width;
    int step = clone_gray->widthStep;
    // int constant_value = *( static_cast<int*>(userData) );
    /* 
    trackbar only use gamma.input "int" value [0-max] not "double"
    gamma.output [0-10.0]
    */
    double gamma_temp = (double)gamma/10;
    printf("Gamma correction - gamma = %f\n", gamma_temp);

    double temp;
    // gamma correction
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp = 255 * pow(((double)data_clone[i*step + j] / 255), gamma_temp);
            if (temp > 255)
            {
                temp = 255;
            }
            data_clone[i*step + j] = (uchar)temp;
            // data_clone[i*step + j*channels + 1] = (uchar)temp;
            // data_clone[i*step + j*channels + 2] = (uchar)temp;
        }
    }
    DisplayImage("GammaCorrection_trackbar", clone_gray);
    cvReleaseImage(&clone_gray);
}