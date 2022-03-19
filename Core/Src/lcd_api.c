#include "lcd_api.h"

void initLCD() {
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(0, LCD_FRAME_BUFFER);
    BSP_LCD_LayerDefaultInit(1, LCD_FRAME_BUFFER+(BSP_LCD_GetXSize()*BSP_LCD_GetYSize()*4));
    BSP_LCD_DisplayOn();
    BSP_LCD_SelectLayer(0);
    BSP_LCD_Clear(BACKGROUND_COLOR);
    BSP_LCD_SelectLayer(1);
    BSP_LCD_Clear(BACKGROUND_COLOR);
    BSP_LCD_SetFont(&Font12);
    BSP_LCD_SetBackColor(BACKGROUND_COLOR);
    BSP_LCD_SetTextColor(TEXT_COLOR);
}

void showTextToLCD(int x, int y, char* buffer, int mode) {
    mapTextToLandscape(&x, &y);

    BSP_LCD_SetTextColor(TEXT_COLOR);
    BSP_LCD_DisplayStringAt(x, y, (uint8_t*) buffer, mode);
}

void showTextAtLine(int x, int line, char* string) {
    BSP_LCD_SetTextColor(TEXT_COLOR);
    BSP_LCD_DisplayStringAt(x, LINE(line), (uint8_t *)string, LEFT_MODE);
}

void drawPoint(int x, int y) {
    int copyX = x;
    int copyY = y;

    mapShapeToLandscape(&copyX, &copyY);

    BSP_LCD_FillRect(copyX, copyY,1, 1);
}

void drawLine(int x1, int y1, int x2, int y2) {
    x1 += 0;
    x2 += 0;
    y1 += 80;
    y2 += 80;

    BSP_LCD_DrawLine(x1, y1, x2, y2);
}

void removeLine(int x1, int y1, int x2, int y2) {
    x1 += 0;
    x2 += 0;
    y1 += 80;
    y2 += 80;

    BSP_LCD_SetTextColor(BACKGROUND_COLOR);
    BSP_LCD_DrawLine(x1, y1, x2, y2);
}

void removePoint(int x, int y) {
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    mapShapeToLandscape(&x, &y);

    BSP_LCD_FillRect(x, y,1, 1);
}

void clearLCD() {
    BSP_LCD_Clear(LCD_COLOR_BLACK);
}

int getWidth() {
    return (int) BSP_LCD_GetXSize();
}

int getHeight() {
    return (int) BSP_LCD_GetYSize();
}

void mapTextToLandscape(int* x, int* y) {
    // -160 0
    int width = LCD_HEIGHT;
    int height = LCD_WIDTH;

    int aux = *x;
    *x = *y;
    *y = width + aux - 160;
    // 0 0
}

void mapShapeToLandscape(int* x, int *y) {
    int width = LCD_WIDTH;
    int height = LCD_HEIGHT;

    int oldX = *x;
    int oldY = *y;

    int newX = height - oldY;
    int newY = oldX;

    *x = newX;
    *y = newY;
}
