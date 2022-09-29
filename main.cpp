#include <iostream>
#include <cstdint> // for specific size integers
#include <fstream> // for file handling
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <process.h>
#include <string.h>
#include <math.h>

using namespace std;

struct BmpHeader {
    char bitmapSignatureBytes[2] = {'B', 'M'};
    uint32_t sizeOfBitmapFile = 54 + 476560; //920x518
    uint32_t reservedBytes = 0;
    uint32_t pixelDataOffset = 54;
} bmpHeader;

struct BmpInfoHeader {
    uint32_t sizeOfThisHeader = 40;
    int32_t width = 512; // in pixels
    int32_t height = 512; // in pixels
    uint16_t numberOfColorPlanes = 1; // must be 1
    uint16_t colorDepth = 24;
    uint32_t compressionMethod = 0;
    uint32_t rawBitmapDataSize = 0; // generally ignored
    int32_t horizontalResolution = 3780; // in pixel per meter
    int32_t verticalResolution = 3780; // in pixel per meter
    uint32_t colorTableEntries = 0;
    uint32_t importantColors = 0;
} bmpInfoHeader;

struct Pixel {
    uint8_t blue = 0;
    uint8_t green = 0;
    uint8_t red = 255;
} pixel;
int licznik=0;

int test()
{
    cout<<"Test"<<endl;

}
//dodalam nowy komentarz
int main(int argc, char **argv) {
     FILE * f = fopen("test.bmp", "rb");
     FILE * f1 = fopen("wynik.bmp", "wb");
     unsigned char bmppad[3] = {
        0,
        0,
        0
    };
     if (!f) {
        printf("Nie moge otworzyc pliku!\n");
        exit(0);
    }
    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f);
    int width = * (int * ) & info[18];
    int height = * (int * ) & info[22];

    cout<<"szerokosc: "<<width<<endl;
    cout<<"wysokosc: "<<height<<endl;
    cout<<"wielkosc naglowka: "<<bmpInfoHeader.sizeOfThisHeader<<endl;
    cout<<"liczba warstw kolorow: "<<bmpInfoHeader.numberOfColorPlanes<<endl;
    cout<<"liczba bitow na pixel: "<<bmpInfoHeader.colorDepth <<endl;
    cout<<"pozioma rozdzielczosc (px/m): "<<bmpInfoHeader.horizontalResolution<<endl;
    cout<<"pionowa rozdzielczosc (px/m): "<<bmpInfoHeader.verticalResolution<<endl;


    bmpInfoHeader.height = height;
    bmpInfoHeader.width = width;
    bmpHeader.sizeOfBitmapFile = 54+ height*width;

    ofstream fout("output.bmp", ios::binary);

    fout.write((char *) &bmpHeader, 14);
    fout.write((char *) &bmpInfoHeader, 40);

    // writing pixel data
    size_t numberOfPixels = bmpInfoHeader.width * bmpInfoHeader.height;
    for (int i = 0; i < numberOfPixels; i++)
        {
            fout.write((char *) &pixel, 3);

        }
    fout.close();

    unsigned char * img = NULL;
    if (img)
        free(img);
    img = (unsigned char * ) malloc(3 * width * height);
    memset(img, 0, sizeof(img));

    fwrite(info, sizeof(unsigned char), 54, f1);

    int length = width * height;
    unsigned long int image[10000][3];
    int i;
    for (i = 0; i < length; i++) {
        image[i][2] = getc(f); // blue
        image[i][1] = getc(f); // green
        image[i][0] = getc(f); // red

        img[licznik] = 255-(unsigned char) image[i][0]; //255-
        licznik += 1;
        img[licznik] = 255-(unsigned char) image[i][2];
        licznik += 1;
        img[licznik] =  255-(unsigned char) image[i][2];
        licznik += 1;
    pixel.red=image[i][0];
    pixel.green=image[i][2];
    pixel.blue=image[i][2];
    }

    for (i = height - 1; i >= 0; i--)
        {
            fwrite(img + (width * (height - i - 1) * 3), 3, width, f1);
            fwrite(bmppad, 1, (4 - (width * 3) % 4) % 4, f1);
            fout.write((char *) &pixel, 3);
        }

    fclose(f);
    fclose(f1);
    fout.close();


    return 250;

}
