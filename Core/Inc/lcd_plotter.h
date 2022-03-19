#ifndef LCD_PLOTTER_H
#define LCD_PLOTTER_H
#include "lcd_api.h"

#define REPRESENTATION_SCALE 1

#define PLOT_MAX_SCALE REPRESENTATION_SCALE
#define PLOT_MIN_SCALE -REPRESENTATION_SCALE

#define GRAPH_COLOR LCD_COLOR_RED
#define GRAPH_ACTIVATED_COLOR LCD_COLOR_RED

#define INITIAL_BUFFER_VALUE -1000

extern double graph[LCD_WIDTH];
extern double lastGraphConfig[LCD_WIDTH];
extern uint32_t graphColor[LCD_WIDTH];
extern uint32_t intervalColors[3];

void plotInit();

void drawGraph(double[LCD_WIDTH]);
double getMax(const double[LCD_WIDTH]);
double getMin(const double[LCD_WIDTH]);
int mapToDisplay(double, double, double );
void pushValue(double*, int, double);
void pushIntValue(uint32_t[LCD_WIDTH], int, uint32_t);
void clearGraph(double [LCD_WIDTH], double[LCD_WIDTH]);
void initializeBuffer(double*, int, double);

double mapToInterval(double, double, double);

uint32_t getColorValueInInterval(double);

void timeStampToTime(unsigned long, char*);

#endif
