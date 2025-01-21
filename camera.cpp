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
      camPos += front * camSpeed;
    }
    if (glfwGetKey(_window, GLFW_KEY_S)){
      auto front = camFront();
      front.y = 0;
      front = glm::normalize(front);
      camPos -= front * camSpeed;
    }
    if (glfwGetKey(_window, GLFW_KEY_A)) {
      camPos -= camRight() * camSpeed;
    }
    if (glfwGetKey(_window, GLFW_KEY_D)) {
      camPos += camRight() * camSpeed;
    }
    if (glfwGetKey(_window, GLFW_KEY_SPACE)) {
        camPos += up * camSpeed;
    }
    if (glfwGetKey(_window, GLFW_KEY_LEFT_SHIFT)){
        camPos -= up * camSpeed;
    } 
  }
  glm::vec3 camRight() {
    return glm::normalize(cross(camFront(), up));
  }

  glm::mat4 myLookAt2(glm::vec3 campos, glm::vec3 camfront, glm::vec3 worldup){
    // default dir is facing negative z-axis
    glm::vec4 front{0,0,-1,0};
    auto id = glm::mat4(1);
    auto r = glm::rotate(id, -glm::radians(angle.x), glm::vec3(0,1,0));
    r = glm::rotate(r, -glm::radians(angle.y), glm::vec3(1,0,0));
    glm::vec3 dir = glm::normalize(campos - camfront);
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
    return myLookAt(camPos, camFront() + camPos, up);
    auto d = camFront();
    auto r = camRight();
    auto u = glm::cross(d, r);
    // glm::mat4 lookat(r.x, r.y, r.z, -camPos.x,
        // u.x, u.y, u.z, -camPos.y,
        // d.x, d.y, d.z, -camPos.z,
        // 0, 0, 0, 1);
    glm::vec4 w{-camPos, 1};
    glm::mat4 lookat(
        glm::vec4(r,0), glm::vec4(u, 0), glm::vec4(d, 0), w
        );
    // auto actualLookAt = glm::lookAt(camPos, camFront() + camPos, up);
    glm::vec4 tpos{1.0,2,3,0};
    glm::vec4 tdir{4.0,5,6,0};
    glm::vec4 tup{0.0,0,1,0};
    glm::vec4 ww{3.0,6,9,1};
    auto tst1 = glm::mat4(tpos,tdir,tup,ww);
    glm::mat4 tst2(
        tpos.x, tpos.y, tpos.z, tpos.w,
        tdir.x, tdir.y, tdir.z, tdir.w,
        tup.x, tup.y, tup.z, tup.w,
        ww.x, ww.y, ww.z, ww.w
        );
    tst2 = glm::transpose(tst2);
    auto actualLookAt = glm::lookAt(glm::vec3(tpos), glm::vec3(tpos+tdir),glm::vec3(tup));
    auto mylookat = myLookAt(glm::vec3(tpos), glm::vec3(tpos+tdir),glm::vec3(tup));

    if (cnter >= 5000) {
      // std::cout << "da passed params:\n";
      // std::cout << glm::to_string(t1) << '\n';
      // std::cout << glm::to_string(t2) << '\n';
      // std::cout << glm::to_string(t3) << '\n';
      // std::cout << "tst1:\n";
      // std::cout << tst1 << '\n';
      // std::cout << glm::to_string(tst1) << '\n';
      // std::cout << "tst2:\n";
      // std::cout << tst2 << '\n';
      // std::cout << glm::to_string(tst2) << '\n';
      std::cout << "resulting lookat:\n";
      std::cout << actualLookAt << '\n';
      // std::cout << glm::to_string(actualLookAt) << '\n';
      std::cout << "MY lookat:\n";
      std::cout << mylookat << '\n';
      // std::cout << glm::to_string(mylookat) << std::endl;
      cnter = 0;
    }
    cnter++;
    return mylookat;
    return actualLookAt;
    // return glm::lookAt(camPos, camFront() + camPos, up);
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
    std::cout << "in cam: " << delta.x << " " << delta.y << '\n';
  }
  glm::vec3 camFront() {
    // default dir is facing negative z-axis
    glm::vec4 front{0,0,-1,0};
    auto id = glm::mat4(1);
    auto r = glm::rotate(id, -glm::radians(angle.x), glm::vec3(0,1,0));
    r = glm::rotate(r, -glm::radians(angle.y), glm::vec3(1,0,0));
    return glm::normalize(r * front);
  }
  GLFWwindow* _window;
  glm::vec3 camPos;
  glm::vec2 angle;
  glm::vec3 up{0, 1, 0};
  float camSpeed = 0.1;
  float sensitivity = 0.05;
  int WINDOW_WIDTH;
  int WINDOW_HEIGHT;
private:
};
