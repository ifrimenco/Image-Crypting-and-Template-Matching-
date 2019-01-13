#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xorshift.h"
#include "imgbmp.h"
#include "criptare.h"
#include "pattern.h"

/// Proiect PP 2018 - 2019
/// (c) Ifrimenco Alexandru - Daniel


int main()
{
    printf("------Proiect PP Ifrimenco Alexandru - Daniel ------\n\n");
    printf("\n\n\nI. CRIPTOGRAFIE\n\n");
    printf("\nIntroduceti numele imaginii .bmp care urmeaza a fi citita\n");
    char NumeImg[101];
    fgets(NumeImg, 101, stdin);
    NumeImg[strlen(NumeImg) - 1] = '\0';
    printf("\nIntroduceti numele fisierului in care doriti sa salvati imaginea criptata, urmat de extensia .bmp\n");
    char ImgDest[101];
    fgets(ImgDest, 101, stdin);
    ImgDest[strlen(ImgDest) - 1] = '\0';
    printf("\nIntroduceti numele fisierului care contine cele doua chei secrete\n");
    char SecretKey[101];
    fgets(SecretKey, 101, stdin);
    SecretKey[strlen(SecretKey) - 1] = '\0';
    if (CriptareImg(NumeImg, ImgDest, SecretKey) == -1)
        return 0;
    printf("\nIntroduceti numele imaginii pe care doriti sa o decriptati\n");
    char ImgEnc[101];
    fgets(ImgEnc, 101, stdin);
    ImgEnc[strlen(ImgEnc) - 1] = '\0';
    printf("\nIntroduceti numele imaginii destinatie\n");
    fgets(ImgDest, 101, stdin);
    ImgDest[strlen(ImgDest) - 1] = '\0';
    printf("\nIntroduceti numele fisierului care contine cele doua chei secrete\n");
    fgets(SecretKey, 101, stdin);
    SecretKey[strlen(SecretKey) - 1] = '\0';
    if (DecriptareImg(ImgEnc, ImgDest, SecretKey) == -1)
        return 0;
    printf("\n\n\nCHI -- PATRAT\n");
    if (ChiPatrat(NumeImg) == -1) return 0;
    if (ChiPatrat(ImgEnc) == -1) return 0;
    printf("\n\n\nII. PATTERN MATCHING\n\n");
    char NumeSablon[] = "cifra .bmp";
    unsigned int i;
    Pixel Culori[10];
    Culori[0] = (Pixel){0, 0, 255};
    Culori[1] = (Pixel){0, 255, 255};
    Culori[2] = (Pixel){0, 255, 0};
    Culori[3] = (Pixel){255, 255, 0};
    Culori[4] = (Pixel){255, 0, 255};
    Culori[5] = (Pixel){255, 0, 0};
    Culori[6] = (Pixel){192, 192, 192};
    Culori[7] = (Pixel){0, 140, 255};
    Culori[8] = (Pixel){128, 0, 128};
    Culori[9] = (Pixel){0, 0, 128};
    printf("\nIntroduceti numele imaginii pe care doriti sa faceti pattern matching\n");
    fgets(NumeImg, 101, stdin);
    NumeImg[strlen(NumeImg) - 1] = '\0';
    Imagine A = CitireImg(NumeImg);
    if (A.ErrCode)
    {
        printf("A aparut o eroare la citirea imaginii %s!\n", NumeImg);
        EliberareMemorie(A);
        return 0;
    }
    printf("\nIntroduceti pragul de detectie\n");
    double prag;
    scanf("%lf", &prag);
    VectorFerestre Detectii = NULL;
    unsigned int NrDetectii = 0;
    char G[50];
    strncpy(G, NumeImg, strlen(NumeImg) - 4);
    strcat(G, "_grayscale.bmp");
    grayscale_image(NumeImg, G);
    Imagine A_GrayScale = CitireImg(G);
    for (i = 0; i < 10; i++)
    {
        NumeSablon[5] = i + '0';
        char StrSab[50] = {'0'};
        strncpy(StrSab, NumeSablon, strlen(NumeSablon) - 4);
        strcat(StrSab, "_grayscale.bmp");
        //StrSab[strlen(StrSab)] = '\0';
        grayscale_image(NumeSablon, StrSab);
        Imagine Sablon_Grayscale = CitireImg(StrSab);

        if (myTemplateMatching(A_GrayScale, Sablon_Grayscale, &NrDetectii, &Detectii, Culori[i], prag) == -1)
        {
            printf("A aparut o eroare la alocarea memoriei!!\n");
            EliberareMemorie(A);
            EliberareMemorie(A_GrayScale);
            EliberareMemorie(Sablon_Grayscale);
            free(Detectii);
            return 0;
        }
       // printf("\n\n%d", NrDetectii);
       // printf("AAAAAAAAAAAA");
        EliberareMemorie(Sablon_Grayscale);
    }
    qsort(Detectii, NrDetectii, sizeof(Fereastra), Compare);
    if(EliminareNonMaxime(&Detectii, &NrDetectii)== -1)
    {
        EliberareMemorie(A);
        free(Detectii);
        printf("\nA aparut o problema la alocarea memoriei!\n");
    }
    for (i = 0; i < NrDetectii; ++i)
        DesenContur(&A, Detectii[i], Detectii[i].Culoare);
    if(StocareImagine(A, "patternmatching_aplicat.bmp") == -1)
        printf("\nA aparut o eroare la stocarea imaginii patternmatching_aplicat.bmp\n");
    EliberareMemorie(A);
    free(Detectii);
    return 0;
}
