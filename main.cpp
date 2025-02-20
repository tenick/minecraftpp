#include "stb_image.h"
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <sys/types.h>
#include <vector>
#include <math.h>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include "camera.cpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
  glViewport(0, 0, width, height);
}
unsigned int VAO;
bool isBound = false;
bool isKeydown = false;
Camera camera;
double lastX = 0, lastY = 0,cnter=0.1;
bool mouseset = false;
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
  camera.mouseMove(xpos, ypos);
}
void processInput(GLFWwindow *window) {
  camera.update();
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
    if (!isKeydown) {
      isKeydown = true;
      if (isBound)
        glBindVertexArray(0);
      else
        glBindVertexArray(VAO);
      isBound = !isBound;
    }
  }
  if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE)
    isKeydown = false;
}

unsigned int create_texture(const std::string& fileName, GLenum activeTexture, GLint format = GL_RGB) {
  unsigned int texture;
  glGenTextures(1, &texture);
  glActiveTexture(activeTexture);
  glBindTexture(GL_TEXTURE_2D, texture);
  // set the texture wrapping/filtering options (on the currently bound texture object)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load and generate the texture
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);
  if (data)
  {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
      std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);
  return texture;
}
unsigned char* load_noise() {
  int width, height, nrChannels;
  unsigned char *data = stbi_load("noiseTexture.png", &width, &height, &nrChannels, STBI_grey);
  std::cout << "da noise uh: " << width << " " << height << " " << nrChannels << std::endl;
  std::cout << "Length of array = " << (sizeof(data)/sizeof(*data)) << std::endl;
  return data;
  for (int i = 0; i < width * height; i++) {
    std::cout << int(data[i]) << std::endl;
  }
  stbi_image_free(data);
}

float linearMap(float froma, float fromb, float toa, float tob, float x) {
  float m = (tob - toa) / (fromb - froma);
  return m * (x - fromb) + tob; 
}

const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;
int main() {
  std::cout << "hello world!!\n";
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "pracopengl", NULL, NULL);
  if (window == NULL) {
    std::cout <<"Failed to create GLFW window\n";
    glfwTerminate();
    return -1;
  }
  camera = Camera(window, WINDOW_WIDTH, WINDOW_HEIGHT);
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    glfwTerminate();
    return -1;
  }

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  // finish initialization

  // auto data = load_noise();
  // create VAO
  // unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  unsigned int vbo;
  glGenBuffers(1, &vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);


  // store data to bound array buffer, vbo
  // float vertices[] = {
    // -.5f, -.5f, .0f, 1, 0, 0,  .0f, .0f,
     // .5f, -.5f, .0f, 0, 1, 0, 1.0f, .0f,
     // .0f,  .5f, .0f, 0, 0, 1,  .5f, 1.0f
  // };
  float verticesbox[] = { 
    -.5f, .5f, .0f, 1, 0, 0,  .0f, 1.0f,
     .5f, .5f, .0f, 0, 1, 0, 1.0f, 1.0f,
    .5f, -.5f, .0f, 0, 0, 1, 1.0f, .0f,
    -.5f,-.5f, .0f, 1, 1, 0, .0f,  .0f
  };
  float verticesCube[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCube), verticesCube, GL_STATIC_DRAW);

  unsigned int EBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  unsigned int indices[] = {
    0, 1, 2,
    0, 3, 2
  };
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // create then use texture
  create_texture("dirtgrass.jpg", GL_TEXTURE0);
  create_texture("grass.jpg", GL_TEXTURE1);
  create_texture("dirt.jpg", GL_TEXTURE2);

  // create vertex shader object
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);

  // define shader source code, attach it to the shader object, then compile shader
  const char *vertexShaderSource = R"(
    #version 400 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;
    layout (location = 2) in vec2 aTexPos;

    out vec4 vertexColor;
    out vec2 texCoord;

    uniform vec4 ourColor;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main()
    {
       vec4 res = projection * view * model * vec4(aPos, 1.0);
       gl_Position = res;
       // vertexColor = vec4(1, 0, 0, 1);
       vertexColor = vec4(aColor, 1);
       texCoord = aTexPos;
    }
  )";
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  const char *fragmentShaderSource = R"(
    #version 400 core
    in vec4 vertexColor;
    in vec2 texCoord;

    out vec4 FragColor;

    uniform vec4 ourColor;
    uniform sampler2D texture1;
    uniform sampler2D texture2;

    void main()
    {
        FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
        FragColor = vertexColor + ourColor;
        // FragColor = texture(texture1, texCoord) * (vertexColor + ourColor );
        // FragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.4);
        FragColor = texture(texture1, texCoord);
        // FragColor = ourColor;
    }
  )";
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  // linking vertex attributes
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);  

  // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  // glEnableVertexAttribArray(1);  

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // create shader program
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);

  // bind textures to uniforms
  glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
  glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1);


  // unbind VAO (finish recording vertex attribute calls)
  glBindVertexArray(0);

  // define different cube models
  std::vector<glm::vec3> cubePositions = {
    glm::vec3( 0.0f,  0.0f,  0),
    glm::vec3( 0.0f,  0.0f,  -10),
  };

  // sine 1d
  // for (int i = 0; i < 180; i++) {
    // cubePositions.push_back({i, 0, round(5*sin(0.2*i))});
  // }

  // sine2
  // for (int i = 0; i < 180; i++) {
    // for (int j = 0; j < 120; j++) {
      // float y2 = round(3*sin(j*0.2));
      // cubePositions.push_back({i, y2, j});
    // }
  // }

  // sine3
  // for (int i = 0; i < 180; i++) {
    // float y2 = 3*sin(0.1*i);
    // for (int j = 0; j < 120; j++) {
      // float y = 3*sin(0.1*j);
      // float ry = round(y * y2);
      // cubePositions.push_back({i, ry, j});
    // }
  // }

  // perlin noise
  auto data = load_noise();
  int maxd = 0;
  int mind = 1e9;
  for (int i = 0; i < 120*180; i++) {
    int pixval = data[i];
    maxd = std::max(maxd, pixval);
    mind = std::min(mind, pixval);
  }

  for (int i = 0; i < 120; i++) {
    for (int j = 0; j < 180; j++) {
      int idx = i * 180 + j;
      int pixval = (int)data[idx];
      float ry = linearMap(mind, maxd, 0, 13, pixval);

      cubePositions.push_back({i, round(ry), j});
    }
  }
  // keep track of time
  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  // enable depth-testing
  glEnable(GL_DEPTH_TEST);

  float r=0.5, g=0.1, b=0.2;
  while (!glfwWindowShouldClose(window)) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;  
    camera.deltaTime = deltaTime;
    std::cout << "deltatime = " << deltaTime << std::endl;

    processInput(window);
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    r += 0.001f;
    g += 0.005f;
    b += 0.0001f;
    if (r > 1) r = 0;
    if (g > 1) g = 0;
    if (b > 1) b = 0;

    // uniform stuffs
    float timeValue = glfwGetTime();
    float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
    int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);


    for (int i = 0; i < cubePositions.size(); i++) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
      
      glm::mat4 view = camera.getLookAt();

      glm::mat4 projection = camera.getProjection();

      glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
      glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
      glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

      // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

      // update textures per cube face
      glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 1);
      glDrawArrays(GL_TRIANGLES, 0, 6);
      glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 2);
      glDrawArrays(GL_TRIANGLES, 6, 6);
      glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
      glDrawArrays(GL_TRIANGLES, 12, 24);
    }

    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  std::cout << "bye world!!\n";
  glfwTerminate();
  return 0;
  
}
