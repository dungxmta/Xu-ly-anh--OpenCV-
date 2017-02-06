// 16/9
// Bai4- Geometric processes
// zoom(scale), rotation

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

void resize_NearestNeighbor(float scale);
void resize_Bilinear(float scale);
// void GammaCorrection_Callback(int gamma, void *userData);
void Blur_Callback(int size, void *userData);

//---------------------------------------------------------------
//-----------------------------------MAIN------------------------
int main()
{
    const char* imgPATH = "a2.jpeg";
    const char* imgPATH1 = "a1.JPG";
    const char* imgPATH2 = "a4.bmp";
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
    
// normal funcion
    resize_NearestNeighbor(2);
    resize_Bilinear(2);
    
    DisplayImage("SourceImage", img);
    showGrayImage(img);

// funcion use trackbar
        
// wait for a key
    cvWaitKey(0);
// release the img*
    cvReleaseImage(&img_gray);
    cvReleaseImage(&img);
    // cvReleaseImage(&img_convolution);
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
// Convolution
void resize_NearestNeighbor(float scale)
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

    int newH = height*scale;
    int newW = width*scale;
    IplImage *img_out = cvCreateImage(cvSize(newW, newH), IPL_DEPTH_8U, 1);
    uchar *data_out=(uchar*)img_out->imageData;
    int newS = img_out->widthStep;

    for (int i = 0; i < newH; ++i)
    {
        for (int j = 0; j < newW; ++j)
        {
            float FloatX = i/scale;
            int X_src = int(FloatX + 0.5);
            float FloatY = j/scale;
            int Y_src = int(FloatY + 0.5);
            data_out[i*newS+j] = data_clone[X_src*step+Y_src];
        }
    }
    DisplayImage("resize.NearestNeighbor", img_out);
    cvReleaseImage(&img_out);
}

void resize_Bilinear(float scale)
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

    int newH = height*scale;
    int newW = width*scale;
    IplImage *img_out = cvCreateImage(cvSize(newW, newH), IPL_DEPTH_8U, 1);
    uchar *data_out=(uchar*)img_out->imageData;
    int newS = img_out->widthStep;

    for (int i = 0; i < newH; ++i)
    {
        for (int j = 0; j < newW; ++j)
        {
            float beta = (0.5)/scale;
            // int beta = FloatX + 0.;
            float alpha = (0.5)/scale;
            // int alpha = FloatY + 0.5;
            int X_src = (int)(i/scale);
            int Y_src = (int)(j/scale);

            data_out[i*newS+j] = (int)(
                (1-alpha)*(1-beta)*data_clone[X_src*step+Y_src] + 
                alpha*(1-beta)*data_clone[X_src*step+(Y_src+1)] + 
                (1-alpha)*beta*data_clone[(X_src+1)*step+Y_src] + 
                alpha*beta*data_clone[(X_src+1)*step+(Y_src+1)]
                );
        }
    }
    DisplayImage("resize.Bilinear", img_out);
    cvReleaseImage(&img_out);
}
//------------------------------------------------------------
//------------------------------------ funcion_callback from trackbar