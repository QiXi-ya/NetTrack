#pragma once
#include <Windows.h>
#include <graphics.h>
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>

class img
{
public:
    //StackBlur算法，高性能
    static void StackBlur(PIMAGE& img, int radius);
    // 近似高斯模糊：两次 StackBlur叠加
    static void BlurWithDownscale(PIMAGE& src, int radius, int scale = 2);

    // Windows 11风格模糊算法优化版
    static void AcrylicGaussianBlur(PIMAGE& pimg, int radius, double baseSigma = 2.0);

    static void BlurWithDownscale_Gaussian(PIMAGE& src, int radius = 5, double sigma = 2.0, int scale = 2);

    static void ZoomImage_Alpha(PIMAGE& pimg, int zoomWidth, int zoomHeight);

    // 旋转到指定角度，srcImg为原始图像，pimg为输出，6邻域加权平均
    static void rotateImage_alpha(PIMAGE& pimg, int angle, const PIMAGE srcImg);

    // 缩放壁纸至屏幕大小（保持宽高比）
    static void ScaleWallpaperToScreen();

    static void zoomImage(PIMAGE& pimg, int zoomWidth, int zoomHeight);
};