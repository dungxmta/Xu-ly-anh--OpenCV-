#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "highgui.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace cv;

Mat Threshold(Mat img_in, float threshold_value)
{
    Mat img_out(img_in.size(), CV_8UC1);
    int height = img_in.rows;
    int width = img_out.cols;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float temp = (float)img_in.at<uchar>(i,j);
            if (temp > threshold_value)
                img_out.at<uchar>(i,j) = 255;
            else
                img_out.at<uchar>(i,j) = 0; 
        }
    }
    // printf("threshold_value = %f\n", threshold_value);
    return img_out;
}

// find Threshold value by Otsu's method
void getThresholdValue_Otsu(Mat img_in, float &threshold_value)
{
    int height = img_in.rows;
    int width = img_in.cols;
    // đếm số lượng mỗi mức xám
    int amount_grayLevel[256];
    for (int i = 0; i < 256; i++)
        amount_grayLevel[i] = 0;

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            amount_grayLevel[img_in.at<uchar>(i,j)]++;
    // đếm pixels
    int pixels_Total = height*width;// tổng số pixel toàn ảnh
    int pixels_B = 0;// tổng pixel bên trái
    int pixels_F = 0;// tổng pixel bên phải
    // tính tần xuất
    float sum_Total = 0;// tổng số lần xuất hiện của mx toàn ảnh
    for (int i = 0; i < 256; ++i)
        sum_Total += i * amount_grayLevel[i];

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