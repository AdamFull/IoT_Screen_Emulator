#ifndef __SE_GFX__
#define __SE_GFX__

#include "SE_UTILS.h"

typedef void (*fnSetPix)(float, float, Color);

class SE_GFX
{
public:
    SE_GFX();
    SE_GFX(uint64_t width, uint64_t height);
    ~SE_GFX();

    void setWidth(uint64_t nWidth) { width = nWidth; }
    void setHeight(uint64_t nHeight) { height = nHeight; }
    uint64_t getWidth() { return width; }
    uint64_t getHeignt() { return height; }
    uint64_t getBufferSize() { return sizeof(float) * 6 * width*height; }

    void setCallback(fnSetPix fnSetPixel) { this->fnSetPixel = fnSetPixel; }

    /********************************Public GFX Methods**************************************/
    void drawPixel(uint32_t x, uint32_t y, Color color);
    void drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, Color color);
    void drawFastVLine(uint32_t x, uint32_t y, uint32_t h, Color color);
    void drawFastHLine(uint32_t x, uint32_t y, uint32_t w, Color color);
    void drawRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, Color color);
    void fillRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, Color color);
    void fillScreen(Color color);
    void drawEllipse(uint32_t x, uint32_t y, uint32_t width, uint32_t height, Color color);
    void fillEllipse(uint32_t x, uint32_t y, uint32_t width, uint32_t height, Color color);
    void drawCircle(uint32_t x, uint32_t y, uint32_t radius, Color color);
    void fillCircle(uint32_t x, uint32_t y, uint32_t radius, Color color);
    void roundRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t r, Color color);
    void drawTriangle(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, Color color);
    //void fillTriangle();

    void drawChar(uint32_t x, uint32_t y, Color bg, Color color, uint32_t size, unsigned char c);
    void print(const char* str);
    void print(const char* str, Color txColor);
    void print(const char* str, Color txColor, Color bgColor);
    void printText(const char* str);
    void printText(const char* str, Color txColor);
    void printText(const char* str, Color txColor, Color bgColor);
    void println(const char* str);
    void println(const char* str, Color txColor);
    void println(const char* str, Color txColor, Color bgColor);
    void newString();

    void setCursor(uint32_t x, uint32_t y) { cursorX = x; cursorY = y; }
    void setCursorDefault() { cursorX = 0; cursorY = height - 3; }
private:
    fnSetPix fnSetPixel;
    //Screen size
    uint64_t width;
    uint64_t height;
    //Text cursor
    uint32_t cursorX;
    uint32_t cursorY;
    uint32_t textSize;

    void swap(int32_t &a, int32_t &b) { 
        uint32_t t = a; 
        a = b; 
        b = t; 
    }
};

#endif