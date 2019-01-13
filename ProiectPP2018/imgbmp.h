#ifndef IMGBMP_H_
#define IMGBMP_H_

/// imgbmp.h - header pentru imagine.
typedef struct
{
    unsigned char B;
    unsigned char G;
    unsigned char R;
} Pixel;

typedef struct
{
    int ErrCode;
    unsigned int Dimensiune;
    unsigned int Lungime;
    unsigned int Latime;
    unsigned char* Header;
    Pixel* VectorPixeli;
    short int Padding;
} Imagine;

typedef struct
{
    double Corr;
    unsigned int LatimeImg;
    unsigned int Lungime;
    unsigned int Latime;
    unsigned int PozColtStg;
    short int Sters;
    Pixel Culoare;
} Fereastra;

typedef Fereastra* VectorFerestre;

Imagine CitireImg(char*);
int StocareImagine(Imagine, char*);
int ChiPatrat(char*);
void grayscale_image(char*, char*);
void DesenContur(Imagine*, Fereastra, Pixel);
void EliberareMemorie(Imagine);

#endif // IMGBMP_H_
