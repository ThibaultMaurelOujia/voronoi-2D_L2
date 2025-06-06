#include<stdio.h>
#include<stdlib.h>
#include "Struct.h"

#include "outils.c"
#include "donnees.c"

#define npts 4



int main(){
    srand(time(NULL));
    printf("lil");
//    generer_points("Liste_Points.txt", npts);
    Point * p = lecture("Liste_Points.txt",npts);
    int ** voisin = malloc(npts*sizeof(int*));
    int i;
    for(i=0;i<npts;i++){
        voisin[i]=malloc(npts*sizeof(int));
    }
    int * longeur=malloc(npts*sizeof(int));

    Delaunay(p, voisin, npts, longeur);
    int j;
    for(i=0;i<npts;i++){
        printf("\n");
        for(j=0;j<npts;j++){
            printf("%3d",voisin[i][j]);
        }
    }
}




