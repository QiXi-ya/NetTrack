#define  _CRT_SECURE_NO_WARNINGS
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include <graphics.h>
#include <vector>
#include <string>
#include <iostream>
#include <cstddef>
#include <Windows.h>
#include <array>
#include "img.h"
#include "Main.h"
#include "Tool.h"

    //StackBlur算法，高性能
    void img::StackBlur(PIMAGE& img, int radius)
    {
        if (!img || radius < 1) return;
        int w = getwidth(img);
        int h = getheight(img);

        int div = radius * 2 + 1;
        std::vector<int> vmin(max(w, h));
        std::vector<int> r(w * h), g(w * h), b(w * h);

        // 读取像素到数组
        for (int y = 0; y < h; ++y)
            for (int x = 0; x < w; ++x) {
                color_t c = getpixel(x, y, img);
                int idx = y * w + x;
                r[idx] = EGEGET_R(c);
                g[idx] = EGEGET_G(c);
                b[idx] = EGEGET_B(c);
            }

        // 水平
        for (int y = 0; y < h; ++y) {
            int sum_r = 0, sum_g = 0, sum_b = 0;
            for (int i = -radius; i <= radius; ++i) {
                int x = std::clamp(i, 0, w - 1);
                int idx = y * w + x;
                sum_r += r[idx];
                sum_g += g[idx];
                sum_b += b[idx];
            }
            for (int x = 0; x < w; ++x) {
                int idx = y * w + x;
                int out_idx = y * w + x;
                r[out_idx] = sum_r / div;
                g[out_idx] = sum_g / div;
                b[out_idx] = sum_b / div;

                int x1 = std::clamp(x - radius, 0, w - 1);
                int x2 = std::clamp(x + radius + 1, 0, w - 1);
                sum_r += r[y * w + x2] - r[y * w + x1];
                sum_g += g[y * w + x2] - g[y * w + x1];
                sum_b += b[y * w + x2] - b[y * w + x1];
            }
        }

        // 垂直
        for (int x = 0; x < w; ++x) {
            int sum_r = 0, sum_g = 0, sum_b = 0;
            for (int i = -radius; i <= radius; ++i) {
                int y = std::clamp(i, 0, h - 1);
                int idx = y * w + x;
                sum_r += r[idx];
                sum_g += g[idx];
                sum_b += b[idx];
            }
            for (int y = 0; y < h; ++y) {
                int idx = y * w + x;
                putpixel(x, y, EGERGB(sum_r / div, sum_g / div, sum_b / div), img);

                int y1 = std::clamp(y - radius, 0, h - 1);
                int y2 = std::clamp(y + radius + 1, 0, h - 1);
                sum_r += r[y2 * w + x] - r[y1 * w + x];
                sum_g += g[y2 * w + x] - g[y1 * w + x];
                sum_b += b[y2 * w + x] - b[y1 * w + x];
            }
        }
    }
    // 近似高斯模糊：两次 StackBlur叠加
    void img::BlurWithDownscale(PIMAGE& src, int radius, int scale)
    {
        int w = getwidth(src), h = getheight(src);
        int sw = w / scale, sh = h / scale;
        PIMAGE small_pmg = newimage(sw, sh);
        // 缩小
        for (int y = 0; y < sh; ++y)
            for (int x = 0; x < sw; ++x)
                putpixel(x, y, getpixel(min(x * scale, w - 1), min(y * scale, h - 1), src), small_pmg);
        // 模糊
        StackBlur(small_pmg, radius);
        // 放大
        for (int y = 0; y < h; ++y)
            for (int x = 0; x < w; ++x)
                putpixel(x, y, getpixel(min(x / scale, sw - 1), min(y / scale, sh - 1), small_pmg), src);
        delimage(small_pmg);
    }

    // Windows 11风格模糊算法优化版
    void img::AcrylicGaussianBlur(PIMAGE& pimg, int radius, double baseSigma) {
        if (!pimg || radius <= 0) return;
        int w = getwidth(pimg), h = getheight(pimg);
        int scale = 2; // 降采样倍数，可调
        int sw = w / scale, sh = h / scale;

        // 1. 降采样
        PIMAGE small_img = newimage(sw, sh);
        for (int y = 0; y < sh; ++y)
            for (int x = 0; x < sw; ++x)
                putpixel(x, y, getpixel(min(x * scale, w - 1), min(y * scale, h - 1), pimg), small_img);

        // 2. 生成一维高斯核
        double sigma = baseSigma * (tool::IsWindows11OrGreater() ? 1.3 : 1.0);
        int kernelSize = radius * 2 + 1;
        std::vector<double> kernel(kernelSize);
        double sum = 0;
        for (int i = 0; i < kernelSize; ++i) {
            int x = i - radius;
            kernel[i] = exp(-(x * x) / (2 * sigma * sigma));
            sum += kernel[i];
        }
        for (auto& v : kernel) v /= sum;

        // 3. 水平模糊
        std::vector<COLORREF> row(sw);
        PIMAGE temp = newimage(sw, sh);
#pragma omp parallel for private(row)
        for (int y = 0; y < sh; ++y) {
            for (int x = 0; x < sw; ++x) row[x] = getpixel(x, y, small_img);
            for (int x = 0; x < sw; ++x) {
                double r = 0, g = 0, b = 0, a = 0;
                for (int k = -radius; k <= radius; ++k) {
                    int ix = std::clamp(x + k, 0, sw - 1);
                    COLORREF c = row[ix];
                    double w = kernel[k + radius];
                    r += EGEGET_R(c) * w;
                    g += EGEGET_G(c) * w;
                    b += EGEGET_B(c) * w;
                    a += EGEGET_A(c) * w;
                }
                putpixel(x, y, EGERGBA((int)r, (int)g, (int)b, (int)a), temp);
            }
        }

        // 4. 垂直模糊
        std::vector<COLORREF> col(sh);
#pragma omp parallel for private(col)
        for (int x = 0; x < sw; ++x) {
            for (int y = 0; y < sh; ++y) col[y] = getpixel(x, y, temp);
            for (int y = 0; y < sh; ++y) {
                double r = 0, g = 0, b = 0, a = 0;
                for (int k = -radius; k <= radius; ++k) {
                    int iy = std::clamp(y + k, 0, sh - 1);
                    COLORREF c = col[iy];
                    double w = kernel[k + radius];
                    r += EGEGET_R(c) * w;
                    g += EGEGET_G(c) * w;
                    b += EGEGET_B(c) * w;
                    a += EGEGET_A(c) * w;
                }
                putpixel(x, y, EGERGBA((int)r, (int)g, (int)b, (int)a), small_img);
            }
        }
        delimage(temp);

        // 5. 上采样回原图
#pragma omp parallel for
        for (int y = 0; y < h; ++y)
            for (int x = 0; x < w; ++x)
                putpixel(x, y, getpixel(min(x / scale, sw - 1), min(y / scale, sh - 1), small_img), pimg);

        delimage(small_img);
    }

    void img::BlurWithDownscale_Gaussian(PIMAGE& src, int radius, double sigma, int scale)
    {
        int w = getwidth(src), h = getheight(src);
        int sw = w / scale, sh = h / scale;
        PIMAGE small_pmg = newimage(sw, sh);
        // 缩小
        for (int y = 0; y < sh; ++y)
            for (int x = 0; x < sw; ++x)
                putpixel(x, y, getpixel(min(x * scale, w - 1), min(y * scale, h - 1), src), small_pmg);
        // 模糊
        AcrylicGaussianBlur(small_pmg, radius, sigma);
        // 放大
        for (int y = 0; y < h; ++y)
            for (int x = 0; x < w; ++x)
                putpixel(x, y, getpixel(min(x / scale, sw - 1), min(y / scale, sh - 1), small_pmg), src);
        delimage(small_pmg);
    }

    void img::ZoomImage_Alpha(PIMAGE& pimg, int zoomWidth, int zoomHeight)
    {
        if (pimg == NULL) return;

        int srcWidth = getwidth(pimg);
        int srcHeight = getheight(pimg);
        if (srcWidth == 0 || srcHeight == 0) return;

        PIMAGE newImg = newimage(zoomWidth, zoomHeight);
        if (!newImg) return;

        for (int y = 0; y < zoomHeight; ++y) {
            float srcY = (y + 0.5f) * srcHeight / zoomHeight - 0.5f;
            int yInt = (int)floor(srcY);

            for (int x = 0; x < zoomWidth; ++x) {
                float srcX = (x + 0.5f) * srcWidth / zoomWidth - 0.5f;
                int xInt = (int)floor(srcX);

                // 6邻域：自身、上下左右、右下对角
                int dx[6] = { 0, 1, 0, -1, 0, 1 };
                int dy[6] = { 0, 0, 1, 0, -1, 1 };

                float sumA = 0, sumR = 0, sumG = 0, sumB = 0;
                int count = 0;

                for (int k = 0; k < 6; ++k) {
                    int sx = std::clamp(xInt + dx[k], 0, srcWidth - 1);
                    int sy = std::clamp(yInt + dy[k], 0, srcHeight - 1);

                    DWORD c = getpixel(sx, sy, pimg);
                    BYTE a = (c >> 24) & 0xFF;
                    BYTE r = (c >> 16) & 0xFF;
                    BYTE g = (c >> 8) & 0xFF;
                    BYTE b = c & 0xFF;

                    sumA += a;
                    sumR += r;
                    sumG += g;
                    sumB += b;
                    count++;
                }

                BYTE a = (BYTE)std::clamp(int(sumA / count + 0.5f), 0, 255);
                BYTE r = (BYTE)std::clamp(int(sumR / count + 0.5f), 0, 255);
                BYTE g = (BYTE)std::clamp(int(sumG / count + 0.5f), 0, 255);
                BYTE b = (BYTE)std::clamp(int(sumB / count + 0.5f), 0, 255);
                DWORD color = (a << 24) | (r << 16) | (g << 8) | b;
                putpixel(x, y, color, newImg);
            }
        }

        delimage(pimg);
        pimg = newImg;
    }

    // 旋转到指定角度，srcImg为原始图像，pimg为输出，6邻域加权平均
    void img::rotateImage_alpha(PIMAGE& pimg, int angle, const PIMAGE srcImg)
    {
        if (srcImg == NULL) return;

        int srcWidth = getwidth(srcImg);
        int srcHeight = getheight(srcImg);
        if (srcWidth == 0 || srcHeight == 0) return;

        double rad = angle * 3.14159265 / 180.0;
        double cosA = cos(rad), sinA = sin(rad);
        int newWidth = int(abs(srcWidth * cosA) + abs(srcHeight * sinA) + 0.5);
        int newHeight = int(abs(srcWidth * sinA) + abs(srcHeight * cosA) + 0.5);

        PIMAGE newImg = newimage(newWidth, newHeight);
        if (!newImg) return;

        float cx = newWidth / 2.0f;
        float cy = newHeight / 2.0f;
        float src_cx = srcWidth / 2.0f;
        float src_cy = srcHeight / 2.0f;

        for (int y = 0; y < newHeight; ++y) {
            for (int x = 0; x < newWidth; ++x) {
                float dx = x - cx;
                float dy = y - cy;

                // 逆向旋转到原图
                float rx = dx * cosA + dy * sinA;
                float ry = -dx * sinA + dy * cosA;

                float srcX = rx + src_cx;
                float srcY = ry + src_cy;

                int xInt = (int)floor(srcX);
                int yInt = (int)floor(srcY);

                // 6邻域：自身、上下左右、右下对角
                int dxs[6] = { 0, 1, 0, -1, 0, 1 };
                int dys[6] = { 0, 0, 1, 0, -1, 1 };

                float sumA = 0, sumR = 0, sumG = 0, sumB = 0;
                int count = 0;

                for (int k = 0; k < 6; ++k) {
                    int sx = std::clamp(xInt + dxs[k], 0, srcWidth - 1);
                    int sy = std::clamp(yInt + dys[k], 0, srcHeight - 1);

                    if (sx >= 0 && sx < srcWidth && sy >= 0 && sy < srcHeight) {
                        DWORD c = getpixel(sx, sy, srcImg);
                        BYTE a = (c >> 24) & 0xFF;
                        BYTE r = (c >> 16) & 0xFF;
                        BYTE g = (c >> 8) & 0xFF;
                        BYTE b = c & 0xFF;

                        sumA += a;
                        sumR += r;
                        sumG += g;
                        sumB += b;
                        count++;
                    }
                }

                if (count > 0) {
                    BYTE a = (BYTE)std::clamp(int(sumA / count + 0.5f), 0, 255);
                    BYTE r = (BYTE)std::clamp(int(sumR / count + 0.5f), 0, 255);
                    BYTE g = (BYTE)std::clamp(int(sumG / count + 0.5f), 0, 255);
                    BYTE b = (BYTE)std::clamp(int(sumB / count + 0.5f), 0, 255);
                    DWORD color = (a << 24) | (r << 16) | (g << 8) | b;
                    putpixel(x, y, color, newImg);
                }
            }
        }

        delimage(pimg);
        pimg = newImg;
    }

    // 缩放壁纸至屏幕大小（保持宽高比）
    void img::ScaleWallpaperToScreen() {
        // 1. 获取屏幕尺寸
        RECT desktopRect;
        SystemParametersInfo(SPI_GETWORKAREA, 0, &desktopRect, 0);
        int screenWidth = desktopRect.right - desktopRect.left;
        int screenHeight = desktopRect.bottom - desktopRect.top;

        int x = getwidth(backImage);
        int y = getheight(backImage);

        POINT ws = { screenWidth ,screenHeight };
        if (y * ws.x / x < ws.y)
        {
            ZoomImage_Alpha(backImage, x * ws.y / y, ws.y);
        }
        else
        {
            ZoomImage_Alpha(backImage, ws.x, y * ws.x / x);
        }
    }

    void img::zoomImage(PIMAGE& pimg, int zoomWidth, int zoomHeight)
    {
        //pimg为空，或目标图像大小和原图像一样，则不用缩放，直接返回
        if ((pimg == NULL) || (zoomWidth == getwidth(pimg) && zoomHeight == getheight(pimg)))
            return;

        PIMAGE zoomImage = newimage(zoomWidth, zoomHeight);
        putimage(zoomImage, 0, 0, zoomWidth, zoomHeight, pimg, 0, 0, getwidth(pimg), getheight(pimg));
        delimage(pimg);

        pimg = zoomImage;
    }