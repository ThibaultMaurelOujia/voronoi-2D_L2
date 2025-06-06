#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include <math.h>
#include "outils.h"




/** cherche le plus proche voisin du point "a" **/
int plus_proche_voisin(Point * p, int a, int npts){
    int i, voisin=0;
    int min = LPLAN*LPLAN+HPLAN*HPLAN;
    for(i = 0; i < npts; i++){
        if ((p[a].x-p[i].x)*(p[a].x-p[i].x)+(p[a].y-p[i].y)*(p[a].y-p[i].y) < min && a!=i){
            min = (p[a].x-p[i].x)*(p[a].x-p[i].x)+(p[a].y-p[i].y)*(p[a].y-p[i].y);
            voisin = i;
        }
    }
    return voisin;
}

int plus_proche_voisin_priver(Point * p, int a, int npts, int priv1, int priv2){
    int i, voisin=0;
    int min = LPLAN*LPLAN+HPLAN*HPLAN;
    for(i = 0; i < npts; i++){
        if ((p[a].x-p[i].x)*(p[a].x-p[i].x)+(p[a].y-p[i].y)*(p[a].y-p[i].y) < min && a!=i && priv1!=i && priv2!=i){
            min = (p[a].x-p[i].x)*(p[a].x-p[i].x)+(p[a].y-p[i].y)*(p[a].y-p[i].y);
            voisin = i;
        }
    }
    return voisin;
}

int plus_proche_voisin_point_priver(Point * p, Point * inter, int q, int npts, int priv1, int priv2, int priv3, int priv4){
    int i, voisin=0;
    int min = LPLAN*LPLAN+HPLAN*HPLAN;
    for(i = 0; i < q+1; i++){
        if (i==npts+1)
            return 0;
        if ((inter[0].x-p[i].x)*(inter[0].x-p[i].x)+(inter[0].y-p[i].y)*(inter[0].y-p[i].y) < min && priv1!=i && priv2!=i && priv3!=i && priv4!=i){
            min = (inter[0].x-p[i].x)*(inter[0].x-p[i].x)+(inter[0].y-p[i].y)*(inter[0].y-p[i].y);
            voisin = i;
        }
    }
    return voisin;
}

/** cherche le plus proche voisin du point "inter"**/
int plus_proche_voisin_point(Point * p, Point * inter, int q, int npts){
    int i, voisin=0;
    int min = LPLAN*LPLAN+HPLAN*HPLAN;
    for(i = 0; i < q+1; i++){
        if (i==npts+1)
            return 0;
        if ((inter[0].x-p[i].x)*(inter[0].x-p[i].x)+(inter[0].y-p[i].y)*(inter[0].y-p[i].y) < min ){
            min = (inter[0].x-p[i].x)*(inter[0].x-p[i].x)+(inter[0].y-p[i].y)*(inter[0].y-p[i].y);
            voisin = i;
        }
    }
    return voisin;
}

/** initialise les germes voisin a une cellule **/
int ** initGerme(int npts){
    int i, j;
    int ** germe = malloc(npts*sizeof(int*));

    for(i=0;i<npts;i++)
        germe[i]=malloc(npts*sizeof(int));

    for(i=0;i<npts;i++){
        for(j=0;j<npts;j++){
            germe[i][j]=-1;
        }
    }
    /** -1 si le germe ne fait pas partie de la construction de la cellule de voronoi, 1 sinon **/
    for(i=0;i<3;i++){
        for(j=0;j<3;j++){
            germe[i][j]=1;
        }
        germe[i][i]=-1;
    }

    return germe;
}

/** calcule pour le moment les equations de toutes les mediatrices entre tous les points. La fonction sera changée plus tard pour ne caluler que les mediatrices interessantes pour ne pas faire du calcul inutile **/
Droite ** equation_mediatrice(Point * p, int npts){
    int a, b, i;
    Droite ** D = malloc(npts*sizeof(Droite *));
    for(i=0;i<npts;i++)
        D[i]=malloc(npts*sizeof(Droite));

    for(a=0;a<npts;a++){
        for(b=a;b<npts;b++){
            D[a][b].a=0;
            D[a][b].b=1;
            D[a][b].c=1;
            if(a!=b){
                D[a][b].a=-2*p[a].x+2*p[b].x;
                D[a][b].b=-2*p[a].y+2*p[b].y;
                D[a][b].c=p[a].x*p[a].x+p[a].y*p[a].y-p[b].x*p[b].x-p[b].y*p[b].y;
                D[b][a].a=D[a][b].a;
                D[b][a].b=D[a][b].b;
                D[b][a].c=D[a][b].c;
            }
        }
    }

    return D;
}


/** calcule l'intersection entre deux mediatrices **/
Point * intersection_med (Droite ** D, int x1, int y1, int x2, int y2){
    Point * inter = malloc(sizeof(Point));
    /*inter[0].y = LPLAN*LPLAN+HPLAN*HPLAN;
    inter[0].x = LPLAN*LPLAN+HPLAN*HPLAN;*/
    if ((D[x2][y2].b*D[x1][y1].a-D[x1][y1].b*D[x2][y2].a)!=0){
        inter[0].y = (D[x1][y1].c*D[x2][y2].a-D[x2][y2].c*D[x1][y1].a)/(D[x2][y2].b*D[x1][y1].a-D[x1][y1].b*D[x2][y2].a);
        inter[0].x = (D[x1][y1].b*D[x2][y2].c-D[x2][y2].b*D[x1][y1].c)/(D[x2][y2].b*D[x1][y1].a-D[x1][y1].b*D[x2][y2].a);
    }

   // printf("inter[0].x = %d  inter[0].y = %d\n", inter[0].x, inter[0].y);
    return inter;
}

/** calcule l'intersection entre deux droites **/
Point * intersection_droit (Droite ** D, Droite * B, int x, int y, int z){
    Point * inter = malloc(sizeof(Point));
 /*   inter[0].y = LPLAN*LPLAN+HPLAN*HPLAN;
    inter[0].x = LPLAN*LPLAN+HPLAN*HPLAN;*/
    if ((B[z].b*D[x][y].a-D[x][y].b*B[z].a)!=0){
        inter[0].y = (D[x][y].c*B[z].a-B[z].c*D[x][y].a)/(B[z].b*D[x][y].a-D[x][y].b*B[z].a);
        inter[0].x = (D[x][y].b*B[z].c-B[z].b*D[x][y].c)/(B[z].b*D[x][y].a-D[x][y].b*B[z].a);
    }
   // printf("\n inter ###  x %d y %d", inter->x, inter->y);

   // printf("inter[0].x = %d  inter[0].y = %d\n", inter[0].x, inter[0].y);
    return inter;
}
/** intersection_med et intersection_droit seront fusionnées plus tard **/

/** cherche le centre du cercle circonscrit, pour le moment la fonction n'est plus utilisée **/
Point * centre_cercle_circonscrit( int a, int b, int c, Point * p){
    Point * c_cir = malloc(sizeof(Point));
    c_cir[0].x=(((p[b].x-p[a].x)*(p[b].x+p[a].x)/c+(p[b].y-p[a].y)*(p[b].y+p[a].y)/c)*(p[c].y-p[a].y)-((p[c].x-p[a].x)*(p[c].x+p[a].x)/c+(p[c].y-p[a].y)*(p[c].y+p[a].y)/c)*(p[b].y-p[a].y))/((p[b].x-p[a].x)*(p[c].y-p[a].y)-(p[c].x-p[a].x)*(p[b].y-p[a].y));
    c_cir[0].y=(((p[c].x-p[a].x)*(p[c].x+p[a].x)/c+(p[c].y-p[a].y)*(p[c].y+p[a].y)/c)*(p[b].x-p[a].x)-((p[b].x-p[a].x)*(p[b].x+p[a].x)/c+(p[b].y-p[a].y)*(p[b].y+p[a].y)/c)*(p[c].x-p[a].x))/((p[b].x-p[a].x)*(p[c].y-p[a].y)-(p[c].x-p[a].x)*(p[b].y-p[a].y));

    return c_cir;
}

Point * milieuPoint( int a, int b, Point * p ){
    Point * milieu = malloc(sizeof(Point));
    milieu[0].x=(p[a].x+p[b].x)/2;
    milieu[0].y=(p[a].y+p[b].y)/2;
   // printf("\n &&%d , %d, %d , %d &&", a, b, milieu[0].x, milieu[0].y);
    return milieu;
}

void init_segment(Seg ** S, Point * p, Droite * B, Droite ** D){
    int i, j, k;
    int min ;
    Point * c_cir = centre_cercle_circonscrit( 0, 1, 2, p);


    for(i=0;i<3;i++){
        for(j=i+1;j<3;j++){
            if(i!=j){
                S[i][j].x1=c_cir[0].x;
                S[i][j].y1=c_cir[0].y;
             //   printf("%d %d ", S[i][j].x1, S[i][j].y1);
            }
        }
    }
    for(i=0;i<3;i++){
        for(j=i+1;j<3;j++){
            min = HPLAN*HPLAN+LPLAN*LPLAN;
            if(i!=j){
                for(k=0;k<4;k++){
                    Point * inter = intersection_droit (D, B, i, j, k);
          //          printf("\n inter ---  x %d y %d", inter->x, inter->y);
                    Point * milieu = milieuPoint( i, j, p );
                    if( ( ( milieu[0].x-inter[0].x )*( milieu[0].x-p[((i+j)*2)%3].x ) )+( ( milieu[0].y-inter[0].y )*( milieu[0].y-p[((i+j)*2)%3].y ) ) < 0 && (( milieu[0].x-inter[0].x )*( milieu[0].x-inter[0].x )+( milieu[0].y-inter[0].y )*( milieu[0].y-inter[0].y ))<min ){
                        min = ( milieu[0].x-inter[0].x )*( milieu[0].x-inter[0].x )+( milieu[0].y-inter[0].y )*( milieu[0].y-inter[0].y );
                        S[i][j].x2=inter[0].x;
                        S[i][j].y2=inter[0].y;
                    }
                }
            }
    //    printf("\nx %d y %d", S[i][j].x2, S[i][j].y2);
        }
    }
    for(i=0;i<3;i++){
        for(j=i+1;j<3;j++){
            S[j][i].x1=S[i][j].x1;
            S[j][i].y1=S[i][j].y1;
            S[j][i].x2=S[i][j].x2;
            S[j][i].y2=S[i][j].y2;
        }
    }
    for(i=0;i<3;i++){
        S[i][i].x1=-1;
        S[i][i].y1=-1;
        S[i][i].x2=-1;
        S[i][i].y2=-1;
    }


}

/** construit un nouveau germe a partir de l'ancien diagramme de voronoi, mais ne modifie pas encore les aretes des autres cellules **/

void intersection_germe(Point * p, Droite ** D, int q, int npts, int ** germe, Droite * B, Seg ** S){
    int fin=0;
    int k, i, bord=0, bordprem=0;
    int suiv=npts+1;
    int min;
    int Pp2;
    int Pp;
    int bouclebord=0;
    if (q!=npts){
        Pp = plus_proche_voisin( p, q, q+1);
    }
    else {
        Pp = plus_proche_voisin( p, q, npts);
    }
  //  printf("\n voisin : %d \n",Pp);
    int pred=Pp;
    germe[Pp][q]=1;
    germe[q][Pp]=1;
    int prem = Pp;
    int prex, prey;
    Point * test = malloc(sizeof(Point));
    test[0].x=0;
    test[0].y=0;
    while(suiv!=prem){
        k=0;
        for(i=0;i<npts;i++){
            if (germe[Pp][i]!=-1){
                Point * inter = intersection_med ( D, Pp, q, Pp, i);
             //   printf("\n %d %d %d %d %d %d \n",inter[0].x,inter[0].y, germe[Pp][i] ,i, Pp, q);
                if ((S[Pp][i].x1-inter[0].x)*(S[Pp][i].x1-inter[0].x)+(S[Pp][i].y1-inter[0].y)*(S[Pp][i].y1-inter[0].y) <= (S[Pp][i].x1-S[Pp][i].x2)*(S[Pp][i].x1-S[Pp][i].x2)+(S[Pp][i].y1-S[Pp][i].y2)*(S[Pp][i].y1-S[Pp][i].y2) && (S[Pp][i].x1-inter[0].x)*(S[Pp][i].x1-S[Pp][i].x2)+(S[Pp][i].y1-inter[0].y)*(S[Pp][i].y1-S[Pp][i].y2) > 0 && -1<inter[0].x && inter[0].x<HPLAN+1 && -1<inter[0].y && inter[0].y<LPLAN+1 && ( inter[0].x!=0 || inter[0].y!=0)){
                    if(k==0){
                        S[Pp][q].x1=inter[0].x;
                        S[Pp][q].y1=inter[0].y;
                        S[Pp][q].suiv1=i;
                        S[q][Pp].x1=inter[0].x;
                        S[q][Pp].y1=inter[0].y;
                        S[q][Pp].suiv1=i;
                        k++;
                        germe[q][i]=1;
                        germe[i][q]=1;
                    }
                    if(k==1){
                        S[Pp][q].x2=inter[0].x;
                        S[Pp][q].y2=inter[0].y;
                        S[Pp][q].suiv2=i;
                        S[q][Pp].x2=inter[0].x;
                        S[q][Pp].y2=inter[0].y;
                        S[q][Pp].suiv2=i;
                        germe[q][i]=1;
                        germe[i][q]=1;
                    }
                }
               // if(Pp!=prem && (S[Pp][q].x1==S[Pp][q].x2+1 || S[Pp][q].x1==S[Pp][q].x2-1) && (S[Pp][q].y1==S[Pp][q].y2+1 || S[Pp][q].y1==S[Pp][q].y2-1))
             //       gestion_sommet(p, D, q, npts, germe, S);
              //  printf("\n--- %d : %d %d %d %d %d\n", q, S[Pp][q].x1,S[Pp][q].y2,S[Pp][q].x2,S[Pp][q].y2, Pp);
            }
        }
        if (S[Pp][q].x1==S[Pp][q].x2 && S[Pp][q].y1==S[Pp][q].y2){
            bord++;
            min = HPLAN*HPLAN+LPLAN*LPLAN;
            Point * milieu = milieuPoint( Pp, q, p );
            for(k=0;k<4;k++){
                Point * interbord = intersection_droit (D, B, Pp, q, k);
                int voisin = plus_proche_voisin_point(p, interbord,q, npts);
                if ((S[Pp][q].x1-milieu[0].x)*(interbord[0].x-milieu[0].x)+(S[Pp][q].y1-milieu[0].y)*(interbord[0].y-milieu[0].y) < 0 && ((milieu[0].x-interbord[0].x)*(milieu[0].x-interbord[0].x)+(milieu[0].y-interbord[0].y)*(milieu[0].y-interbord[0].y)) < min && Pp==prem && -1<interbord[0].x && interbord[0].x<HPLAN+1 && -1<interbord[0].y && interbord[0].y<LPLAN+1 && ( interbord[0].x!=0 || interbord[0].y!=0)){
                    min = ((milieu[0].x-interbord[0].x)*(milieu[0].x-interbord[0].x)+(milieu[0].y-interbord[0].y)*(milieu[0].y-interbord[0].y));
                    S[Pp][q].x2=interbord[0].x;
                    S[Pp][q].y2=interbord[0].y;
                    S[q][Pp].x2=interbord[0].x;
                    S[q][Pp].y2=interbord[0].y;// printf("\nTEst 1");
                }
                else if (Pp!=prem){
                    if(S[pred][q].x1!=S[Pp][q].x1 && S[pred][q].y1!=S[Pp][q].y1){
                        prex=S[pred][q].x1;
                        prey=S[pred][q].y1;//printf("\nTEst 2");
                    }
                    else {
                        prex=S[pred][q].x2;
                        prey=S[pred][q].y2;//printf("\nTEst 3 ------%d %d voisin%d ", interbord[0].x, interbord[0].y, voisin);
                    }
                //    printf("\n %d < %d ---  %d*%d ---   %d %d \n",((milieu[0].x-interbord[0].x)*(milieu[0].x-interbord[0].x)+(milieu[0].y-interbord[0].y)*(milieu[0].y-interbord[0].y)), min,((D[suiv][pred].a*prex+D[suiv][pred].b*prey+D[suiv][pred].c)/100,(D[suiv][pred].a*interbord[0].x+D[suiv][pred].b*interbord[0].y+D[suiv][pred].c)/100),suiv, pred);
                    if (((milieu[0].x-interbord[0].x)*(milieu[0].x-interbord[0].x)+(milieu[0].y-interbord[0].y)*(milieu[0].y-interbord[0].y)) < min && /*((D[suiv][pred].a*prex+D[suiv][pred].b*prey+D[suiv][pred].c)/100*(D[suiv][pred].a*interbord[0].x+D[suiv][pred].b*interbord[0].y+D[suiv][pred].c)/100)<=0 &&*/ -1<interbord[0].x && interbord[0].x<HPLAN+1 && -1<interbord[0].y && interbord[0].y<LPLAN+1 && ( interbord[0].x!=0 || interbord[0].y!=0) && (voisin==Pp || voisin==q) ){
                        min = ((milieu[0].x-interbord[0].x)*(milieu[0].x-interbord[0].x)+(milieu[0].y-interbord[0].y)*(milieu[0].y-interbord[0].y));
                        S[Pp][q].x2=interbord[0].x;
                        S[Pp][q].y2=interbord[0].y;
                        S[q][Pp].x2=interbord[0].x;
                        S[q][Pp].y2=interbord[0].y;//printf("\nTEst 4");printf("\n--- %d : %d %d %d %d %d\n", q, S[Pp][q].x1,S[Pp][q].y2,S[Pp][q].x2,S[Pp][q].y2, Pp);
                    }
                }
            }
            if(Pp==prem)
                bordprem++;
        }
        if (S[Pp][q].x1==0 && S[Pp][q].y1==0 ){
            bord++;
            min = HPLAN*HPLAN+LPLAN*LPLAN;
            Point * milieu = milieuPoint( Pp, q, p );
            for(k=0;k<4;k++){
                Point * interbord = intersection_droit (D, B, Pp, q, k);
                if ( interbord[0].x>-1 && interbord[0].x<HPLAN+1 && interbord[0].y>-1 && interbord[0].y<LPLAN+1 && ((milieu[0].x-interbord[0].x)*(milieu[0].x-interbord[0].x)+(milieu[0].y-interbord[0].y)*(milieu[0].y-interbord[0].y)) < min){
                    min = ((milieu[0].x-interbord[0].x)*(milieu[0].x-interbord[0].x)+(milieu[0].y-interbord[0].y)*(milieu[0].y-interbord[0].y));
                    S[Pp][q].x1=interbord[0].x;
                    S[Pp][q].y1=interbord[0].y;
                    S[q][Pp].x1=interbord[0].x;
                    S[q][Pp].y1=interbord[0].y;
                }
            }
            min = HPLAN*HPLAN+LPLAN*LPLAN;
            for(k=0;k<4;k++){
                Point * interbord = intersection_droit (D, B, Pp, q, k);
                if (interbord[0].x>-1 && interbord[0].x<HPLAN+1 && interbord[0].y>-1 && interbord[0].y<LPLAN+1 && (S[Pp][q].x1-milieu[0].x)*(interbord[0].x-milieu[0].x)+(S[Pp][q].y1-milieu[0].y)*(interbord[0].y-milieu[0].y) < 0 && ((milieu[0].x-interbord[0].x)*(milieu[0].x-interbord[0].x)+(milieu[0].y-interbord[0].y)*(milieu[0].y-interbord[0].y)) < min){
                    min = ((milieu[0].x-interbord[0].x)*(milieu[0].x-interbord[0].x)+(milieu[0].y-interbord[0].y)*(milieu[0].y-interbord[0].y));
                    S[Pp][q].x2=interbord[0].x;
                    S[Pp][q].y2=interbord[0].y;
                    S[q][Pp].x2=interbord[0].x;
                    S[q][Pp].y2=interbord[0].y;
                }
            }
        }
    //    printf("\n %d : %d %d %d %d %d\n", q, S[Pp][q].x1,S[Pp][q].y1,S[Pp][q].x2,S[Pp][q].y2, Pp);
        int pred2=pred;
        if (Pp!=prem)
            pred=suiv;
        Pp2=Pp;

     //   printf("\nS[Pp][q].suiv1=%d pred %d suiv %d pred2 %d", S[Pp][q].suiv1, pred, suiv, pred2);
        fin++;
        if ((test[0].x!=S[Pp][q].x1 || test[0].y!=S[Pp][q].y1)&& pred2!=S[Pp][q].suiv1){
            test[0].x=S[Pp][q].x1;
            test[0].y=S[Pp][q].y1;
            Pp=S[Pp][q].suiv1;
    //        printf("\na %d", Pp);
        }
        else if( pred2!=S[Pp][q].suiv2){
            test[0].x=S[Pp][q].x2;
            test[0].y=S[Pp][q].y2;
            Pp=S[Pp][q].suiv2;
    //        printf("\nb %d", Pp);
        }
   /* test */
        if (bord==1 && S[Pp2][q].suiv1==prem && bouclebord==1){
            Pp=S[Pp2][q].suiv2;
            bouclebord++;
     //       printf("\nc %d", Pp);
        }
   /* fintest */
        if (bord==1 && Pp2!=prem && bordprem!=1 && bouclebord==0){
            Pp=S[prem][q].suiv2;
            bouclebord++;
     //       printf("\nc %d ", Pp);
        }
        if (bord==2 ||fin==npts){
            Pp=prem;
    //        printf("\nd %d", Pp);
        }
        suiv=Pp;

    }

}


void gestion_sommet(Point * p, Droite ** D, int q, int npts, int ** germe, Seg ** S){

    int i, j;
    Seg ** S2 = malloc(npts*sizeof(Seg *));
    for(i=0;i<npts;i++)
        S2[i]=malloc(npts*sizeof(Seg));
    for(i=0;i<npts;i++)
        for(j=0;j<npts;j++)
            S2[i][j]=S[i][j];
    Point * provisoire = malloc(sizeof(Point));
    Point * provisoire2 = malloc(sizeof(Point));
//    printf("\nq=%d\n",q);
    for(i=0;i<q;i++){
      //  printf("\n");
        for(j=i+1;j<q;j++){
            provisoire[0].x=S[i][j].x1;
            provisoire[0].y=S[i][j].y1;
          //  printf("\n%d  %d\n",i, j);
            provisoire2[0].x=S[i][j].x2;
            provisoire2[0].y=S[i][j].y2;
            if ( plus_proche_voisin_point(p, provisoire, q, npts)==q && plus_proche_voisin_point(p, provisoire2, q, npts)==q && germe[i][j]!=-1){
                germe[i][j]=-1;
                germe[j][i]=-1;
                S[i][j].x1=-1;
                S[i][j].y1=-1;
                S[j][i].x1=-1;
                S[j][i].y1=-1;
                S[i][j].x2=-1;
                S[i][j].y2=-1;
                S[j][i].x2=-1;
                S[j][i].y2=-1;
            }

            if( j!=q && (S[i][j].x1!=-1 || S[i][j].y1!=-1 || S[i][j].x2!=-1 || S[i][j].y2!=-1) ){
                provisoire[0].x=S[i][j].x1;
                provisoire[0].y=S[i][j].y1;

         /*       if(i==0 && j ==2){
                    printf("voisin =%d\n",plus_proche_voisin_point(p, provisoire, q, npts));
                    printf("\n %d %d \n", (provisoire[0].x-p[0].x)*(provisoire[0].x-p[0].x)+(provisoire[0].y-p[0].y)*(provisoire[0].y-p[0].y),(provisoire[0].x-p[4].x)*(provisoire[0].x-p[4].x)+(provisoire[0].y-p[4].y)*(provisoire[0].y-p[4].y) );
                    printf("\n %d %d \n", provisoire[0].x, provisoire[0].y);
                }*/
          //      printf("\n0 4 -- %d %d \n", S2[0][4].x2, S2[4][0].y2);
                if(plus_proche_voisin_point(p, provisoire, q, npts)==q){
                    if( abs(D[i][j].a*S[i][q].x1+D[i][j].b*S[i][q].y1+D[i][j].c) < abs(D[i][j].a*S[i][q].x2+D[i][j].b*S[i][q].y2+D[i][j].c) ){
                        S2[i][j].x1=S[i][q].x1;
                        S2[i][j].y1=S[i][q].y1;
                        S2[j][i].x1=S[i][q].x1;
                        S2[j][i].y1=S[i][q].y1;
                 //       printf("test1 %d %d\n", i, j);
            //            printf("\n %d %d \n", S2[i][j].x1, S2[i][j].y1);
                    }
                    else{
                        S2[i][j].x1=S[i][q].x2;
                        S2[i][j].y1=S[i][q].y2;
                        S2[j][i].x1=S[i][q].x2;
                        S2[j][i].y1=S[i][q].y2;
                 //       printf("test2 %d %d\n", i, j);
                //        printf("\n %d %d \n", S2[i][q].x2, S2[i][q].y2);
              //          printf("\n %d %d \n", S2[i][j].x1, S2[i][j].y1);
                    }
                }
                else{
                    provisoire[0].x=S[i][j].x2;
                    provisoire[0].y=S[i][j].y2;
                    if(plus_proche_voisin_point(p, provisoire, q, npts)==q){
                //        printf("\n%d %d\n", abs(D[i][j].a*S[i][q].x1+D[i][j].b*S[i][q].y1+D[i][j].c) , abs(D[i][j].a*S[i][q].x2+D[i][j].b*S[i][q].y2+D[i][j].c));
                        if( abs(D[i][j].a*S[i][q].x1+D[i][j].b*S[i][q].y1+D[i][j].c) < abs(D[i][j].a*S[i][q].x2+D[i][j].b*S[i][q].y2+D[i][j].c) ){
                            S2[i][j].x2=S[i][q].x1;
                            S2[i][j].y2=S[i][q].y1;
                            S2[j][i].x2=S[i][q].x1;
                            S2[j][i].y2=S[i][q].y1;
                    //        printf("test3 %d %d\n", i, j);
                //            printf("\n %d %d \n", S2[i][j].x2, S2[i][j].y2);
                        }
                        else{
                            S2[i][j].x2=S[i][q].x2;
                            S2[i][j].y2=S[i][q].y2;
                            S2[j][i].x2=S[i][q].x2;
                            S2[j][i].y2=S[i][q].y2;
                   //         printf("test4 %d %d\n", i, j);
                    //        printf("\n %d %d  %d %d\n", S[i][q].x2, S[i][q].y2,i,q);
                   //         printf("\n %d %d \n", S2[i][j].x2, S2[i][j].y2);
                        }
                    }
                }
            }
        }
    }



    for(i=0;i<npts;i++)
        for(j=0;j<npts;j++)
            S[i][j]=S2[i][j];
}

void reorganise_point(Point * p, int npts){

    int i,j,x,y;
    Point * point = malloc(npts*sizeof(Point));

    int tab[npts];

    /*for(i=0;i<npts;i++){
        printf("%d %d\n",p[i].x, p[i].y);
    }*/

    for(i=0;i<npts;i++){
        tab[i]=(HPLAN/2-p[i].x)*(HPLAN/2-p[i].x)+(LPLAN/2-p[i].y)*(LPLAN/2-p[i].y);
        point[i].x=p[i].x;
        point[i].y=p[i].y;
   //     printf("%d ",tab[i]);
    }
 //   printf("\n");
    for (i = 1; i < npts; ++i) {
        int elem = tab[i];
        x=point[i].x;
        y=point[i].y;
        for (j = i; j > 0 && tab[j-1] > elem; j--){
            tab[j] = tab[j-1];
            point[j].x=point[j-1].x;
            point[j].y=point[j-1].y;
        }
        tab[j] = elem;
        point[j].x=x;
        point[j].y=y;
    }
    for(i=0;i<npts;i++)
 // //      printf("%d ",tab[i]);

    for(i=0;i<npts;i++){
        p[i].x=point[i].x;
        p[i].y=point[i].y;
     //   printf("\n%d  %d",p[i].x, p[i].y);
    }
}

void germe_zero(int ** germe, Seg ** S, int npts){

    int i, j;

    for(i=0;i<npts;i++)
        for(j=0;j<npts;j++)
            if ((S[i][j].x1==0 && S[i][j].y1==0) || (S[i][j].x2==0 && S[i][j].y2==0))
                germe[i][j]=-1;

}
