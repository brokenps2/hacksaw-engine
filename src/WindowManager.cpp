#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstring>
#include "Config.h"
#include "Math.h"

GLFWwindow* window;

void initWindow() {
 
  std::string why = cfgGetTitle(); // whhhhyyyyy do you need to dooo thisssss
  char* ctitle = (char*)why.c_str();

  window = glfwCreateWindow(cfgGetResX(), cfgGetResY(), ctitle, NULL, NULL);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 0);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwMakeContextCurrent(window);
  glfwSetWindowPos(window, 800, 600);

  const GLenum err = glewInit();
  glewExperimental = GL_TRUE;
  if (GLEW_OK != err) {
    std::cout << "GLEW Error: " << glewGetErrorString(err) << std::endl;
    exit(1);
  }

  glViewport(0, 0, cfgGetResX(), cfgGetResY());
  glEnable(GL_DEPTH_TEST);
}

GLFWwindow* getWindow() {
  return window;
}

void updateWindow() {
  glfwSwapBuffers(window);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, (GLint)cfgGetResX(), (GLint)cfgGetResY());
  glClearColor(glc(135), glc(206), glc(235), 1);
}

