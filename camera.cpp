#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>
#include <glm/ext.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/io.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <cmath>
#include <vector>
class Camera {
public:
  Camera(){};
  Camera(GLFWwindow* window, int windowWidth, int windowHeight) : WINDOW_WIDTH(windowWidth), WINDOW_HEIGHT(windowHeight), camPos(0, 0, 3), angle(0, 0) {
    _window = window;
    glfwSetCursorPos(_window, WINDOW_WIDTH/2.0, WINDOW_HEIGHT/2.0);
  }

  void update() {
    if (glfwGetKey(_window, GLFW_KEY_W)) {
      auto front = camFront();
      front.y = 0;
      front = glm::normalize(front);
      camPos += front * camSpeed * deltaTime;
    }
    if (glfwGetKey(_window, GLFW_KEY_S)){
      auto front = camFront();
      front.y = 0;
      front = glm::normalize(front);
      camPos -= front * camSpeed * deltaTime;
    }
    if (glfwGetKey(_window, GLFW_KEY_A)) {
      camPos -= camRight() * camSpeed * deltaTime;
    }
    if (glfwGetKey(_window, GLFW_KEY_D)) {
      camPos += camRight() * camSpeed * deltaTime;
    }
    if (glfwGetKey(_window, GLFW_KEY_SPACE)) {
        camPos += up * camSpeed * deltaTime;
    }
    if (glfwGetKey(_window, GLFW_KEY_LEFT_SHIFT)){
        camPos -= up * camSpeed * deltaTime;
    } 
  }
  glm::vec3 camRight() {
    return glm::normalize(cross(camFront(), up));
  }

  glm::mat4 myLookAt2(){
    glm::vec4 defdir{0,0,-1, 0};
    auto id = glm::mat4(1);
    auto r = glm::rotate(id, glm::radians(-angle.x), glm::vec3(0,1,0));
    r = glm::rotate(r, glm::radians(-angle.y), glm::vec3(1,0,0));

    glm::vec3 dir(-glm::normalize(r * defdir));
    glm::vec3 right = glm::normalize(glm::cross({0,1,0}, dir));
    glm::vec3 yup = glm::normalize(glm::cross(dir, right));
    glm::mat4 m1(
        right.x, right.y, right.z, 0,
        yup.x, yup.y, yup.z, 0,
        dir.x, dir.y, dir.z, 0,
        0,0,0,1
        );
    m1 = glm::inverse(m1);

    glm::mat4 m2(
        1,0,0,-camPos.x,
        0,1,0,-camPos.y,
        0,0,1,-camPos.z,
        0,0,0,1);
    m2 = glm::transpose(m2);
    m2 = glm::mat4(1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        -camPos.x, -camPos.y, -camPos.z,1);
    return m1 * m2;
  }

  glm::mat4 myLookAt(glm::vec3 campos, glm::vec3 camfront, glm::vec3 worldup){
    glm::vec3 dir = glm::normalize(campos - camfront);
    glm::vec3 right = glm::normalize(glm::cross(worldup, dir));
    glm::vec3 yup = glm::normalize(glm::cross(dir, right));
    glm::mat4 m1(
        right.x, right.y, right.z, 0,
        yup.x, yup.y, yup.z, 0,
        dir.x, dir.y, dir.z, 0,
        0,0,0,1
        );
    m1 = glm::transpose(m1);
    glm::mat4 m2(1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        -campos.x, -campos.y, -campos.z,1);
    return m1 * m2;
  }
  int cnter = 0;
  glm::mat4 getLookAt() {
    return myLookAt2();
    return myLookAt(camPos, camFront() + camPos, up);
  }

  bool firsttime = true;
  void mouseMove(double xpos, double ypos) {
    glfwSetCursorPos(_window, WINDOW_WIDTH/2.0, WINDOW_HEIGHT/2.0);
    if (firsttime) {
      firsttime = false;
      return;
    }

    glm::vec2 center{WINDOW_WIDTH/2.0, WINDOW_HEIGHT/2.0};
    glm::vec2 delta = center - glm::vec2{xpos, ypos};
    angle -= delta * sensitivity;
    static float alimit = 89;
    if (angle.y > alimit)
      angle.y = alimit;
    if (angle.y < -alimit)
      angle.y = -alimit;
    // std::cout << "in cam: " << delta.x << " " << delta.y << '\n';
  }
  glm::vec3 camFront() {
    // default dir is facing negative z-axis
    glm::vec4 front{0,0,-1,0};
    auto id = glm::mat4(1);
    auto r = glm::rotate(id, -glm::radians(angle.x), glm::vec3(0,1,0));
    r = glm::rotate(r, -glm::radians(angle.y), glm::vec3(1,0,0));
    return glm::normalize(r * front);
  }
  glm::mat4 perspectiveMatrix(float horizontalFov, float aspectRatio, float n, float f) {
    float r = tan(horizontalFov/2) * n;
    float l = -r;
    float h = 1/aspectRatio * (r - l);
    float t = h/2;
    float b = -t;
    glm::mat4 projection(
        2.0f * n / (r - l), 0, 0, 0,
        0, 2.0f * n / (t - b), 0, 0,
        (r + l) / (r - l), (t + b) / (t - b), (f + n) / (n - f), -1,
        0, 0, 2 * n * f / (n - f), 0
        );
    return projection;
  }
  glm::mat4 getProjection() {
    float horizontalFov = 90.0f; // degrees
    float aspectRatio = 1.0f * WINDOW_WIDTH / WINDOW_HEIGHT;
    float near = 0.1f, far = 100.0f;
    float zoom = 1/orthoZoom;
    return perspectiveMatrix(glm::radians(horizontalFov), aspectRatio, near, far);
    return orthographicMatrix(aspectRatio, zoom, near, far);
    return glm::ortho(-aspectRatio*zoom, aspectRatio*zoom, -zoom, zoom, near, far);
    auto projection = glm::perspective(glm::radians(horizontalFov), aspectRatio, near, far);
    return projection;
  }
  glm::mat4 orthographicMatrix(float ar, float zoom, float n, float f) {
    float r = ar*zoom;
    float l = -r;
    float t = zoom;
    float b = -t;
    glm::mat4 projection(
        2/(r-l), 0, 0, 0,
        0, 2/(t-b), 0, 0,
        0, 0, 2/(n-f), 0,
        (-r-l)/(r-l), (-t-b)/(t-b), (n+f)/(n-f), 1
        );
    return projection;
  }
  GLFWwindow* _window;
  glm::vec3 camPos;
  glm::vec2 angle;
  glm::vec3 up{0, 1, 0};
  float camSpeed = 20;
  float sensitivity = 0.05f;
  float orthoZoom = 0.1;
  float deltaTime = 0.0f;
  float lastFrame = 0.0f;
  int WINDOW_WIDTH;
  int WINDOW_HEIGHT;
private:
};
