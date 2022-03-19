#ifndef LCD_API_H
#define LCD_API_H

#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_ts.h"
#include "stm32f429i_discovery_io.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_gyroscope.h"
#include "stm32f429i_discovery_sdram.h"

#define LCD_WIDTH 320
#define LCD_HEIGHT 240

#define TEXT_COLOR LCD_COLOR_WHITE
#define BACKGROUND_COLOR LCD_COLOR_BLACK

void initLCD();
void showTextToLCD(int, int, char*, int);
void clearLCD();
int getWidth();
int getHeight();
void mapTextToLandscape(int*, int*);
void mapShapeToLandscape(int*, int*);
void drawPoint(int, int);
void drawLine(int, int, int, int);
void removePoint(int, int);
void removeLine(int, int, int, int);

void showTextAtLine(int, int, char*);

#endif
