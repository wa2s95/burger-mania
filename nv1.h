#ifndef NV1_H
#define NV1_H


#include <allegro.h>
#include <math.h>

#define NB_PIECES 29

typedef struct {
    int x, y;
    int active;
} Piece;




int collision_collectible(int xj, int yj, int wj, int hj, int xc, int yc, int wc, int hc);
void animer_retrait(BITMAP *page, BITMAP *fresque, BITMAP *fond, BITMAP *requin, int decor_x, int decor_y, int screenx);



void lancer_niveau1();

#endif //NV1_H
