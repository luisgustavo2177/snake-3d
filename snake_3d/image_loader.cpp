#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image_loader.h"

#pragma pack(push, 1)
typedef struct {
    unsigned short bfType;
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;
} BMPFileHeader;

typedef struct {
    unsigned int biSize;
    int biWidth;
    int biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    int biXPelsPerMeter;
    int biYPelsPerMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
} BMPInfoHeader;
#pragma pack(pop)

Image* load_image(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return NULL;
    }

    BMPFileHeader fileHeader;
    fread(&fileHeader, sizeof(BMPFileHeader), 1, file);

    if (fileHeader.bfType != 0x4D42) {
        fprintf(stderr, "Not a BMP file: %s\n", filename);
        fclose(file);
        return NULL;
    }

    BMPInfoHeader infoHeader;
    fread(&infoHeader, sizeof(BMPInfoHeader), 1, file);

    if (infoHeader.biBitCount != 24 && infoHeader.biBitCount != 32) {
        fprintf(stderr, "Unsupported bit count: %d\n", infoHeader.biBitCount);
        fclose(file);
        return NULL;
    }

    int channels = infoHeader.biBitCount / 8;
    int rowSize = ((infoHeader.biWidth * channels + 3) / 4) * 4;

    fseek(file, fileHeader.bfOffBits, SEEK_SET);

    unsigned char* data = (unsigned char*)malloc(rowSize * infoHeader.biHeight);
    fread(data, rowSize, infoHeader.biHeight, file);

    fclose(file);

    Image* image = (Image*)malloc(sizeof(Image));
    image->width = infoHeader.biWidth;
    image->height = infoHeader.biHeight;
    image->channels = channels;
    image->data = data;

    return image;
}

void free_image(Image* image) {
    if (image) {
        if (image->data) {
            free(image->data);
        }
        free(image);
    }
}
