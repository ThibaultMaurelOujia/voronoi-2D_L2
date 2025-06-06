#ifndef __STRUCT__
#define __STRUCT__

#define HPLAN  30000
#define LPLAN  30000



typedef struct {
    long long int x;
    long long int y;
} Point;

typedef struct {
    long long int x;
    long long int y;
} Vect;

typedef struct {
    long long int a;
    long long int b;
    long long int c;
} Droite;

typedef struct {
    long long int x1;
    long long int y1;
    long long int x2;
    long long int y2;
    long long int suiv1;
    long long int suiv2;
} Seg;




void generer_points(char * filename, int npts);

Point * lecture(char * nomFichier, int npts);

Point * centre_cercle_circonscrit( int a, int b, int c, Point * p);

void ecrit_seg(char * nomFichier, int npts, Seg ** S, int ** germe);

Droite * cree_bord();

int ** initGerme(int npts);

Droite ** equation_mediatrice(Point * p, int npts);

void init_segment(Seg ** S, Point * p, Droite * B, Droite ** D);

void intersection_germe(Point * p, Droite ** D, int q, int npts, int ** germe, Droite * B, Seg ** S);

int plus_proche_voisin(Point * p, int a, int npts);

void gestion_sommet(Point * p, Droite ** D, int q, int npts, int ** germe, Seg ** S);

void reorganise_point(Point * p, int npts);

void germe_zero(int ** germe, Seg ** S, int npts);


#endif /*__STRUCT__*/
