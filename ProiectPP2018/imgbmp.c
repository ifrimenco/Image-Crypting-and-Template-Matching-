#include "imgbmp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Imagine CitireImg(char *CaleImg)
{
    Imagine A;
    FILE* f = fopen(CaleImg, "rb");
    if (f == NULL) {
        A.ErrCode = 1;
        return A;
    }
    A.ErrCode = 0;

    A.Header = (unsigned char*)malloc(54 * sizeof(unsigned char));
    if (fread(A.Header, 54, 1, f) != 1)
    {
        A.ErrCode = 1;
        return A;
    }
    A.Dimensiune = *((unsigned int*) (A.Header + 2)); // dimensiunea imaginii in octeti se afla pe octetul 2 (4 octeti)
    A.Latime = *((unsigned int*) (A.Header + 18)); // octetul 18 - latimea (4 octeti)
    A.Lungime = *((unsigned int*) (A.Header + 22)); // octetul 22 - lungimea (4 octeti)
    printf("\nIMAGINE DESCHISA: %s", CaleImg);
    printf("\nDimensiune: %u\n", A.Dimensiune);
    printf("Latime imagine: %u\nLungime imagine: %u\n", A.Latime, A.Lungime);
    A.VectorPixeli = (Pixel*) malloc(A.Latime * A.Lungime * sizeof(Pixel));
    A.Padding = (4 - A.Latime * 3 % 4) % 4;
    printf("Padding: %d\n", A.Padding);
    unsigned int i;
    for (i = 1; i <= A.Lungime; i++)
    {
        fseek(f, - i * A.Latime * sizeof(Pixel) - i * A.Padding, SEEK_END);
        if (fread(A.VectorPixeli + (i - 1) * A.Latime, A.Latime * sizeof(Pixel), 1, f) != 1) {
            A.ErrCode = 1;
            return A;
        }
    }
    fclose(f);
    return A;
}

int StocareImagine(Imagine A, char* CaleImg) {
    FILE* f = fopen(CaleImg, "wb");
    if (f == NULL) return -1;
    unsigned char pudding = 0;
    if (fwrite(A.Header, 54, 1, f) != 1) return -1;
    //fflush(f);
    int i;
    for (i = (A.Lungime - 1) * A.Latime; i >= 0; i -= A.Latime)
    {
        //printf("%d\n", i);
        if (fwrite(A.VectorPixeli + i, A.Latime * sizeof(Pixel), 1, f) != 1)
            return -1;
        if (A.Padding)
            if (fwrite(&pudding, 1, A.Padding, f) != A.Padding)
                return -1;
       // fflush(f);
    }
    fclose(f);
    return 0;
}

int ChiPatrat(char* CaleImg)
{
    Imagine A = CitireImg(CaleImg);
    FILE* f = fopen("chi-squared-values.txt", "a");
    if (A.ErrCode == 1)
    {
        printf("\nA aparut o eroare la citirea imaginii!\n");
        return -1;
    }
    double MedR = 0, MedG = 0, MedB = 0;
    unsigned int *R, *G, *B;
    R = (unsigned int*) calloc(256, sizeof(unsigned int));
    G = (unsigned int*) calloc(256, sizeof(unsigned int));
    B = (unsigned int*) calloc(256, sizeof(unsigned int));
    unsigned int i;
    for (i = 0; i < A.Latime * A.Lungime; i++)
    {
        Pixel Q = A.VectorPixeli[i];
        //printf("%d ", B[Q.B]);
        B[Q.B]++;
        G[Q.G]++;
        R[Q.R]++;
    }

    double Medie = A.Latime * A.Lungime / 256.0;
    for (i = 0; i < 256; ++i)
    {
       // fprintf(f, "R: %.2f\nG:  %.2f\nB:  %.2f\n", MedR, MedG, MedB);
        MedB += (B[i] - Medie) * (B[i] - Medie) / Medie;
        MedG += (G[i] - Medie) * (G[i] - Medie) / Medie;
        MedR += (R[i] - Medie) * (R[i] - Medie) / Medie;
    }
    fprintf(f, "Imagine: %s\nR: %.2f\nG:  %.2f\nB:  %.2f\n\n", CaleImg, MedR, MedG, MedB);
    printf("\nR: %.2f\nG:  %.2f\nB:  %.2f\n\n", MedR, MedG, MedB);
    fclose(f);
    free(R);
    free(G);
    free(B);
    EliberareMemorie(A);
    return 0;
}

void grayscale_image(char* nume_fisier_sursa,char* nume_fisier_destinatie)
{

    FILE *fin, *fout;
    unsigned int dim_img, latime_img, inaltime_img;
    unsigned char pRGB[3], header[54], aux;

    printf("nume_fisier_sursa = %s \n",nume_fisier_sursa);

    fin = fopen(nume_fisier_sursa, "rb");
    if(fin == NULL)
   	{
        printf("nu am gasit imaginea sursa din care citesc");
   		return;
   	}

    fout = fopen(nume_fisier_destinatie, "wb+");

    fseek(fin, 2, SEEK_SET);
    fread(&dim_img, sizeof(unsigned int), 1, fin);
    printf("Dimensiunea imaginii in octeti: %u\n", dim_img);

    fseek(fin, 18, SEEK_SET);
    fread(&latime_img, sizeof(unsigned int), 1, fin);
    fread(&inaltime_img, sizeof(unsigned int), 1, fin);
    printf("Dimensiunea imaginii in pixeli (latime x inaltime): %u x %u\n",latime_img, inaltime_img);

    //copiaza octet cu octet imaginea initiala in cea noua
	fseek(fin,0,SEEK_SET);
	unsigned char c;
	while(fread(&c,1,1,fin)==1)
	{
		fwrite(&c,1,1,fout);
		fflush(fout);
	}
	fclose(fin);

	//calculam padding-ul pentru o linie
	int padding;
    if(latime_img % 4 != 0)
        padding = 4 - (3 * latime_img) % 4;
    else
        padding = 0;

    printf("padding = %d \n",padding);

	fseek(fout, 54, SEEK_SET);
	int i,j;
	for(i = 0; i < inaltime_img; i++)
	{
		for(j = 0; j < latime_img; j++)
		{
			//citesc culorile pixelului
			fread(pRGB, 3, 1, fout);
			//fac conversia in pixel gri
			aux = 0.299*pRGB[2] + 0.587*pRGB[1] + 0.114*pRGB[0];
			pRGB[0] = pRGB[1] = pRGB[2] = aux;
        	fseek(fout, -3, SEEK_CUR);
        	fwrite(pRGB, 3, 1, fout);
        	fflush(fout);
		}
		fseek(fout,padding,SEEK_CUR);
	}
	fclose(fout);
}

void DesenContur(Imagine *A, Fereastra B, Pixel q)
{
    unsigned int poz = B.PozColtStg, i;
    if (poz + B.Lungime * A->Latime >= A->Latime * A->Lungime) return;
    if (poz + B.Latime >= (poz / A->Latime + 1) * A->Latime) return;
    for (i = 0; i < B.Latime; ++i)
    {
        A->VectorPixeli[poz + i] = q;
        A->VectorPixeli[poz + i + (A->Latime) * (B.Lungime - 1)] = q;
    }

    for (i = 0; i < B.Lungime; ++i)
    {
        A->VectorPixeli[poz + A->Latime * i] = q;
        A->VectorPixeli[poz + A->Latime * i + B.Latime - 1] = q;
    }
}

void EliberareMemorie(Imagine A)
{
    if (A.VectorPixeli != NULL)
    free(A.VectorPixeli);
    if (A.Header != NULL)
    free(A.Header);
}


