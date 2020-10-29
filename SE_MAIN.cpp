#include "SE_MAIN.h"

#include "stdio.h"
#include "stdlib.h"
#include <cstring>

#include <chrono>

static SE_MAIN* pData;

/*****************************************************************************************/
/****************************************CALLBACKS****************************************/
/*****************************************************************************************/
extern "C"
void window_size_callback(GLFWwindow* window, int width, int height) {
    pData->gfx.setWidth(width);
    pData->gfx.setHeight(height);
	glViewport(0, 0, width, height);		// Draw into entire window
}

/*****************************************************************************************/
extern "C"
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    pData->keyboardHandler((unsigned char)key, scancode, action, mods);
}

/*****************************************************************************************/
void createPixelCallback(float x, float y, Color color){
    pData->createPixel(x, y, color);
}

/*****************************************************************************************/
/**************************************CONSTRUCTORS***************************************/
/*****************************************************************************************/
SE_MAIN::SE_MAIN(){
    gfx = SE_GFX(240, 320);
    gfx.setCallback(createPixelCallback);
    pixelCounter = 0;
}

/*****************************************************************************************/
SE_MAIN::SE_MAIN(uint32_t sWidth, uint32_t sHeight){
    gfx = SE_GFX(sWidth, sHeight);
    gfx.setCallback(createPixelCallback);
    pixelCounter = 0;
}

/*****************************************************************************************/
SE_MAIN::~SE_MAIN(){
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
}

/*****************************************************************************************/
/**************************************INITIALISATION*************************************/
/*****************************************************************************************/
bool SE_MAIN::init(){
    return init("");
}
bool SE_MAIN::init(std::string shaderFolderPath){
    printf("Before init anything is ok.\r\n");
    glfwInit();

    #ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    window = glfwCreateWindow(gfx.getWidth(), gfx.getHeignt(), "Screen Simulator", NULL, NULL);
	if (window == NULL) {
		fprintf(stdout, "Failed to create GLFW window!\n");
		glfwTerminate();
		return false;
	}

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetFramebufferSizeCallback(window, window_size_callback);
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        fprintf(stdout, "Failed to initialize GLAD\n");
        return false;
    }

    //Shaders initialisation
    shaderLoader.addShaderPath(shaderFolderPath);
    shaderLoader.loadShaders();

    //Init vertex array
    initVertexes();

    glEnable(GL_DOUBLEBUFFER);
    glDisable(GL_DEPTH_TEST);

    #ifdef GL_SHADING_LANGUAGE_VERSION
        printf("Supported GLSL version is %s.\n", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
    #endif
    printf("Using GLEW version %s.\n", glGetString(GL_VERSION));
	printf("------------------------------\n");
	printf("Press space bar to cycle thru images.\n");
	printf("Press ESCAPE or 'X' or 'x' to exit.\n");

    pData = this;
    return true;
}

/*****************************************************************************************/
void SE_MAIN::initVertexes(){

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, gfx.getBufferSize(), 0, GL_DYNAMIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

/*****************************************************************************************/
void SE_MAIN::display(){
    static std::chrono::steady_clock::time_point prv = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point cur = std::chrono::steady_clock::now();
    const float dt = std::chrono::duration< float >( cur - prv ).count();
    prv = cur;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(shaderLoader.getProgram());
    glBindVertexArray(VAO);

    glDrawArrays(GL_POINTS, 0, pixelCounter);

    gfx.setCursor(0, 7);
    char buffer[64];
    sprintf(buffer, "FPS: %d", (int)(1000/dt*1000));
    gfx.print(buffer);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

/*****************************************************************************************/
void SE_MAIN::clear(){
    //glDeleteBuffers();
    //glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * width*height, 0, GL_DYNAMIC_DRAW);
    pixelCounter = 0;
}

/*****************************************************************************************/
bool SE_MAIN::getWindowWorker(){
    return !glfwWindowShouldClose(window);
}

/*****************************************************************************************/
void SE_MAIN::keyboardHandler(unsigned char c, int scancode, int action, int mods){
    if (action == GLFW_RELEASE) {
        printf("Pixels in array: %llu \r\n", pixelCounter);
		return;			// Ignore key up (key release) events
	}
	if (c == GLFW_KEY_ESCAPE || c == GLFW_KEY_X) {
		glfwSetWindowShouldClose(window, true);
	}
	else if (c == GLFW_KEY_SPACE) {
		//CurrentMode = (CurrentMode+1) % 6;	// Takes on values from 0 to 5
	}
}

/*****************************************************************************************/
void SE_MAIN::createPixel(float x, float y, Color color){
    Pixel pixel = {x, y, 0.0f, color};
    /*pixel.pos.x = x;
    pixel.pos.y = y;
    pixel.pos.z = 0.0f;
    pixel.color = color;*/
    setPixel(pixel);
    pixelCounter++;
}

/*****************************************************************************************/
void SE_MAIN::setPixel(Pixel pixel){
    union dataToData{
        Pixel pix;
        float floats[6];
    } data;

    data.pix = pixel;
    glBufferSubData(GL_ARRAY_BUFFER, pixelCounter*sizeof(data.floats), sizeof(data.floats), &data.floats);
}