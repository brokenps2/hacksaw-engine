#include "Audio.h"
#include "Camera.h"
#include "Input.h"
#include "Menus.h"
#include "Models.h"
#include "Renderer.h"
#include "Shader.h"
#include <cglm/vec3.h>
#include <stdio.h>

Camera camera;
vec3 camPos = {4, 7, 4};
vec3 soundPos = {0, 0, 0};

Object plane;
Object table;
Object mario;
Object radio;
Object sky;

Object yard;

Sound testSound;

PointLight light1;
PointLight light2;
PointLight light3;
PointLight light4;

void initScene() {
    gtmaCreateSound(&testSound, "audio/test2.wav", true, 2, soundPos);
    //playSoundFrom(&testSound, 5);

    gtmaCreateCamera(&camera, getWindowWidth(), getWindowHeight(), camPos);
    gtmaSetRenderCamera(&camera);

    gtmaCreateObject(&plane, "models/plane.glb", 0, 0, 0,    8, 8, 8,    0, 0, 0);
    gtmaCreateObject(&table, "models/table.glb", -16, 0, 0,    1, 1, 1,    0, 0, 0);
    gtmaCreateObject(&mario, "models/mario.glb", 7, 1.2, -3, 1, 1, 1,    0, 0, 0);
    gtmaCreateObject(&sky,   "models/sky.glb",   3, 3, 3,    2.5, 2.5, 2.5,    0, 0, 0);

    gtmaCreateObject(&yard, "models/yard.glb", 0, 0, 0, 10, 10, 10, 0, 0, 0);

    for(int i=0; i < sky.model.meshCount; i++) {
        sky.model.meshes[i].lit = false;
    }

    gtmaCreatePointLight(&light1, 0, 200, 500, 0.6, 0.6, 0.6);
    light1.sunMode = true;
    gtmaCreatePointLight(&light2, 500, 200, 0, 0.67, 0.67, 0.67);
    light2.sunMode = true;
    gtmaCreatePointLight(&light3, 0, 200, -500, 1, 1, 1);
    light3.sunMode = true;
    gtmaCreatePointLight(&light4, -20, 8, 0, 0.6, 0.6, 0.6);
    light4.sunMode = false;

    //addObject(&table);
    gtmaAddObject(&plane);
    //addObject(&yard);
    //gtmaAddObject(&sky);
    //addLight(&light1);
    //addLight(&light2);
    //addLight(&light3);
    gtmaAddLight(&light4);


}

void updateScene() {
    gtmaCameraMatrix(&camera, 67.0f, 0.1f, 200.0f, gtmaGetShader(), "camMatrix");
    if(!getIOPtr()->WantCaptureMouse) {
        gtmaCameraMove(&camera);
    }

    glm_vec3_copy(camera.position, sky.position);

    printf("\r%f  %f  %f", camera.position[0], camera.position[1], camera.position[2]);
    fflush(stdout);
 
    gtmaUpdateAudio(camera.position, camera.direction);

    //glm_vec3_copy(light4.position, table.position);
    
    light4.position[0] += (sin(glfwGetTime())) / 8;
    light4.position[2] += (cos(glfwGetTime())) / 8;

}

void disposeScene() {
    gtmaDeleteObject(&plane);
    gtmaDeleteObject(&table);
    gtmaDeleteObject(&mario);
    gtmaDeleteObject(&sky);
    gtmaDeleteObject(&yard);
}
