#include "criptare.h"

#include <stdio.h>
#include <stdlib.h>


int CriptareImg(char *ImgSursa, char* ImgDest, char* SecretKey)
{
    Imagine A = CitireImg(ImgSursa);
    if (A.ErrCode == 1)
    {
        printf("\n A aparut o eroare la citirea imaginii %s!\n", ImgSursa);
        return -1;
    }
    FILE *f = fopen(SecretKey, "r");
    unsigned int R0, SV;
    fscanf(f, "%u %u", &R0, &SV);
    unsigned int *Random = gen_sir_random(R0, A.Latime * A.Lungime);
    unsigned int *Perm = generare_permutare(A.Latime * A.Lungime, Random);
    Pixel* Temp = (Pixel*) malloc(A.Latime * A.Lungime * sizeof(Pixel));
    if (Random == NULL || Perm == NULL || Temp == NULL) printf("\nA aparut o eroare la alocarea memoriei\n");
    unsigned int i;
    for (i = 0; i < A.Latime * A.Lungime; i++)
       Temp[Perm[i]] = A.VectorPixeli[i];
    for (i = 0; i < A.Latime * A.Lungime; i++)
        A.VectorPixeli[i] = Temp[i];
    free(Temp);
    A.VectorPixeli[0] = XorPixUInt(A.VectorPixeli[0], SV);
    A.VectorPixeli[0] = XorPixUInt(A.VectorPixeli[0], Random[A.Lungime * A.Latime - 1]);
    for (i = 1; i < A.Latime * A.Lungime; ++i)
    {
        A.VectorPixeli[i] = XorPixels(A.VectorPixeli[i], A.VectorPixeli[i - 1]);
        A.VectorPixeli[i] = XorPixUInt(A.VectorPixeli[i], Random[A.Lungime * A.Latime + i - 1]);
    }
    if (StocareImagine(A, ImgDest) == -1)
    {
        printf("\nA aparut o eroare la stocarea imaginii %s in memoria externa!\n", ImgDest);
        return -1;
    }
    free(Random);
    free(Perm);
    EliberareMemorie(A);
    fclose(f);
    return 0;
}

int DecriptareImg(char *ImgSursa, char* ImgDest, char* SecretKey)
{
    Imagine A = CitireImg(ImgSursa);
    if (A.ErrCode == 1)
    {
        printf("\n A aparut o eroare la citirea imaginii %s!\n", ImgSursa);
        EliberareMemorie(A);
        return -1;
    }
    FILE *f = fopen(SecretKey, "r");
    unsigned int R0, SV;
    fscanf(f, "%u %u", &R0, &SV);
    unsigned int *Random = gen_sir_random(R0, A.Latime * A.Lungime);
    unsigned int *Perm = generare_permutare(A.Latime * A.Lungime, Random);
    unsigned int *Inv = gen_perm_inv(A.Latime * A.Lungime, Perm);
    Pixel* Temp = (Pixel*) malloc(A.Latime * A.Lungime * sizeof(Pixel));
    if (Random == NULL || Perm == NULL || Inv == NULL || Temp == NULL) printf("\nA aparut o eroare la alocarea memoriei\n");
    unsigned int i;
    for (i = A.Latime * A.Lungime - 1; i > 0; --i) {
        A.VectorPixeli[i] = XorPixels(A.VectorPixeli[i], A.VectorPixeli[i - 1]);
        A.VectorPixeli[i] = XorPixUInt(A.VectorPixeli[i], Random[A.Lungime * A.Latime + i - 1]);
    }
    A.VectorPixeli[0] = XorPixUInt(A.VectorPixeli[0], SV);
    A.VectorPixeli[0] = XorPixUInt(A.VectorPixeli[0], Random[A.Lungime * A.Latime - 1]);

    for (i = 0; i < A.Latime * A.Lungime; i++)
        Temp[Inv[i]] = A.VectorPixeli[i];
    for (i = 0; i < A.Latime * A.Lungime; i++)
        A.VectorPixeli[i] = Temp[i];
    free(Temp);
    if (StocareImagine(A, ImgDest) == -1)
    {
        printf("\nA aparut o eroare la stocarea imaginii %s in memoria externa!\n", ImgDest);
        return -1;
    }
    free(Random);
    free(Perm);
    free (Inv);
    EliberareMemorie(A);
    fclose(f);
    return 0;
}

Pixel XorPixels(Pixel A, Pixel B)
{
    Pixel C;
    C.B = B.B ^ A.B;
    C.G = B.G ^ A.G;
    C.R = B.R ^ A.R;
    return C;
}

Pixel XorPixUInt(Pixel A, unsigned int b)
{
    Pixel C;
    C.B = A.B ^ (unsigned char)b;
    b >>= 8;
    C.G = A.G ^ (unsigned char)b;
    b >>= 8;
    C.R = A.R ^ (unsigned char)b;
    return C;
}
