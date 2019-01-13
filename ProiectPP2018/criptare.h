#ifndef CRIPTARE_H_
#define CRIPTARE_H_

#include "xorshift.h"
#include "imgbmp.h"

int CriptareImg(char*, char*, char*);
int DecriptareImg(char*, char*, char*);
Pixel XorPixels(Pixel, Pixel);
Pixel XorPixUInt(Pixel, unsigned int);

#endif // CRIPTARE_H_
