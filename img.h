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
    //StackBlur�㷨��������
    static void StackBlur(PIMAGE& img, int radius);
    // ���Ƹ�˹ģ�������� StackBlur����
    static void BlurWithDownscale(PIMAGE& src, int radius, int scale = 2);

    // Windows 11���ģ���㷨�Ż���
    static void AcrylicGaussianBlur(PIMAGE& pimg, int radius, double baseSigma = 2.0);

    static void BlurWithDownscale_Gaussian(PIMAGE& src, int radius = 5, double sigma = 2.0, int scale = 2);

    static void ZoomImage_Alpha(PIMAGE& pimg, int zoomWidth, int zoomHeight);

    // ��ת��ָ���Ƕȣ�srcImgΪԭʼͼ��pimgΪ�����6�����Ȩƽ��
    static void rotateImage_alpha(PIMAGE& pimg, int angle, const PIMAGE srcImg);

    // ���ű�ֽ����Ļ��С�����ֿ�߱ȣ�
    static void ScaleWallpaperToScreen();

    static void zoomImage(PIMAGE& pimg, int zoomWidth, int zoomHeight);
};