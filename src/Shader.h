#pragma once

#include <GL/glew.h>
#include <string>
#include "FileLoader.h"

class Shader {

public:

  GLuint vertexShader;
  GLuint fragmentShader;

  GLuint shaderProgram;

  int sType;

  std::string vtShaderValue;
  const char* vtShaderSrc;
  
  std::string frShaderValue;
  const char* frShaderSrc;

  Shader(int type);

  void initialize();

  void use();

  void setBool(const std::string &name, bool value) const;  
  void setInt(const std::string &name, int value) const;   
  void setFloat(const std::string &name, float value) const;

};
