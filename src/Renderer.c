#include <cglm/vec3.h>
#include <stb_image.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/common.h>
#include <cglm/types.h>
#include "Audio.h"
#include "Input.h"
#include "Shader.h"
#include "Camera.h"
#include "WindowManager.h"
#include "Texture.h"
#include "Models.h"

Shader shader;
Camera camera;
vec3 camPos = {4, 4, 4};
vec3 soundPos = {0, 0, 0};
vec3 lightPos;

float frameVertices[] = {
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
    1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
    1.0f, -1.0f,  1.0f, 0.0f,
    1.0f,  1.0f,  1.0f, 1.0f
};

unsigned int fbo;
unsigned int rbo;
unsigned int textureColorbuffer;
unsigned int pvao;
unsigned int pvbo;

Texture tile;
Object plane;

Object screen;
Texture screentex;

Sound testSound;

time_t t;

void initRenderer() {

    glGenVertexArrays(1, &pvao);
    glGenBuffers(1, &pvbo);
    glBindVertexArray(pvao);
    glBindBuffer(GL_ARRAY_BUFFER, pvbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(frameVertices), frameVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 384, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 512, 384);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    createSound(&testSound, "test2.wav", true, 2, soundPos);
    playSoundFrom(&testSound, 5);

    createShader(&shader);
    createCamera(&camera, getWindowWidth(), getWindowHeight(), camPos);

    createTexture(&tile, "tile.png");
    createObject(&plane, &tile, "plane.obj", 0, 0, 0,    4, 4, 4,    0, 0, 0);

    glfwSetInputMode(getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    lightPos[0] = -7;
    lightPos[1] = 10;
    lightPos[2] = -10;

}

void renderScreen() {

    glViewport(0, 0, getWindowWidth(), getWindowHeight());

    glDisable(GL_DEPTH_TEST);
    useShader(&shader);
    setBool(&shader, "frame", true);
    glBindVertexArray(pvao);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glDrawArrays(GL_TRIANGLES, 0, 6);

}

void renderObject(Object* object) {

    glViewport(0, 0, 512, 384);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    
    glBindVertexArray(object->model.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, object->model.VBO);
    glBufferData(GL_ARRAY_BUFFER, (object->model.indexCount) * sizeof(Vertex), object->model.vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->model.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * object->model.indexCount, object->model.indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(2);

    mat4 transformationMatrix;
    loadTransformationMatrix(&transformationMatrix, object);
    setMatrix(&shader, "transMatrix", transformationMatrix);

    useShader(&shader);
    setBool(&shader, "lightEnabled", object->model.lit);
    setBool(&shader, "frame", false);

    setVec3(&shader, "viewPos", camera.pos);
    setVec3(&shader, "lightPos", lightPos);

    glBindTexture(GL_TEXTURE_2D, object->model.texture.id);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDrawElements(GL_TRIANGLES, object->model.indexCount, GL_UNSIGNED_INT, 0);

}

void render() {

    cameraMatrix(&camera, 67.0f, 0.1f, 200.0f, &shader, "camMatrix");
    cameraMove(&camera);
    printf("\r%f  %f  %f", camera.pos[0], camera.pos[1], camera.pos[2]);
    fflush(stdout);

    updateAudio(camera.pos, camera.direction);
    renderObject(&plane);
    renderScreen();
}
