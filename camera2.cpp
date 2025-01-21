#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/constants.hpp>
#include <iostream>
class Camera2 {
public:
  Camera2(){};
  Camera2(GLFWwindow* window, int windowWidth, int windowHeight) : WINDOW_WIDTH(windowWidth), WINDOW_HEIGHT(windowHeight), camPos(0, 0, 3), camTarget(0, 0, 0) {
    _window = window;
    prevMousePos = {WINDOW_WIDTH/2, WINDOW_HEIGHT/2};
    // glfwSetCursorPos(window, prevMousePos.x, prevMousePos.y);
    // camPos = {0,0,3};
    // camTarget = {0, 0, 0};
    // std::cout << "the default... "<< camPos.x << camPos.y << camPos.z << std::endl;
  }

  void update() {
    glm::vec3 posdelta;
    // posdelta.w = 1;
    if (glfwGetKey(_window, GLFW_KEY_W)) {
      posdelta.z+=0.1f;
    }
    if (glfwGetKey(_window, GLFW_KEY_S)){
      posdelta.z-=0.1f;
    }
    if (glfwGetKey(_window, GLFW_KEY_A)) {
      posdelta.x+=0.1f;
    }
    if (glfwGetKey(_window, GLFW_KEY_D)) {
      posdelta.x-=0.1f;
    }
    if (glfwGetKey(_window, GLFW_KEY_SPACE)) {
        posdelta.y-=0.1f;
    }
    if (glfwGetKey(_window, GLFW_KEY_LEFT_SHIFT)){
        posdelta.y+=0.1f;
    } 
    // glm::mat4 view = glm::mat4(1.0f);
    // view = glm::rotate(view, angle.x, glm::vec3{0,1.0f,0});
    // view = glm::rotate(view, angle.y, glm::vec3{1.0f,0,0});
    
    // pos += glm::vec3{posdelta * view};
    pos += posdelta;
  }
  bool first = true;
  void mouseMove(double xpos, double ypos) {
    return;
    // glfwSetCursorPos(_window, prevMousePos.x, prevMousePos.y);
    glm::vec2 newPos{xpos, ypos}; 
    if (first) {
      first = false;
      prevMousePos = newPos; 
    }
    glm::vec2 delta = (prevMousePos - newPos);
    delta.x /= WINDOW_WIDTH;
    delta.y /= WINDOW_HEIGHT;
    std::cout << "in mouse move: " << delta.x << " " << delta.y << '\n';
    angle -= delta;
    float alimit = glm::pi<float>()/2-.1f;
    if (angle.y > alimit)
      angle.y = alimit;
    else if (angle.y < -alimit)
      angle.y = -alimit;
  }
  glm::vec3 pos{0.0f, 0.0f, -3.0f};
  glm::vec2 angle{0, 0};
  glm::vec2 prevMousePos{0, 0};
private:
  GLFWwindow* _window;
  glm::vec3 camTarget;
  glm::vec3 camPos;
  int WINDOW_WIDTH;
  int WINDOW_HEIGHT;
};
