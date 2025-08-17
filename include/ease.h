#pragma once
#include <math.h>
using namespace std;
class ease {
public:
    //=====================圆周率====================
    static constexpr double M_PI = 3.14159265358979323846;

    // ==================== 基本缓动函数 ====================
    // 线性缓动：匀速运动，无加速度 : . . . . . . . . . . . . . . .
    static double linear(double t);

    // ==================== 幂次缓动 ====================
    // 立方/幂次缓动（easeIn）：加速运动，开始慢，后面快 : .      .      .    .   .   . . . ...
    static double easeIn(double t, double exp = 3.0);
    // 幂次缓动（easeOut）：减速运动，开始快，后面慢 : ... . . .   .   .    .      .      .
    static double easeOut(double t, double exp = 3.0);
    // 幂次缓动（easeInOut）：先慢后快再慢 : .    .   . . . . . . . .   .    .
    static double easeInOut(double t, double exp = 3.0);

    // ==================== 弹性缓动 ====================
    // 弹性缓动（easeInElastic）：开始时有弹性回拉，逐渐加速 : .  .   .    .     .      .   . . . . .
    static double easeInElastic(double t, double amplitude = 1.0, double period = 0.3);
    // 弹性缓动（easeOutElastic）：结束时有弹性回拉，逐渐减速 : . . . . .   .      .     .    .   .  .
    static double easeOutElastic(double t, double amplitude = 1.0, double period = 0.3);
    // 弹性缓动（easeInOutElastic）：两端弹性回拉 : .  .   .    . . . . . . . .    .   .  .
    static double easeInOutElastic(double t, double amplitude = 1.0, double period = 0.45);

    // ==================== 回弹缓动 ====================
    // Back 缓动（easeInBack）：开始时先回拉再前进 :   .    .   . . . . . . . . . .
    static double easeInBack(double t, double s = 1.70158);
    // Back 缓动（easeOutBack）：结束时超出目标再回弹 : . . . . . . . . .   .    .
    static double easeOutBack(double t, double s = 1.70158);
    // Back 缓动（easeInOutBack）：两端回弹 :   .   . . . . . . . . .   .   .
    static double easeInOutBack(double t, double s = 1.70158);

    // ==================== 弹跳缓动 ====================
    // Bounce 缓动（easeOutBounce）：像球落地弹跳，逐渐停止 : . . . . . . . . . . . . . . .
    static double easeOutBounce(double t, int bounces = 3, double height = 0.5);
    // Bounce 缓动（easeInBounce）：反向弹跳，像球反弹上升 : . . . . . . . . . . . . . . .
    static double easeInBounce(double t, int bounces = 3, double height = 0.5);
    // Bounce 缓动（easeInOutBounce）：两端弹跳 : . . . . . . . . . . . . . . .
    static double easeInOutBounce(double t, int bounces = 3, double height = 0.5);

    // ==================== 正弦缓动 ====================
    // 正弦缓动（easeInSine）：开始慢，后面快，正弦曲线 : .    .   . . . . . . . . .
    static double easeInSine(double t);
    // 正弦缓动（easeOutSine）：开始快，后面慢，正弦曲线 : . . . . . . . .   .    .
    static double easeOutSine(double t);
    // 正弦缓动（easeInOutSine）：先慢后快再慢，正弦曲线 : .   . . . . . . . .   .
    static double easeInOutSine(double t);

    // ==================== 指数缓动 ====================
    // 指数缓动（easeInExpo）：开始极慢，后面极快 : .         .        .      .   . . . . .
    static double easeInExpo(double t);
    // 指数缓动（easeOutExpo）：开始极快，后面极慢 : . . . . .   .      .        .         .
    static double easeOutExpo(double t);
    // 指数缓动（easeInOutExpo）：两端极慢，中间极快 : .     . . . . . . . . .     .
    static double easeInOutExpo(double t);

    // ==================== 圆形缓动 ====================
    // 圆形缓动（easeInCirc）：开始慢，后面快，圆弧曲线 : .    .   . . . . . . . .
    static double easeInCirc(double t);
    // 圆形缓动（easeOutCirc）：开始快，后面慢，圆弧曲线 : . . . . . . . .   .    .
    static double easeOutCirc(double t);
    // 圆形缓动（easeInOutCirc）：两端慢，中间快，圆弧曲线 : .   . . . . . . . .   .
    static double easeInOutCirc(double t);
};