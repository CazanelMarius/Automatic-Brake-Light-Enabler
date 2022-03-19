#ifndef FILTER_H
#define FILTER_H

#include "lcd_api.h"

#define POINTS_5 5
#define POINTS_11 11

#define NO_FILTER "No Filter"
#define FILTER_SAVITZKY_GOLAY_5 "Savitzky Golay 5"
#define FILTER_SAVITZKY_GOLAY_11 "Savitzky Golay 11"

extern double accOX[LCD_WIDTH];

#define FILTER_NUMBER 3

extern int SavitzkyGolayCoefficients_5[];
extern int SavitzkyGolayCoefficients_11[];

double SavitzkyGolayFilter(const double[], const int[], int);
void removeSpike(double[], int);

double nextFilteredValueFromUserChoice(double[], int, int);
void onFilterChanged(char[], uint32_t *, int, double**, int*, int*);
int calculateDelay(int);

#endif