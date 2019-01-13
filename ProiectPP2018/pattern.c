#include "pattern.h"

int myTemplateMatching(Imagine A, Imagine S, unsigned int* NrDetectii, VectorFerestre* Detectii, Pixel Culoare, double Prag)
{
    Fereastra Q;
    Q.Latime = S.Latime;
    Q.Lungime = S.Lungime;
    Q.Culoare = Culoare;
    Q.PozColtStg = 0;
    Q.LatimeImg = A.Latime;
    Q.Sters = 0;
    unsigned int ArieSablon = S.Latime * S.Lungime;
    double MedieS = 0, SigmaS = 0;
    unsigned int i, j, cont;
    for (i = 0; i < ArieSablon; ++i)
    {
        MedieS += S.VectorPixeli[i].R;
    }
    MedieS /= ArieSablon;
    for (i = 0; i < ArieSablon; ++i)
    {
        SigmaS += (S.VectorPixeli[i].R - MedieS) * (S.VectorPixeli[i].R - MedieS);
    }
    SigmaS = sqrt(SigmaS / (ArieSablon - 1));
    for (cont = 0; cont <= A.Latime * A.Lungime - A.Latime * (S.Lungime - 1) - S.Latime; ++cont)
    {
        if (cont + S.Lungime * A.Latime >= A.Latime * A.Lungime) continue;
        if (cont + S.Latime >= (cont / A.Latime + 1) * A.Latime) continue;
        double MedieFi = 0, SigmaFi = 0;
        Q.PozColtStg = cont;

        for (i = 0; i < Q.Lungime; ++i)
            for (j = 0; j < Q.Latime; ++j)
                MedieFi += A.VectorPixeli[cont + j + i * A.Latime].R;

        MedieFi /= ArieSablon;

        for (i = 0; i < Q.Lungime; ++i)
            for (j = 0; j < Q.Latime; ++j)
                SigmaFi += (A.VectorPixeli[cont + j + i * A.Latime].R - MedieFi) * (A.VectorPixeli[cont + j + i * A.Latime].R - MedieFi);

        double corr = 0;
        SigmaFi = sqrt(SigmaFi / (ArieSablon - 1));
        int it = 0;
        for (i = 0; i < Q.Lungime; ++i)
            for (j = 0; j < Q.Latime; ++j)
            {
                corr += (A.VectorPixeli[cont + j + i * A.Latime].R - MedieFi) * (S.VectorPixeli[it++].R - MedieS);
            }
        corr /= ArieSablon * SigmaFi * SigmaS;
        if (corr >= Prag)
        {
            Q.Corr = corr;
            if (*Detectii == NULL)
            {
                *Detectii =(Fereastra*) malloc(++(*NrDetectii) * sizeof(Fereastra));
                (*Detectii)[*NrDetectii - 1] = Q;
            }
            else
            {
                //printf("\n\n%d", *NrDetectii);
                VectorFerestre Aux = (Fereastra*)realloc(*Detectii, ++(*NrDetectii) * sizeof(Fereastra));
                if (Aux == NULL)
                {
                    return -1;
                }
                *Detectii = Aux;
                (*Detectii)[*NrDetectii - 1] = Q;
            }
        }
    }
    return 0;
}

unsigned int minim(unsigned int a, unsigned int b)
{
    return a < b ? a : b;
}

unsigned int maxim(unsigned int a, unsigned int b)
{
    return a > b ? a : b;
}

int Compare(const void* A, const void* B)
{
    Fereastra _A = *((Fereastra*)A);
    Fereastra _B = *((Fereastra*)B);
    if (_A.Corr > _B.Corr) return -1;
    if (_A.Corr < _B.Corr) return 1;
    return 0;
}

int Suprapunere(Fereastra A, Fereastra B)
{
    double Rezultat = 0;
    double ArieA, ArieB, ArieIntersectie;
    ArieA = A.Latime * A.Lungime;
    ArieB = B.Lungime * B.Latime;
    int XA, YA, XB, YB, XdrA, YdrA, XdrB, YdrB, XInt, YInt, XdrInt, YdrInt;
    XA = A.PozColtStg % A.LatimeImg;
    YA = A.PozColtStg / A.LatimeImg;
    XB = B.PozColtStg % B.LatimeImg;
    YB = B.PozColtStg / B.LatimeImg;
    XdrA = XA + A.Latime - 1;
    XdrB = XB + B.Latime - 1;
    YdrA = YA + A.Lungime - 1;
    YdrB = YB + B.Lungime - 1;
    XInt = maxim(XA, XB);
    YInt = maxim(YA, YB);
    XdrInt = minim(XdrA, XdrB);
    YdrInt = minim(YdrA, YdrB);
    if (XInt > XdrInt || YInt > YdrInt) return 0;
    ArieIntersectie = (XdrInt - XInt + 1) * (YdrInt - YInt + 1);
    if (ArieIntersectie < 0) return 0;
    Rezultat = ArieIntersectie / (ArieA + ArieB - ArieIntersectie);
    if (Rezultat <= 0.2) return 0;
    return 1;
}

int EliminareNonMaxime(VectorFerestre* Detectii, unsigned int* NrDetectii)
{
    unsigned int i, j, DetectiiElim = 0;
    for (i = 0; i < *NrDetectii - 1; ++i)
    {
        for (j = i + 1; j < *NrDetectii; ++j)
        {
            if (!((*Detectii)[j]).Sters && Suprapunere((*Detectii)[i],(*Detectii)[j]))
            {
                ((*Detectii)[j]).Sters = 1;
                DetectiiElim++;
            }
        }
    }
    VectorFerestre Aux = (Fereastra*)malloc(((*NrDetectii) - DetectiiElim) * sizeof(Fereastra));
    if (Aux == NULL) return -1;
    unsigned int tmp = 0;
    for (i = 0; i < *NrDetectii; ++i)
    {
        if ((*Detectii)[i].Sters == 0) Aux[tmp++] = (*Detectii)[i];
    }
    (*NrDetectii) = tmp;
    free(*Detectii);
    *Detectii = Aux;
    return 0;
}
