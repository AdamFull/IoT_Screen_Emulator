# IoT_Screen_Emulator

[![forthebadge](https://forthebadge.com/images/badges/made-with-c-plus-plus.svg)](https://forthebadge.com)
[![forthebadge](https://forthebadge.com/images/badges/built-with-love.svg)](https://forthebadge.com)

[![Build Status](https://travis-ci.org/AdamFull/IoT_Screen_Emulator.svg?branch=main)](https://travis-ci.org/AdamFull/IoT_Screen_Emulator) 
[![GitHub Issues](https://img.shields.io/github/issues/AdamFull/IoT_Screen_Emulator.svg)](https://github.com/AdamFull/IoT_Screen_Emulator/issues)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)

It's util for debugging embeeded projects with some gui's.

Emulator has all methods, that have Adafruit screen libraries, and more another.

With this util you can debug your gui's on your pc, that is more efficient then on hardware.

Installation

-----

* Create subdirrectory named "screen_emulator" in your project.

* Pull project files to this derrectory.

* In your cmake file add:

```cmake
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/screen_emulator/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/screen_emulator/)

#and link with your project
target_link_libraries(YOUR_PROJECT_NAME IOT_SCREEN_EMULATOR)
```

Requirements

-----

* gcc5 and higher

* Installed freeglut3 and higher

* Installed glew-2.x.x and higher

* Installed glfw-3.x.x and higher

* GLSL version support 330 core

Usage

-----

```C++

int main(){
    uint32_t width = 240, height = 320;

    //Create screen object
    SE_MAIN emu(width, height);
    //Init with your shader folder (If shader dirrectory in the same path with executable, you don't need type path, just init())
    if(emu.init("..\\screen_emulator\\shaders\\")){
        while(emu.getWindowWorker()){
            emu.clear();

            //Draw graphics here
            emu.gfx.drawLine(0, 0, 5, height-1, getRandomColor())
            //...
            
            emu.display();
        }
    }

    return 0;
}

```

License

-------

MIT © [AdamFull](https://github.com/AdamFull).