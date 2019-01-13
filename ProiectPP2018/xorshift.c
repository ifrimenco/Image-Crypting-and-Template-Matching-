#include "xorshift.h"
#include <stdlib.h>

unsigned int rng(unsigned int seed)
{
    unsigned int x = seed;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return x;
}

unsigned int* gen_sir_random(unsigned int R0, unsigned int Dimensiune)
{
    unsigned int* v = (unsigned int*)malloc(2 * Dimensiune * sizeof(unsigned int));
    v[0] = rng(R0);
    int i;
    for (i = 1; i < 2 * Dimensiune; ++i)
    {
        v[i] = rng(v[i - 1]);
    }
    return v;
}

unsigned int* generare_permutare(unsigned int Dimensiune, unsigned int* Sir)
{
    unsigned int i;
    unsigned int* Sigma;
    Sigma = (unsigned int*)malloc(Dimensiune * sizeof(unsigned int));
    for (i = 0; i < Dimensiune; ++i)
    {
        Sigma[i] = i;
    }
    int q = 0;
    for (i = Dimensiune - 1; i > 0; i--)
    {
        unsigned int j = Sir[q++] % (i + 1);
        unsigned int aux;
        aux = Sigma[i];
        Sigma[i] = Sigma[j];
        Sigma[j] = aux;
    }
    return Sigma;
}

unsigned int* gen_perm_inv(unsigned int Dimensiune, unsigned int *Perm)
{
    unsigned int* Inv = (unsigned int*) malloc(Dimensiune * sizeof(unsigned int));
    int i;
    for (i = 0; i < Dimensiune; ++i)
    {
        Inv[Perm[i]] = i;
    }
    return Inv;
}
