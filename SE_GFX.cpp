#include "SE_GFX.h"
#include "character_codes.h"

#include <cmath>

/*****************************************************************************************/
SE_GFX::SE_GFX(){
    setCursorDefault();
    textSize = 1;
}

/*****************************************************************************************/
SE_GFX::SE_GFX(uint64_t width, uint64_t height){
    this->width = width;
    this->height = height;
    setCursorDefault();
    textSize = 1;
}

/*****************************************************************************************/
SE_GFX::~SE_GFX(){

}


/*****************************************************************************************/
/**********************************Drawing GFX methods************************************/
/*****************************************************************************************/
void SE_GFX::drawPixel(uint32_t x, uint32_t y, Color color){
    float x0 = ((2.0f*(float)x+1.0f)/width)-1.0f;
    float y0 = ((2.0f*(float)y+1.0f)/height)-1.0f;
    fnSetPixel(x0, y0, color);
}

/*****************************************************************************************/
void SE_GFX::drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, Color color){
    int steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        swap(x0, y0);
        swap(x1, y1);
    }
    if (x0 > x1) {
        swap(x0, x1);
        swap(y0, y1);
    }
    int dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);
    int err = dx / 2;
    int ystep;
    if (y0 < y1) {ystep = 1;}
    else {ystep = -1;};
    for ( ; x0 <= x1; x0++) {
        if (steep) {drawPixel(y0, x0, color);}
        else {drawPixel(x0, y0, color);};
		err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

/*****************************************************************************************/
void SE_GFX::drawFastVLine(uint32_t x, uint32_t y, uint32_t h, Color color){
    drawLine(x, y, x, y+h-1, color);
}

/*****************************************************************************************/
void SE_GFX::drawFastHLine(uint32_t x, uint32_t y, uint32_t w, Color color){
    drawLine(x, y, x+w-1, y, color);
}

/*****************************************************************************************/
void SE_GFX::drawRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, Color color){
    drawFastHLine(x, y, w, color);
    drawFastHLine(x, y+h-1, w, color);
    drawFastVLine(x, y, h, color);
    drawFastVLine(x+w-1, y, h, color);
}

/*****************************************************************************************/
void SE_GFX::fillRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, Color color){
    for (int16_t i=x; i<x+w; i++) {
        drawFastVLine(i, y, h, color);
    }
}

/*****************************************************************************************/
void SE_GFX::fillScreen(Color color){
    fillRect(0, 0, width, height, color);
}

/*****************************************************************************************/
void SE_GFX::drawEllipse(uint32_t x, uint32_t y, uint32_t width, uint32_t height, Color color){
    for (uint32_t j = 0; j < 360; j++){
        double angle_circle = (j*0.01745331);
        uint32_t x_edge = (x + (width*cos(angle_circle)));
        uint32_t y_edge = (y + (height*sin(angle_circle)));
        drawPixel(x_edge,y_edge, color);
    }
}

/*****************************************************************************************/
void SE_GFX::fillEllipse(uint32_t x, uint32_t y, uint32_t width, uint32_t height, Color color){
    uint32_t newWidth = width;
    uint32_t newHeight = height;
    while(newWidth > 0 && newHeight > 0){
        drawEllipse(x, y, newWidth, newHeight, color);
        newWidth--;
        newHeight--;
    }
}

/*****************************************************************************************/
void SE_GFX::drawCircle(uint32_t x, uint32_t y, uint32_t radius, Color color){
    drawEllipse(x, y, radius, radius, color);
}

/*****************************************************************************************/
void SE_GFX::fillCircle(uint32_t x, uint32_t y, uint32_t radius, Color color){
    fillEllipse(x, y, radius, radius, color);
}

/*****************************************************************************************/
void SE_GFX::roundRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t r, Color color){
    uint32_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
    //if (r > max_radius)
    //    r = max_radius;
    // smarter version
    //writeFastHLine(x + r, y, w - 2 * r, color);         // Top
    //writeFastHLine(x + r, y + h - 1, w - 2 * r, color); // Bottom
    //writeFastVLine(x, y + r, h - 2 * r, color);         // Left
    //writeFastVLine(x + w - 1, y + r, h - 2 * r, color); // Right
    // draw four corners
    //drawCircleHelper(x + r, y + r, r, 1, color);
    //drawCircleHelper(x + w - r - 1, y + r, r, 2, color);
    //drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
    //drawCircleHelper(x + r, y + h - r - 1, r, 8, color);
}

/*****************************************************************************************/
void SE_GFX::drawTriangle(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, Color color){
    drawLine(x0, y0, x1, y1, color);
    drawLine(x1, y1, x2, y2, color);
    drawLine(x2, y2, x0, y0, color);
}

/*****************************************************************************************/
void SE_GFX::drawChar(uint32_t x, uint32_t y, Color bg, Color color, uint32_t size, unsigned char c){
    if((x >= width) ||(y >= height) || ((x + 4) < 0) || ((y + 7) < 0)) return;
	if(c<128)            c = c-32;
	if(c>=144 && c<=175) c = c-48;
	if(c>=128 && c<=143) c = c+16;
	if(c>=176 && c<=191) c = c-48;
	if(c>191)  return;
	for (int32_t i=0; i<6; i++) {
		uint32_t line;
		if (i == 5) {line = 0x00;}
		else {
			line = font[(c*5)+i];
			for (int32_t j = 0; j<8; j++)
			{
				if (line & 0x01) {
					if(size == 1){
						drawPixel(x+i, y-j, color);
					}else{
						fillRect(x+(i*size), y-(j*size), size, size, color);
					}
				}else if(!isColorsSame(bg, color)){
					if(size == 1){
						drawPixel(x+i, y-j, bg);
					}else{
						fillRect(x+(i*size), y-(j*size), size, size, bg);
					}
				}
				line >>= 1;
			}
		}
	}
}

/*****************************************************************************************/
void SE_GFX::print(const char* str){
    print(str, CL_WHITE(), CL_BLACK());
}

/*****************************************************************************************/
void SE_GFX::print(const char* str, Color txColor){
    print(str, txColor, CL_BLACK());
}

/*****************************************************************************************/
void SE_GFX::print(const char* str, Color txColor, Color bgColor){
    unsigned char type = *str;
    if(type>=128) cursorX=cursorX-3;
    while(*str){
        drawChar(cursorX, cursorY, bgColor, txColor, 1, *str++);
        unsigned char type = *str;
        if (type>=128) {cursorX=cursorX+textSize*3;}
        else {cursorX=cursorX+textSize*5;};
    }
}

/*****************************************************************************************/
void SE_GFX::printText(const char* str){
    printText(str, CL_WHITE(), CL_BLACK());
}

/*****************************************************************************************/
void SE_GFX::printText(const char* str, Color txColor){
    printText(str, txColor, CL_BLACK());
}

/*****************************************************************************************/
void SE_GFX::printText(const char* str, Color txColor, Color bgColor){
    uint32_t charsTyped = 0;
    unsigned char type = *str;
    if(type>=128) cursorX=cursorX-3;
    while(*str){
        drawChar(cursorX, cursorY, bgColor, txColor, 1, *str++);
        unsigned char type = *str;
        if (type>=128) {cursorX=cursorX+textSize*3;}
        else {cursorX=cursorX+textSize*5;};
        if((float)charsTyped++ * 5.2f * textSize > width){
            charsTyped = 0;
            newString();
        }
    }
    newString();
}

/*****************************************************************************************/
void SE_GFX::println(const char* str){
    println(str, CL_WHITE(), CL_BLACK());
}

/*****************************************************************************************/
void SE_GFX::println(const char* str, Color txColor){
    println(str, txColor, CL_BLACK());
}

/*****************************************************************************************/
void SE_GFX::println(const char* str, Color txColor, Color bgColor){
    print(str, txColor, bgColor);
    newString();
}

void SE_GFX::newString(){
    cursorX = 0;
    if(cursorY > 16) cursorY = cursorY - 8;
    else cursorY = height - 3;
}