#ifndef PATTERN_H_
#define PATTERN_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "imgbmp.h"

//typedef Fereastra* VectorFerestre

unsigned int minim(unsigned int, unsigned int);
unsigned int maxim(unsigned int, unsigned int);
int myTemplateMatching(Imagine, Imagine, unsigned int*, VectorFerestre*, Pixel, double);
int Compare(const void*, const void*);
int Suprapunere(Fereastra, Fereastra);
int EliminareNonMaxime(VectorFerestre*, unsigned int*);

#endif // PATTERN_H_
