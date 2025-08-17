#include "Main.h"
#include "ease.h"
    // ÏßÐÔ»º¶¯
    double ease::linear(double t) {
        return t;
    }

    double ease::easeIn(double t, double exp) {
        return pow(t, exp);
    }

    double ease::easeOut(double t, double exp) {
        return 1 - pow(1 - t, exp);
    }

    double ease::easeInOut(double t, double exp) {
        if (t < 0.5)
            return 0.5 * pow(2 * t, exp);
        else
            return 1 - 0.5 * pow(2 * (1 - t), exp);
    }

    double ease::easeInElastic(double t, double amplitude, double period) {
        if (t == 0) return 0;
        if (t == 1) return 1;

        double s = period / 4;
        t = t - 1;
        return -(amplitude * pow(2, 10 * t) * sin((t - s) * (2 * M_PI) / period));
    }

    double ease::easeOutElastic(double t, double amplitude, double period) {
        if (t == 0) return 0;
        if (t == 1) return 1;

        double s = period / 4;
        return amplitude * pow(2, -10 * t) * sin((t - s) * (2 * M_PI) / period) + 1;
    }

    double ease::easeInOutElastic(double t, double amplitude, double period) {
        if (t == 0) return 0;
        if (t == 1) return 1;

        t *= 2;
        if (t < 1) {
            return -0.5 * (amplitude * pow(2, 10 * (t - 1)) *
                sin((t - 1 - period / 4) * (2 * M_PI) / period));
        }
        return amplitude * pow(2, -10 * (t - 1)) *
            sin((t - 1 - period / 4) * (2 * M_PI) / period) * 0.5 + 1;
    }

    double ease::easeInBack(double t, double s) {
        return (s + 1) * t * t * t - s * t * t;
    }

    double ease::easeOutBack(double t, double s) {
        return 1 + (s + 1) * pow(t - 1, 3) + s * pow(t - 1, 2);
    }

    double ease::easeInOutBack(double t, double s) {
        double s2 = s * 1.525;
        if (t < 0.5)
            return 0.5 * (pow(2 * t, 2) * ((s2 + 1) * 2 * t - s2));
        else
            return 0.5 * (pow(2 * t - 2, 2) * ((s2 + 1) * (t * 2 - 2) + s2) + 2);
    }

    double ease::easeOutBounce(double t, int bounces, double height) {
        if (t < 1 / 2.75) {
            return 7.5625 * t * t;
        }
        else if (t < 2 / 2.75) {
            t -= 1.5 / 2.75;
            return 7.5625 * t * t + 0.75;
        }
        else if (t < 2.5 / 2.75) {
            t -= 2.25 / 2.75;
            return 7.5625 * t * t + 0.9375;
        }
        else {
            t -= 2.625 / 2.75;
            return 7.5625 * t * t + 0.984375;
        }
    }

    double ease::easeInBounce(double t, int bounces, double height) {
        return 1 - easeOutBounce(1 - t, bounces, height);
    }

    double ease::easeInOutBounce(double t, int bounces, double height) {
        if (t < 0.5) return easeInBounce(t * 2, bounces, height) * 0.5;
        return easeOutBounce(t * 2 - 1, bounces, height) * 0.5 + 0.5;
    }

    double ease::easeInSine(double t) {
        return 1 - cos((t * M_PI) / 2);
    }

    double ease::easeOutSine(double t) {
        return sin((t * M_PI) / 2);
    }

    double ease::easeInOutSine(double t) {
        return -(cos(M_PI * t) - 1) / 2;
    }

    double ease::easeInExpo(double t) {
        return t == 0 ? 0 : pow(2, 10 * (t - 1));
    }

    double ease::easeOutExpo(double t) {
        return t == 1 ? 1 : 1 - pow(2, -10 * t);
    }

    double ease::easeInOutExpo(double t) {
        if (t == 0) return 0;
        if (t == 1) return 1;
        if (t < 0.5) return 0.5 * pow(2, 20 * t - 10);
        return 1 - 0.5 * pow(2, -20 * t + 10);
    }

    double ease::easeInCirc(double t) {
        return 1 - sqrt(1 - t * t);
    }

    double ease::easeOutCirc(double t) {
        return sqrt(1 - pow(t - 1, 2));
    }

    double ease::easeInOutCirc(double t) {
        if (t < 0.5)
            return 0.5 * (1 - sqrt(1 - 4 * t * t));
        else
            return 0.5 * (sqrt(1 - pow(-2 * t + 2, 2))) + 0.5;
    }