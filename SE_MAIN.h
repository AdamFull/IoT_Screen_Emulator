#ifndef __SE_MAIN__
#define __SE_MAIN__

#include "stdint.h"

#define GLEW_STATIC

#include "ShaderLoader.h"
//#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "SE_GFX.h"

typedef void(*GLKeyboardCallback)(GLFWwindow* window, int key, int scancode, int action, int mods);
typedef void(*GLMouseCallback)(GLFWwindow* window, double xpos, double ypos);

class SE_MAIN{
public:
    SE_MAIN();
    SE_MAIN(uint32_t sWidth, uint32_t sHeight);
    SE_MAIN(uint32_t sWidth, uint32_t sHeight, GLKeyboardCallback GL_Keyboard_Callback, GLMouseCallback GL_Mouse_Callback);
    ~SE_MAIN();

    void setScreenSize(uint32_t width, uint32_t height);

    bool init();
    bool init(std::string shaderFolderPath);

    void initVertexes();

    void setColor(Pixel &pixel);
    void display();
    void clear();

    bool getWindowWorker();

public:
    SE_GFX gfx;

/**********************************GL private variables**************************************/
private:
    GLFWwindow* window;
    void* gpuMemAddr;
    GLuint VBO, VAO;
    ShaderLoader shaderLoader;

/**********************************Emulator variables**************************************/
private:
    uint64_t pixelCounter;

private:
    void setPixel(Pixel pixel);

//Callbacks
private:
    GLKeyboardCallback GL_Keyboard_Callback;
    GLMouseCallback GL_Mouse_Callback;

public:
    void createPixel(float x, float y, Color color);
    void keyboardHandler(unsigned char c, int scancode, int action, int mods);
};


#endif