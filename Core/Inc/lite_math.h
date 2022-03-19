#ifndef LITE_MATH_H
#define LITE_MATH_H

#define PI 3.1415926535

double mean(const double*, int n);
double min(double, double);
double max(double, double);

int vectorIntSum(const int[], int);

double radiansToDegree(double);
double vectorMagnitude(int, ...);
double vectorAngle(double, double);
double vectorAngleInRadians(double, double);

double vectorRealAngle(double, double);
double vectorRealAngleInRadians(double, double);

double toRadians(double);
double getBoardAngle(double, double);
short sign(double);
double absD(double);
#endif