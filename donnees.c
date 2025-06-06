#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include "outils.h"


/** genere des points et les ecrits dans un fichier **/
void generer_points(char * nomFichier, int npts){
	int i;
	FILE* fichier = NULL;
	fichier = fopen(nomFichier, "w");
	if (fichier != NULL){
		for(i=0;i<npts;i++){
			fprintf(fichier,"%d %d\n",rand()%HPLAN ,rand()%LPLAN);
		}
	}
	fclose(fichier);
}


/** retourne un tableau de npts Point(s) à partir du fichier nomFichier **/
Point * lecture(char * nomFichier, int npts){
  int i;
  Point *p=malloc(sizeof(Point)*npts);//On reserve la memoire pour npts points


  FILE* fichier = fopen(nomFichier, "r");
  if (fichier == NULL){
  	fprintf(stderr, "fichier impossible a ouvrir\n");
  	exit(1);
  }
  for(i=0;i<npts;i++) {
	  if(fscanf(fichier,"%d %d", &(p[i].x), &(p[i].y)) == EOF) {	//	&(p[i].x) adresse de p[i].x
	  	fprintf(stderr, "fin de fichier atteinte avant lecture des %d points\n", i);
  		exit(1);
	  }
	}
  fclose(fichier);

  return p;
}


void ecrit_seg(char * nomFichier, int npts, Seg ** S, int ** germe){
	int i,j;
	FILE* fichier = NULL;
	fichier = fopen(nomFichier, "w");
	if (fichier != NULL){
        for(i=0;i<npts;i++){
            for(j=0;j<npts;j++){
                if(germe[i][j]==1){
                    fprintf(fichier,"%d %d %d %d\n",S[i][j].x1, S[i][j].y1, S[i][j].x2, S[i][j].y2);
                }
            }
        }
	}
	fclose(fichier);
}


/** equation des droites des bords **/
Droite * cree_bord(){

    Droite * B = malloc(4*sizeof(Droite));

    B[0].a = 1;
    B[0].b = 0;
    B[0].c = 0;
    B[1].a = 0;
    B[1].b = 1;
    B[1].c = 0;
    B[2].a = 1;
    B[2].b = 0;
    B[2].c = -LPLAN;
    B[3].a = 0;
    B[3].b = 1;
    B[3].c = -HPLAN;

  //  for(int i=0;i<4;i++)
    //    printf("\n %dx+%d+%d=0", B[i].a, B[i].b, B[i].c);

    return B;
}
