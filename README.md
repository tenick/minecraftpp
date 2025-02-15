# minecraftpp

very WIP minecraft clone using C++, OpenGL, GLFW

OS: Linux Ubuntu 24.04

- sudo apt update
- sudo apt install build-essential
- sudo apt install cmake
- download "Source Package" in https://www.glfw.org/download.html
- `unzip <package_name>.zip`
- cd into the unzipped folder, create `build/` folder and cd into that
- install [Wayland dependencies](https://www.glfw.org/docs/latest/compile_guide.html#compile_deps_wayland) `sudo apt install libwayland-dev libxkbcommon-dev xorg-dev`
- run `cmake -G "Unix Makefiles" ../` then `make` then `sudo make install`
- `sudo apt-get install libx11-dev libglu1-mesa-dev freeglut3-dev libglew-dev libgl1 libglx-mesa0 libglfw3-dev libglfw3`
- generate GLAD files [here](https://glad.dav1d.de/), make sure `API > gl` version is `4.0` and `Profile` is set to `Core`, and `Generate a loader` is checked. Then click GENERATE.
- download `glad.zip`, unzip, then move the `include/` folder to `/usr/include/`, and copy the `src/glad.c` file into this repository's root
- download GLM headers [here (Source code (zip))](https://github.com/g-truc/glm/releases), then just move the `glm/` folder into includes path (usually in `/usr/include/`) 
Compilation: `g++ *.cpp glad.c -o main -Wall -lGL -lGLU -lglut -lGLEW -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor`
