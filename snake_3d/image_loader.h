#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

typedef struct {
    int width;
    int height;
    int channels;
    unsigned char* data;
} Image;

Image* load_image(const char* filename);
void free_image(Image* image);

#endif