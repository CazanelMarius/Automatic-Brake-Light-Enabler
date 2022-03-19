#include <math.h>
#include <stdarg.h>
#include "lite_math.h"

double mean(const double* v, int n) {
    double sum = 0;

    for (int i = 0; i < n; i++) {
        sum += v[i];
    }

    return sum / n;
}

double min(double a, double b) {
    return a < b ? a : b;
}

double max(double a, double b) {
    return b > a ? b : a;
}

int vectorIntSum(const int v[], int n) {
    int sum = 0;

    for (int i = 0; i < n; i++) {
        sum += v[i];
    }

    return sum;
}

/** Converts the degree value in radians */
double radiansToDegree(double radians) {
    return radians * 180 / PI;
}

/** Calculates the magnitude of a vector with a given dimension */
double vectorMagnitude(int n, ...) {
    va_list arguments;
    double sum = 0;

    va_start (arguments, n);

    for (int i = 0; i < n; i++ ) {
        double value = va_arg (arguments, double);
        sum += value * value;
    }

    va_end ( arguments );

    return pow(sum, 1.0 / n);
}

/** Returns the angle in degrees of a vector with the origin */
double vectorAngle(double x, double y) {
    return radiansToDegree(atan(y / x));
}

/** Returns the angle in radians of a vector with the origin */
double vectorAngleInRadians(double x, double y) {
    return atan(y / x);
}

/** Returns the real angle in degrees of a vector */
double vectorRealAngle(double x, double y) {
    double angle = radiansToDegree(atan(absD(y) / absD(x)));

    if (x >= 0 && y >= 0) {
        return angle;
    }

    if (x < 0 && y > 0) {
        return 180 - angle;
    }

    if (x < 0 && y < 0) {
        return 180 + angle;
    }

    return 360 - angle;
}

/**  */
double vectorRealAngleInRadians(double x, double y) {
    return toRadians(vectorAngle(x, y));
}

double toRadians(double degree) {
    return degree * PI / 180;
}

/** Returns the board angle. It gets the x and z values and returns the slope.
 *  It's calculated by inverting the angle because the angle given decrease as the real angle grows
 *  and the resulted value is mapped to it's corresponding quadrant.
 *  But it's legacy because the vector doesn't give the board angle in movement.
 * */
double getBoardAngle(double x, double y) {
    double angle = 360 - vectorRealAngle(x, y) + 90;

    if (angle > 360) {
        angle -= 360;
    }

    if (angle >= 180) {
        angle = angle - 360;
    }

    return angle;
}

short sign(double x) {
    return x < 0 ? -1 : 1;
}

double absD(double x) {
    return x < 0 ? - x : x;
}