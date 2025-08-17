#pragma once
#include <math.h>
using namespace std;
class ease {
public:
    //=====================Բ����====================
    static constexpr double M_PI = 3.14159265358979323846;

    // ==================== ������������ ====================
    // ���Ի����������˶����޼��ٶ� : . . . . . . . . . . . . . . .
    static double linear(double t);

    // ==================== �ݴλ��� ====================
    // ����/�ݴλ�����easeIn���������˶�����ʼ��������� : .      .      .    .   .   . . . ...
    static double easeIn(double t, double exp = 3.0);
    // �ݴλ�����easeOut���������˶�����ʼ�죬������ : ... . . .   .   .    .      .      .
    static double easeOut(double t, double exp = 3.0);
    // �ݴλ�����easeInOut��������������� : .    .   . . . . . . . .   .    .
    static double easeInOut(double t, double exp = 3.0);

    // ==================== ���Ի��� ====================
    // ���Ի�����easeInElastic������ʼʱ�е��Ի������𽥼��� : .  .   .    .     .      .   . . . . .
    static double easeInElastic(double t, double amplitude = 1.0, double period = 0.3);
    // ���Ի�����easeOutElastic��������ʱ�е��Ի������𽥼��� : . . . . .   .      .     .    .   .  .
    static double easeOutElastic(double t, double amplitude = 1.0, double period = 0.3);
    // ���Ի�����easeInOutElastic�������˵��Ի��� : .  .   .    . . . . . . . .    .   .  .
    static double easeInOutElastic(double t, double amplitude = 1.0, double period = 0.45);

    // ==================== �ص����� ====================
    // Back ������easeInBack������ʼʱ�Ȼ�����ǰ�� :   .    .   . . . . . . . . . .
    static double easeInBack(double t, double s = 1.70158);
    // Back ������easeOutBack��������ʱ����Ŀ���ٻص� : . . . . . . . . .   .    .
    static double easeOutBack(double t, double s = 1.70158);
    // Back ������easeInOutBack�������˻ص� :   .   . . . . . . . . .   .   .
    static double easeInOutBack(double t, double s = 1.70158);

    // ==================== �������� ====================
    // Bounce ������easeOutBounce����������ص�������ֹͣ : . . . . . . . . . . . . . . .
    static double easeOutBounce(double t, int bounces = 3, double height = 0.5);
    // Bounce ������easeInBounce���������������򷴵����� : . . . . . . . . . . . . . . .
    static double easeInBounce(double t, int bounces = 3, double height = 0.5);
    // Bounce ������easeInOutBounce�������˵��� : . . . . . . . . . . . . . . .
    static double easeInOutBounce(double t, int bounces = 3, double height = 0.5);

    // ==================== ���һ��� ====================
    // ���һ�����easeInSine������ʼ��������죬�������� : .    .   . . . . . . . . .
    static double easeInSine(double t);
    // ���һ�����easeOutSine������ʼ�죬���������������� : . . . . . . . .   .    .
    static double easeOutSine(double t);
    // ���һ�����easeInOutSine������������������������� : .   . . . . . . . .   .
    static double easeInOutSine(double t);

    // ==================== ָ������ ====================
    // ָ��������easeInExpo������ʼ���������漫�� : .         .        .      .   . . . . .
    static double easeInExpo(double t);
    // ָ��������easeOutExpo������ʼ���죬���漫�� : . . . . .   .      .        .         .
    static double easeOutExpo(double t);
    // ָ��������easeInOutExpo�������˼������м伫�� : .     . . . . . . . . .     .
    static double easeInOutExpo(double t);

    // ==================== Բ�λ��� ====================
    // Բ�λ�����easeInCirc������ʼ��������죬Բ������ : .    .   . . . . . . . .
    static double easeInCirc(double t);
    // Բ�λ�����easeOutCirc������ʼ�죬��������Բ������ : . . . . . . . .   .    .
    static double easeOutCirc(double t);
    // Բ�λ�����easeInOutCirc�������������м�죬Բ������ : .   . . . . . . . .   .
    static double easeInOutCirc(double t);
};