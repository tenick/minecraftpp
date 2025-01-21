# minecraftpp

very WIP minecraft clone using C++, OpenGL, GLFW

OS: Linux Ubuntu 24.04

- sudo apt update
- sudo apt install build-essential
- sudo apt install cmake
- sudo apt install xorg-dev
- download "Source Package" in https://www.glfw.org/download.html
- `unzip package.zip`
- cd into the unzipped folder, create `build/` folder and cd into that
- run `cmake -G "Unix Makefiles" ../` then `make` then `sudo make install`
- `sudo apt-get install libx11-dev libglu1-mesa-dev freeglut3-dev libglew-dev libgl1 libglx-mesa0 libglfw3-dev libglfw3`

Compilation: `g++ *.cpp glad.c -o main -Wall -lGL -lGLU -lglut -lGLEW -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor`
