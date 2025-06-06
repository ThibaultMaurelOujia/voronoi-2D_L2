 // gcc -Wall Affiche.c ez-draw.c -o Affiche.exe -lgdi32

#include "ez-draw.h"
#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include "outils.h"

#define npts 50



void win1_on_expose (Ez_event *ev)
{
    printf("\ntestdebut\n");

    srand(time(NULL));

    int ** germe;
    Droite ** D;
    Droite * B;
    Point * p;
    Seg ** S;

    int i,j;

    B = cree_bord();


    generer_points("Liste_Points.txt", npts);

    p = lecture("Liste_Points.txt", npts);

    reorganise_point( p, npts);

    germe = initGerme(npts);
    D = equation_mediatrice(p, npts);

    S = malloc(npts*sizeof(Seg *));
    for(i=0;i<npts;i++)
        S[i]=malloc(npts*sizeof(Seg));
    init_segment(S, p, B , D);


    for(i=3;i<npts;i++){
        //printf("\n%d - ",i);
        intersection_germe(p, D, i, npts, germe, B, S);
        gestion_sommet(p, D, i, npts, germe, S);
        germe_zero(germe,  S,  i);
        germe[i][i]=-1;
    }


    ecrit_seg("Liste_Seg.txt", npts, S, germe);

    for(i=0;i<npts;i++){
        if(i>2)
            ez_set_color (ez_get_RGB ((rand()%256), (rand()%256), (rand()%256)));
        ez_set_thick (4);
        ez_draw_point     (ev->win,  p[i].x/30,  p[i].y/30);
        ez_draw_text      (ev->win, EZ_BL, p[i].x/30-10, p[i].y/30-10, "%d", i);
     //   printf("\n");
        for(j=0;j<npts;j++){
            if(germe[i][j]==1){
                ez_set_thick (2);
                ez_draw_line      (ev->win,  S[i][j].x1/30,  S[i][j].y1/30, S[i][j].x2/30,  S[i][j].y2/30 );
          //      printf("%d, %d : %3d, %3d, %3d, %3d ;", i, j, S[i][j].x1, S[i][j].y1, S[i][j].x2, S[i][j].y2);
            }
        }
    }
  //  printf("\ntest  : %d", S[2][9].suiv2);
/*    ez_set_color (ez_get_RGB (255, 0, 0));
    ez_set_thick (10);
    ez_draw_point     (ev->win,  541,  656);
    ez_draw_point     (ev->win,  565,  656);*/
    Point * centre =centre_cercle_circonscrit(0, 1, 2,p);
  //  printf(" \n cntre %d  %d",  centre->x, centre->y);

}



void win1_on_key_press (Ez_event *ev)
{
    switch (ev->key_sym) {
        case XK_q : ez_quit (); break;
    }

}


void win1_on_event (Ez_event *ev)                /* Called by ez_main_loop() */
{                                                /* for each event on win1   */
    switch (ev->type) {
        case Expose   : win1_on_expose    (ev); break;
        case KeyPress : win1_on_key_press (ev); break;
    }
}



int main ()
{
    Ez_window win1;
    if (ez_init() < 0) exit(1);

    win1 = ez_window_create (HPLAN/30, LPLAN/30, "Test", win1_on_event);

    ez_main_loop ();
    exit(0);
}


