//author: eli
//created: 1-6-2024
//renderer.cpp opengl renderer

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include "Shader.h"
#include "Files.h"

float vertices[] = {
 
  -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
  0.5f, 0.4f, 0.0f,   0.4f, 0.0f, 1.0f,
  0.5f, -0.4f, 0.0f,  0.0f, 0.0f, 1.0f

};
unsigned int indices[] = {
  0, 2, 1,
  1, 2, 3
};
float texCoords[] = {
  0.0f, 0.0f,  // lower-left corner  
  1.0f, 0.0f,  // lower-right corner
  0.5f, 1.0f   // top-center corner
};

GLuint shaderProgram;

GLuint VBO;
GLuint VAO;
GLuint EBO;

hsTexture texture = hsTexture("res/cool.png");

Shader shader = Shader(0);


void initRenderer() {

  shader.initialize();

  //generate vao & vbo
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glGenTextures(1, &texture.id);

  //bind vao
  glBindVertexArray(VAO);

  //bind array buffer to vbo
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  //add trianngle to vbo
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  //bind and initialize index buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  //tell gl how to read vbo
  //             attrib #, size, type, norm,   
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  //enable vbo
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
  glEnableVertexAttribArray(1);

  //bind vbo
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glBindTexture(GL_TEXTURE_2D, texture.id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture.data);
  
  //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  //glEnableVertexAttribArray(2); 

  texture.dispose();

}

void render() {

  shader.use();
  //glBindTexture(GL_TEXTURE_2D, texture.id);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


}
