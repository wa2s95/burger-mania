#ifndef COMMON_H
#define COMMON_H

#include <allegro.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

int detection_noir(int color);
int collision_bas(BITMAP *bmp, int x, int y, int w);
int hauteur_obstacle(BITMAP *bmp, int x, int y, int h, int direction, int joueur_largeur);
void afficher_ecran_fin();
BITMAP* charger_sprite_redimensionne(const char* nom_fichier, int largeur, int hauteur);
void dessiner_joueur(BITMAP *page, int en_saut, BITMAP **frames, int frame, BITMAP *sprite_saut, int joueur_x, int joueur_y, int screenx);

#endif //COMMON_H
