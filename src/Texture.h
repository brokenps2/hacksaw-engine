#pragma once
#include <stddef.h>

typedef struct Texture {
    int w;
    int h;
    int channels;
    unsigned char* data;
    unsigned int id;


} Texture;

void gtmaCreateTexture(Texture* tex, const char* path);
void gtmaLoadTextureFromMemory(Texture* texture, const unsigned char* buffer, size_t size);
void gtmaDeleteTexture(Texture* tex);
