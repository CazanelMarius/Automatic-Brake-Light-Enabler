#include <stdio.h>
#include "lcd_plotter.h"

double graph[LCD_WIDTH];
double lastGraphConfig[LCD_WIDTH];
uint32_t graphColor[LCD_WIDTH];
extern uint32_t intervalColors[3] = {LCD_COLOR_BROWN, LCD_COLOR_CYAN, LCD_COLOR_YELLOW};

double lastGraphConfig[LCD_WIDTH];

void plotInit() {
    initializeBuffer(graph, LCD_WIDTH, INITIAL_BUFFER_VALUE);
    initializeBuffer(lastGraphConfig, LCD_WIDTH, INITIAL_BUFFER_VALUE);

    for (int i = 0; i < LCD_WIDTH; i++) {
        graphColor[i] = GRAPH_COLOR;
    }
}

void drawGraph(double v[LCD_WIDTH]) {
    double min = PLOT_MIN_SCALE;
    double max = PLOT_MAX_SCALE;

    int padding = 20;

    char string[20];

    sprintf(string, "%4.2f", max);
    showTextAtLine(LCD_WIDTH - 40, 7, string);

    sprintf(string, "%4.2f", min);
    showTextAtLine(LCD_WIDTH - 48, 5, string);

    for (int i = 1; i < LCD_WIDTH; i++) {
        int x1 = i - 1;
        int x2 = i;

        if (v[i - 1] == -1000) {
            continue;
        }

        double y1 = mapToInterval(min, v[i - 1], max);
        double y2 = mapToInterval(min, v[i], max);

        int y1M = mapToDisplay(min, y1, max);
        int y2M = mapToDisplay(min, y2, max);

        BSP_LCD_SetTextColor(graphColor[i]);

        drawLine(x1, LCD_HEIGHT - y1M - padding, x2, LCD_HEIGHT - y2M - padding);
    }

}

uint32_t getColorValueInInterval(double lastY) {
    double min = PLOT_MIN_SCALE;
    double max = PLOT_MAX_SCALE;

    double interval = max - min;
    double part = interval / 3;

    double currentPosition = lastY - min;

    if (isUnderTreshold(lastY)) {
    	return LCD_COLOR_GREEN;
    }

    for (int i = 0; i < 3; i++) {
        if (currentPosition >= part * i && currentPosition < part * (i + 1)) {
            return intervalColors[i];
        }
    }

    return LCD_COLOR_BLUE;
}

void pushValue(double *v, int n, double x) {
    for (int i = 0; i < n - 1; i++) {
        v[i] = v[i + 1];
    }

    v[n - 1] = x;
}

void pushIntValue(uint32_t v[LCD_WIDTH], int n, uint32_t value) {
    for (int i = 0; i < n - 1; i++) {
        v[i] = v[i + 1];
    }

    v[n - 1] = value;
}

void initializeBuffer(double v[], int n, double value) {
    for (int i = 0; i < n; i++) {
        v[i] = value;
    }
}

int mapToDisplay(double min, double value, double max) {
    int width = LCD_WIDTH;
    int height = LCD_HEIGHT - 40;

    double dif = max - min;

    double ratio = 1.0 * height / dif;

    value = value - min;
    value *= ratio;

    return (int) value;
}

double getMax(const double v[LCD_WIDTH]) {
    double max = -100000;

    for (int i = 0; i < LCD_WIDTH; i++) {
        if (v[i] == -1000) {
            continue;
        }

        if (max < v[i]) {
            max = v[i];
        }
    }

    return max;
}

double getMin(const double v[LCD_WIDTH]) {
    double min = 1 << 16;

    for (int i = 0; i < LCD_WIDTH; i++) {
        if (v[i] == -1000) {
            continue;
        }

        if (min > v[i]) {
            min = v[i];
        }
    }

    return min;
}

/** Clears the graph from the screen */
void clearGraph(double graph[LCD_WIDTH], double lastGraphConfig[]) {
    double min = PLOT_MIN_SCALE;
    double max = PLOT_MAX_SCALE;

    int padding = 20;

    for (int i = 1; i < LCD_WIDTH; i++) {
        int x1 = i - 1 + 1;
        int x2 = i + 1;

        if (graph[i - 1] == -1000) {
            continue;
        }

        /* CURRENT (x, y) POINT */
        int y1 = mapToDisplay(min, graph[i - 1], max);
        int y2 = mapToDisplay(min, graph[i], max);

        /* PREVIOUS (x, y) POINT */
        int old_y1 = mapToDisplay(min, lastGraphConfig[i - 1], max);
        int old_y2 = mapToDisplay(min, lastGraphConfig[i], max);

        /* IF THE CURRENT LINE AND OLD LINE ARE THE SAME, THEN DON'T CHANGE THE PIXELS */
        if (y1 == old_y1 && y2 == old_y2) {
            continue;
        }

        double y1V = mapToInterval(min, lastGraphConfig[i - 1], max);
        double y2V = mapToInterval(min, lastGraphConfig[i], max);

        old_y1 = mapToDisplay(min, y1V, max);
        old_y2 = mapToDisplay(min, y2V, max);

        removeLine(x1, LCD_HEIGHT - old_y1 - padding, x2, LCD_HEIGHT - old_y2 - padding);
    }
}

double mapToInterval(double start, double value, double end) {
    if (value > end) {
        return end;
    }

    if (value < start) {
        return start;
    }

    return value;
}

void timeStampToTime(unsigned long time, char* buffer) {
    unsigned long ms = time % 1000;

    unsigned long totalSec = time / 1000;

    unsigned long hours = totalSec / 3600;
    unsigned long minutes = (totalSec - hours * 3600) / 60;
    unsigned long seconds = totalSec - hours * 3600 - minutes * 60;

    sprintf(buffer, "%2luh %2lumin %2lusec %3lums", hours, minutes, seconds, ms);
}
