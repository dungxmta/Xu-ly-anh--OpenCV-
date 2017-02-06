// 16/9
// Bai3- Area process
// Convolution - tich chap

#include <opencv2/opencv.hpp>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
#define PI 3.14
const int M=7;


IplImage* img_gray=NULL;

void DisplayImage(char* tittle,IplImage* image)
{
    cvNamedWindow(tittle, CV_WINDOW_AUTOSIZE);// create a window
    cvShowImage(tittle, image);
}
// default funcion with argument
void showGrayImage(IplImage *img);

void getMaskIndex(CvPoint maskIndex[], int size);
void getMaskBlur(float mask[], int size);
void getMaskSharpen(float mask[], int size, int alpha);
void getMaskGaussian(float mask[], CvPoint maskIndex[], int size, float sigma);

void Convolution(IplImage *img_out, float mask[], CvPoint maskIndex[], int size);
// callback funcion for Trackbar
void Blur_Callback(int size, void *userData);
void Sharpen_Callback_size(int size, void *userData);
void Sharpen_Callback_alpha(int alpha, void *userData);
void Gaussian_Callback_size(int size, void *userData);
void Gaussian_Callback_sigma(int sigma, void *userData);
//---------------------------------------------------------------
//-----------------------------------MAIN------------------------
int main()
{
    const char* imgPATH = "a2.jpeg";
    const char* imgPATH2 = "a1.JPG";
    IplImage* img = cvLoadImage(imgPATH);// source color image
    img_gray = cvLoadImage(imgPATH2, 0);// source gray image
    // // IplImage* gray = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);
    // if (!img)
    // {
    //     printf("Could not load image file: %s\n", imgPATH);
    //     exit(0);
    // }
    //data_img = (uchar *)img->imageData;
    // printf("Processing a %dx%d image with %d channels\n", height, width, channels);
    
// normal funcion
    
    // float mask[M*M];
    // // ma trận lưu chỉ số của mặt nạ ->3x3: (-1,-1),(-1,0),...(1,1)
    // CvPoint maskIndex[M*M];
    // getMaskIndex(maskIndex, M);
    // // bluring
    // IplImage *img_blur = cvCreateImage(cvSize(img_gray->width, img_gray->height), IPL_DEPTH_8U, 1);;
    // getMaskBlur(mask, M);
    // Convolution(img_blur, mask, maskIndex, M);
    // DisplayImage("Bluring", img_blur);
    // cvReleaseImage(&img_blur);
    // // gaussian smoothing
    // IplImage *img_gaussian = cvCreateImage(cvSize(img_gray->width, img_gray->height), IPL_DEPTH_8U, 1);
    // getMaskGaussian(mask, maskIndex, M, 1.0);
    // Convolution(img_gaussian, mask, maskIndex, M);
    // DisplayImage("Gaussian smoothing", img_gaussian);
    // cvReleaseImage(&img_gaussian);

    DisplayImage("SourceImage", img);
    showGrayImage(img);

// funcion use trackbar
    int size = 0;
    // Bluring-trackbar
    cvNamedWindow("Bluring_trackbar",1);
    cv::createTrackbar("2*size+1", "Bluring_trackbar", &size, 10, Blur_Callback);
    cvShowImage("Bluring_trackbar",img_gray);
    // Sharpening-trackbar
    int alpha = 0;
    cvNamedWindow("Shapening_trackbar",1);
    cv::createTrackbar("2*size + 1", "Shapening_trackbar", &size, 10, Sharpen_Callback_size, &alpha);
    cv::createTrackbar("9*alpha-1", "Shapening_trackbar", &alpha, 10, Sharpen_Callback_alpha, &size);
    cvShowImage("Shapening_trackbar",img_gray);
    // Gaussian-trackbar
    int sigma = 0;
    cvNamedWindow("GaussianSmoothing_trackbar",1);
    cv::createTrackbar("2*size+1", "GaussianSmoothing_trackbar", &size, 10, Gaussian_Callback_size, &sigma);
    cv::createTrackbar("sigma/10", "GaussianSmoothing_trackbar", &sigma, 100, Gaussian_Callback_sigma, &size);
    cvShowImage("GaussianSmoothing_trackbar",img_gray);
    
// wait for a key
    cvWaitKey(0);
// release the img*
    cvReleaseImage(&img_gray);
    cvReleaseImage(&img);
    // cvReleaseImage(&img_convolution);
    cvDestroyAllWindows();
    return 0;
}
//-----------------------------------END-MAIN--------------------
//---------------------------------------------------------------
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
// create matrix-point[size x size]
// ma trận lưu chỉ số ->3x3: (-1,-1),(-1,0),(-1,-1),(0,-1),(0,0)...(1,1)
// struct CvPoint (int x, int y)
void getMaskIndex(CvPoint maskIndex[], int size)
{
    printf("\nmaskIndex\n");
    int step = size/2;
    int valueX = -step;
    int valueY = -step;
    
    for (int i = 0; i < size*size; ++i)
    {
        maskIndex[i].x = valueX;
        maskIndex[i].y = valueY;

        valueY++;
        if(valueY > step)
        {
            valueY = -step;
            valueX++;
        }

        printf("[%2d].x=%2d - [%2d].y=%2d\n",i,maskIndex[i].x,i,maskIndex[i].y);
    }
}
// get mask[size x size]
void getMaskBlur(float mask[], int size)
{
    for (int i = 0; i < size*size; ++i)
    {
        mask[i] = 1.0/(size*size);
    }
    //print matrix
        printf("\nBlur mask [%dx%d]\n", size, size);
        for (int i = 0; i < size*size; ++i)
        {
            printf("%f",mask[i]);
            if((i+1)%size==0)
               printf("\n");
            else
               printf(" - ");
        }
}
void getMaskSharpen(float mask[], int size, int alpha)
{
    for (int i = 0; i < size*size; ++i)
    {
        mask[i] = -1.0/(size*size);
    }
    int center_point = size*size/2; 
    mask[center_point] = (float)alpha/(size*size);
    //print matrix
        printf("\nSharpen mask [%dx%d]\n", size, size);
        for (int i = 0; i < size*size; ++i)
        {
            printf("%2.3f",mask[i]);
            if((i+1)%size==0)
               printf("\n");
            else
               printf("   ");
        }
}
void getMaskGaussian(float mask[], CvPoint maskIndex[], int size, float sigma)
{
    float const_value = 1.0/(2.0*PI*sigma*sigma);
    for (int i = 0; i < size*size; ++i)
    {
        int valueX = maskIndex[i].x;
        int valueY = maskIndex[i].y;
        float sup_value = -1.0*(valueX*valueX+valueY*valueY)/(2.0*sigma*sigma);
        mask[i] = const_value * expf(sup_value);
    }
    //print matrix
        printf("\nGaussian mask [%dx%d] with sigma=%.2f \n", size, size, sigma);
        for (int i = 0; i < size*size; ++i)
        {
            printf("%2f",mask[i]);
            if((i+1)%size==0)
               printf("\n");
            else
               printf(" - ");
        }
}
// Convolution
void Convolution(IplImage *img_out, float mask[], CvPoint maskIndex[], int size)
{
    // CvSize size = cvSize(img_gray->width, img_gray->height)
    // IplImage *clone_gray = cvCreateImage(size, IPL_DEPTH_8U, 1);
    IplImage *clone_gray= cvCloneImage(img_gray);
    // img_out = cvCreateImage(cvSize(img_gray->width, img_gray->height), IPL_DEPTH_8U, 1);
    // get info of image
    uchar *data_clone=(uchar*)clone_gray->imageData;
    int height = clone_gray->height;
    int width = clone_gray->width;
    int step = clone_gray->widthStep;

    for (int i = 0; i < height; ++i)
    {
        uchar *data_out=(uchar*)img_out->imageData;

        for (int j = 0; j < width; ++j)
        {
            float sum_point = 0;// tổng giá trị của pixel tương ứng với mask
            // duyệt mask
            for (int k = 0; k < size*size; ++k)
            {
                // bỏ qua những pixel nằm ngoài
                int index_x = i - maskIndex[k].x;
                if(index_x < 0 || index_x > height)
                    continue;
                int index_y = j - maskIndex[k].y;
                if(index_y < 0 || index_y > width)
                    continue;

                sum_point = sum_point + mask[k]*(float)data_clone[index_x*step+index_y];
            }
            data_out[i*step+j] = sum_point;
        }
    }
}
//------------------------------------------------------------
//------------------------------------ funcion_callback from trackbar
void Blur_Callback(int size, void *userData)
{
    //size.output [0-3-5-7-9-11-13-15]
    if(size==0)
    {
        DisplayImage("Bluring_trackbar", img_gray);
        return;
    }
    int newsize = 2*size+1;
 
    IplImage *img_out = cvCreateImage(cvSize(img_gray->width, img_gray->height), IPL_DEPTH_8U, 1);

    float mask[newsize*newsize];
    getMaskBlur(mask, newsize);
    // ma trận lưu chỉ số của mặt nạ ->3x3: (-1,-1),(-1,0),...(1,1)
    CvPoint maskIndex[newsize*newsize];
    getMaskIndex(maskIndex, newsize);

    Convolution(img_out, mask, maskIndex, newsize);

    DisplayImage("Bluring_trackbar", img_out);
    cvReleaseImage(&img_out);
}

void Sharpen_Callback_size(int size, void *userData)
{
 /* 
 trackbar only use gamma.input "int" value [0-max] not "float"
 size.output [0-3-5-7-9-11-13-15]
 sigma.output [1-10]
 */
    int newsize = 2*size+1;
    // get alpha value
    int alpha = *( static_cast<int*>(userData));
    int newalpha = 9*alpha-1;
  
    if(size==0 || alpha==0)
    {
        DisplayImage("Shapening_trackbar", img_gray);
        return;
    }

    IplImage *img_out = cvCreateImage(cvSize(img_gray->width, img_gray->height), IPL_DEPTH_8U, 1);

    float mask[newsize*newsize];
    // ma trận lưu chỉ số của mặt nạ ->3x3: (-1,-1),(-1,0),...(1,1)
    CvPoint maskIndex[newsize*newsize];
    // Sharpening
    getMaskIndex(maskIndex, newsize);
    getMaskSharpen(mask, newsize, newalpha);
    Convolution(img_out, mask, maskIndex, newsize);

    DisplayImage("Shapening_trackbar", img_out);
    cvReleaseImage(&img_out);
}
void Sharpen_Callback_alpha(int alpha, void *userData)
{
 /* 
 trackbar only use gamma.input "int" value [0-max] not "float"
 size.output [3-5-7-9-11-13-15]
 sigma.output [1-10]
 */
    int newalpha = 9*alpha-1;
    // get size value
    int size = *( static_cast<int*>(userData));
    int newsize = 2*size+1;

    if(size==0 || alpha==0)
    {
        DisplayImage("Shapening_trackbar", img_gray);
        return;
    }

    IplImage *img_out = cvCreateImage(cvSize(img_gray->width, img_gray->height), IPL_DEPTH_8U, 1);

    float mask[newsize*newsize];
    // ma trận lưu chỉ số của mặt nạ ->3x3: (-1,-1),(-1,0),...(1,1)
    CvPoint maskIndex[newsize*newsize];
    // Sharpening
    getMaskIndex(maskIndex, newsize);
    getMaskSharpen(mask, newsize, newalpha);
    Convolution(img_out, mask, maskIndex, newsize);

    DisplayImage("Shapening_trackbar", img_out);
    cvReleaseImage(&img_out);
}

void Gaussian_Callback_size(int size, void *userData)
{
 /* 
 trackbar only use gamma.input "int" value [0-max] not "float"
 size.output [0-3-5-7-9-11-13-15]
 sigma.output [0.0-10.0]
 */
    int newsize = 2*size+1;
    // get sigma value
    int sigma = *( static_cast<int*>(userData));
    float newsigma = (float)sigma/10.0;
  
    if(size==0 || sigma==0)
    {
        DisplayImage("GaussianSmoothing_trackbar", img_gray);
        return;
    }

    IplImage *img_out = cvCreateImage(cvSize(img_gray->width, img_gray->height), IPL_DEPTH_8U, 1);

    float mask[newsize*newsize];
    // ma trận lưu chỉ số của mặt nạ ->3x3: (-1,-1),(-1,0),...(1,1)
    CvPoint maskIndex[newsize*newsize];
    // Gaussian smoothing
    getMaskIndex(maskIndex, newsize);
    getMaskGaussian(mask, maskIndex, newsize, newsigma);
    Convolution(img_out, mask, maskIndex, newsize);

    DisplayImage("GaussianSmoothing_trackbar", img_out);
    cvReleaseImage(&img_out);
}
void Gaussian_Callback_sigma(int sigma, void *userData)
{
 /* 
 trackbar only use gamma.input "int" value [0-max] not "float"
 size.output [0-3-5-7-9-11-13-15]
 sigma.output [0.0-10.0]
 */
    float newsigma = (float)sigma/10.0;
    // get size value
    int size = *( static_cast<int*>(userData));
    int newsize = 2*size+1;

    if(size==0 || sigma==0)
    {
        DisplayImage("GaussianSmoothing_trackbar", img_gray);
        return;
    }

    IplImage *img_out = cvCreateImage(cvSize(img_gray->width, img_gray->height), IPL_DEPTH_8U, 1);

    float mask[newsize*newsize];
    // ma trận lưu chỉ số của mặt nạ ->3x3: (-1,-1),(-1,0),...(1,1)
    CvPoint maskIndex[newsize*newsize];
    // Gaussian smoothing
    getMaskIndex(maskIndex, newsize);
    getMaskGaussian(mask, maskIndex, newsize, newsigma);
    Convolution(img_out, mask, maskIndex, newsize);

    DisplayImage("GaussianSmoothing_trackbar", img_out);
    cvReleaseImage(&img_out);
}