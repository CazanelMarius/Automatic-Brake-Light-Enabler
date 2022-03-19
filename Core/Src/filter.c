#include <math.h>
#include <lite_math.h>
#include <stdio.h>
#include <malloc.h>
#include "filter.h"

/* THE REAL VALUES FROM ACC (Ox) */
double accOX[LCD_WIDTH];

int SavitzkyGolayCoefficients_5[] = {-3, 12, 17, 12, -3};
int SavitzkyGolayCoefficients_11[] = {-36, 9, 44, 69, 84, 89, 84, 69, 44, 9, -36};

double SavitzkyGolayFilter(const double buffer[], const int coefficients[], int n) {
    double sum = 0;

    int sumCoeff = vectorIntSum(coefficients, n);

    for (int i = 0; i < n; i++) {
        sum += (coefficients[i] * buffer[i]);
    }

    return sum / sumCoeff;
}

void removeSpike(double v[], int n) {
    int m = (n - 1) / 2;

    double minSpikeValue = 0.03;

    double a = v[m - 1];
    double b = v[m];
    double c = v[m + 1];

    double minValue = min(a, min(b, c));
    double maxValue = max(a, max(b, c));

    if (minValue == b) {
        double minInt = min(a, c);
        v[m] = minInt - 0.0001;
        return;
    }

    if (maxValue == b) {
        double maxInt = max(a, c);
        v[m] = maxInt + 0.0001;
        return;
    }
}

double nextFilteredValueFromUserChoice(double buffer[], int n, int filterType) {
//    switch (filterType) {
//        case 1:
//            return SavitzkyGolayFilter(buffer, SavitzkyGolayCoefficients_5, POINTS_5);
//        case 2:
//            return SavitzkyGolayFilter(buffer, SavitzkyGolayCoefficients_11, POINTS_11);
//        default:
//            break;
//    }
//
//    return buffer[n - 1];

    return SavitzkyGolayFilter(buffer, SavitzkyGolayCoefficients_11, POINTS_11);
}

/** Changed the displayed parameters */
void onFilterChanged(char filerName[], uint32_t * delay, int filterType, double** OxBuffer, int *OxBufferLength, int* lightDelay) {
    // now the delay is calculated in the while loop
    free(*OxBuffer);

    *lightDelay = 500 + (*lightDelay % 5000);

    switch (filterType) {
        case 1:
//            sprintf(filerName, "%s", FILTER_SAVITZKY_GOLAY_5);
//            *OxBufferLength = 5;
//            *delay = calculateDelay(*OxBufferLength);
//
//            *OxBuffer = (double*) malloc(*OxBufferLength * sizeof(double));
            break;
        case 2:
//            sprintf(filerName, "%s", FILTER_SAVITZKY_GOLAY_11);
//            *OxBufferLength = 11;
//            *delay = calculateDelay(*OxBufferLength);
//
//            *OxBuffer = (double*) malloc(*OxBufferLength * sizeof(double));
            break;
        default:
//            sprintf(filerName, "%-20s", NO_FILTER);
//            *delay = calculateDelay(*OxBufferLength);
//            *OxBufferLength = 1;
//
//            *OxBuffer = (double*) malloc(*OxBufferLength * sizeof(double));
            break;
    }

    sprintf(filerName, "%s", FILTER_SAVITZKY_GOLAY_11);
    *OxBufferLength = 11;
    *delay = calculateDelay(*OxBufferLength);

    *OxBuffer = (double*) malloc(*OxBufferLength * sizeof(double));

}

/* TODO: The formula */
int calculateDelay(int sampleSize) {
    return sampleSize / 2;
}