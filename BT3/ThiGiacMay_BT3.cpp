// 16/9
// Bai3- Area process
// add Noise: Salt&Pepper noise, Gaussian white noise

#include <opencv2/opencv.hpp>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
#include <stdlib.h>//2 thu vien dung cho ham random
#include <time.h>
#define PI 3.14
const int M=7;


IplImage* img_gray=NULL;

void DisplayImage(char* tittle,IplImage* image)
{
    cvNamedWindow(tittle, CV_WINDOW_AUTOSIZE);// create a window
    cvShowImage(tittle, image);
}
// default funcion with argument
void showNoise(IplImage *img_noise, char nameWindow[]);
void showGrayImage(IplImage *img);
void addNoiseSaltPepper(IplImage *img_out, int black, int white);
    // float rand_gauss();
void addNoiseGaussian(IplImage *img_out, float mu, float sigma);
// callback funcion for Trackbar
void addNoiseSaltPepper_Callback_black(int white, void *userData);
void addNoiseSaltPepper_Callback_white(int black, void *userData);
void addNoiseGaussian_Callback_mu(int mu, void *userData);
void addNoiseGaussian_Callback_sigma(int sigma, void *userData);
//---------------------------------------------------------------
//-----------------------------------MAIN------------------------
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

/* test Gaussian pdf -> ~0.2-0.3
    float sigma=1;// double [0->1]
    float mu=0.03;
    float const_value = 1.0/(sqrt(2.0*PI)*sigma);
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            float rd = rand_gauss();
            printf("\nrd_func[%d][%d]=%f\n",i,j,rd);

            float value = (float)rand()/RAND_MAX;
            printf("\nra[%d][%d]=%f\n",i,j,value);
            float sup_value = -1.0*(value-mu)*(value-mu)/(2.0*sigma*sigma);
            printf("sup_val[%d][%d]=%f\n",i,j,sup_value);
            printf("Gaussian[%d][%d]=%f\n",i,j,const_value*expf(sup_value));
        }
    }
*/

// normal funcion
    srand(time(NULL));//dung cho ham random

    IplImage *img_noiseSP = cvCreateImage(cvSize(img_gray->width, img_gray->height), IPL_DEPTH_8U, 1);
    // salt and pepper noise
    addNoiseSaltPepper(img_noiseSP, 3, 253);// black < white
    DisplayImage("Noise Salt & pepper", img_noiseSP);
    showNoise(img_noiseSP, "only SP noise: g-f");
    cvReleaseImage(&img_noiseSP);
    // gaussian noise
    IplImage *img_noiseGauss = cvCreateImage(cvSize(img_gray->width, img_gray->height), IPL_DEPTH_8U, 1);
    addNoiseGaussian(img_noiseGauss, 0, 0.03);// [0.0-1]
    DisplayImage("Noise Gaussian white", img_noiseGauss);
    showNoise(img_noiseGauss, "only Gauss noise: g-f");
    cvReleaseImage(&img_noiseGauss);

    DisplayImage("SourceImage", img);
    showGrayImage(img);

// funcion use trackbar
    // salt and pepper trackbar
    int black = 3;
    int white = 252;
    cvNamedWindow("noiseSaltPepper_trackbar",1);
    cv::createTrackbar("black", "noiseSaltPepper_trackbar", &black, 255, addNoiseSaltPepper_Callback_black, &white);
    cv::createTrackbar("white", "noiseSaltPepper_trackbar", &white, 255, addNoiseSaltPepper_Callback_white, &black);
    addNoiseSaltPepper_Callback_black(black, &white);// update first stage
    // gauss trackbar
    int mu = 0;
    int sigma = 1;
    cvNamedWindow("noiseGaussian_trackbar",1);
    cv::createTrackbar("mu/100    ", "noiseGaussian_trackbar", &mu, 100, addNoiseGaussian_Callback_mu, &sigma);
    cv::createTrackbar("sigma/100", "noiseGaussian_trackbar", &sigma, 100, addNoiseGaussian_Callback_sigma, &mu);
    addNoiseGaussian_Callback_mu(mu, &sigma);// update first stage
// wait for a key
    cvWaitKey(0);
// release the img*
    cvReleaseImage(&img_gray);
    cvReleaseImage(&img);
    // cvReleaseImage(&img_convolution);
    cvDestroyAllWindows();
    return 0;
}
//-----------------------------------END-MAIN---------------------
//---------------------------------------------------------------
void showNoise(IplImage *img_noise, char nameWindow[])
{
    IplImage *clone_gray= cvCloneImage(img_gray);
    IplImage *img_show=cvCreateImage(cvSize(clone_gray->width, clone_gray->height), IPL_DEPTH_8U, 1);
    
    // get info of image
    uchar *data_noise = (uchar *)img_noise->imageData;
    uchar *data_gray=(uchar*)clone_gray->imageData;
    uchar *data_show=(uchar*)img_show->imageData;

    int height = clone_gray->height;
    int width = clone_gray->width;
    int step = clone_gray->widthStep;

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            data_show[i*step+j] = data_noise[i*step+j] - data_gray[i*step+j];
        }
    }
    DisplayImage(nameWindow, img_show);
}
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
            
        }
    }
    DisplayImage("GrayImage", clone_img);
    cvReleaseImage(&clone_img);
}
// salt and pepper noise
void addNoiseSaltPepper(IplImage *img_out, int black, int white)
{
    // CvSize size = cvSize(img_gray->width, img_gray->height)
    // IplImage *clone_gray = cvCreateImage(size, IPL_DEPTH_8U, 1);
    IplImage *clone_gray= cvCloneImage(img_gray);
    // img_out = cvCreateImage(cvSize(img_gray->width, img_gray->height), IPL_DEPTH_8U, 1);
    // get info of image
    uchar *data_out=(uchar*)img_out->imageData;
    uchar *data_clone=(uchar*)clone_gray->imageData;
    int height = clone_gray->height;
    int width = clone_gray->width;
    int step = clone_gray->widthStep;

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            // random [0-255]
            int temp = rand() % 256;
            if(temp <= black)// black point (pepper)
                data_out[i*step+j] = 0;
            else if(temp >= white)// white point (salt)
                data_out[i*step+j] = 255;
            else
                data_out[i*step+j] = data_clone[i*step+j];
        }
    }
    /* // from MathLab
    double probability = 0.03;// xac suat
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            double temp = (double)rand()/RAND_MAX;
            if(temp <= probability/2.0)// black point (pepper)
                data_out[i*step+j] = 0;
            else if(temp >= probability/2.0 & temp < probability)// white point (salt)
                data_out[i*step+j] = 255;
            else
                data_out[i*step+j] = data_clone[i*step+j];
        }
    }
    */
}
// Gaussian noise
// float rand_gauss()
// {
//     float s, x, y;
//     while(1)
//     {
//         x = 2.0 * ((float) rand()/RAND_MAX) - 1;
//         y = 2.0 * ((float) rand()/RAND_MAX) - 1;
//         s = x*x + y*y;
//         if(s < 1.0) 
//             break;
//     }
//     if (s == 0.0)
//         return 0.0;
//     else
//         return (x*sqrt(-2.0 * log(s) / s));
// }
void addNoiseGaussian(IplImage *img_out, float mu=0, float sigma=0.01)
{
    // CvSize size = cvSize(img_gray->width, img_gray->height)
    // IplImage *clone_gray = cvCreateImage(size, IPL_DEPTH_8U, 1);
    IplImage *clone_gray= cvCloneImage(img_gray);
    // get info of image
    uchar *data_out=(uchar*)img_out->imageData;
    uchar *data_clone=(uchar*)clone_gray->imageData;
    int height = clone_gray->height;
    int width = clone_gray->width;
    int step = clone_gray->widthStep;

    // float const_value = 1.0/(sqrt(2.0*PI)*sigma);
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            // rd[-128,128]
            float rd_value = (float)(rand() % 256 - 128);
            // float sup_value = -1.0*((float)value-mu)*((float)value-mu)/(2.0*sigma*sigma);
            float gauss_value = (float)data_clone[i*step+j] + sqrt(sigma)*rd_value + mu;
            if(gauss_value > 255 || gauss_value < 0)
                data_out[i*step+j] = data_clone[i*step+j];
            else
                data_out[i*step+j] = gauss_value;
            //(const_value*expf(sup_value));
            // printf("Gaussian[%d][%d]=%f\n",i,j,const_value*expf(sup_value));
        }
    }
}
//----------------------------------callback-funcion for Trackbar
// callback - Salt and pepper noise
void addNoiseSaltPepper_Callback_black(int black, void *userData)
{  
    // get white value
    int white = *( static_cast<int*>(userData));

    if(black > white)
        return;

    IplImage *clone_gray= cvCloneImage(img_gray);
    IplImage *img_out = cvCreateImage(cvSize(img_gray->width, img_gray->height), IPL_DEPTH_8U, 1);
    // get info of image
    uchar *data_out=(uchar*)img_out->imageData;
    uchar *data_clone=(uchar*)clone_gray->imageData;
    int height = clone_gray->height;
    int width = clone_gray->width;
    int step = clone_gray->widthStep;

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            // random [0-255]
            int temp = rand() % 256;
            if(temp <= black)// black point (pepper)
                data_out[i*step+j] = 0;
            else if(temp >= white)// white point (salt)
                data_out[i*step+j] = 255;
            else
                data_out[i*step+j] = data_clone[i*step+j];
        }
    }
    DisplayImage("noiseSaltPepper_trackbar", img_out);
    cvReleaseImage(&img_out);
}
void addNoiseSaltPepper_Callback_white(int white, void *userData)
{  
    // get black value
    int black = *( static_cast<int*>(userData));

    if(black > white)
        return;

    IplImage *clone_gray= cvCloneImage(img_gray);
    IplImage *img_out = cvCreateImage(cvSize(img_gray->width, img_gray->height), IPL_DEPTH_8U, 1);
    // get info of image
    uchar *data_out=(uchar*)img_out->imageData;
    uchar *data_clone=(uchar*)clone_gray->imageData;
    int height = clone_gray->height;
    int width = clone_gray->width;
    int step = clone_gray->widthStep;

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            // random [0-255]
            int temp = rand() % 256;
            if(temp <= black)// black point (pepper)
                data_out[i*step+j] = 0;
            else if(temp >= white)// white point (salt)
                data_out[i*step+j] = 255;
            else
                data_out[i*step+j] = data_clone[i*step+j];
        }
    }
    DisplayImage("noiseSaltPepper_trackbar", img_out);
    cvReleaseImage(&img_out);
}
// callback - Gaussian noise
void addNoiseGaussian_Callback_mu(int mu, void *userData)
{
    // get sigma value
    int sigma = *( static_cast<int*>(userData));
    if(sigma==0)
        return;
    float newsigma = (float)sigma/100.0;
    float newmu = (float)mu/100.0;

    IplImage *clone_gray= cvCloneImage(img_gray);
    IplImage *img_out = cvCreateImage(cvSize(img_gray->width, img_gray->height), IPL_DEPTH_8U, 1);
    // get info of image
    uchar *data_out=(uchar*)img_out->imageData;
    uchar *data_clone=(uchar*)clone_gray->imageData;
    int height = clone_gray->height;
    int width = clone_gray->width;
    int step = clone_gray->widthStep;

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            // rd[-128,128]
            float rd_value = (float)(rand() % 256 - 128);

            float gauss_value = (float)data_clone[i*step+j] + sqrt(newsigma)*rd_value + newmu;
            if(gauss_value > 255 || gauss_value < 0)
                data_out[i*step+j] = data_clone[i*step+j];
            else
                data_out[i*step+j] = gauss_value;
        }
    }
    DisplayImage("noiseGaussian_trackbar", img_out);
    cvReleaseImage(&img_out);
}
void addNoiseGaussian_Callback_sigma(int sigma, void *userData)
{
    if(sigma==0)
        return;
    // get mu value
    int mu = *( static_cast<int*>(userData));
    float newmu = (float)mu/100.0;
    float newsigma = (float)sigma/100.0;
    
    IplImage *clone_gray= cvCloneImage(img_gray);
    IplImage *img_out = cvCreateImage(cvSize(img_gray->width, img_gray->height), IPL_DEPTH_8U, 1);
    // get info of image
    uchar *data_out=(uchar*)img_out->imageData;
    uchar *data_clone=(uchar*)clone_gray->imageData;
    int height = clone_gray->height;
    int width = clone_gray->width;
    int step = clone_gray->widthStep;

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            // rd[-128,128]
            float rd_value = (float)(rand() % 256 - 128);

            float gauss_value = (float)data_clone[i*step+j] + sqrt(newsigma)*rd_value + newmu;
            if(gauss_value > 255 || gauss_value < 0)
                data_out[i*step+j] = data_clone[i*step+j];
            else
                data_out[i*step+j] = gauss_value;
        }
    }
    DisplayImage("noiseGaussian_trackbar", img_out);
    cvReleaseImage(&img_out);
}