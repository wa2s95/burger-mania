#ifndef MENU_H
#define MENU_H



#include <allegro.h>

void saisir_pseudo(char pseudo[], BITMAP *fond);
int afficher_menu_joueur(BITMAP *buffer, BITMAP *fond);
int choisir_niveau(BITMAP *buffer, BITMAP *fond);
int verification_pseudo(const char pseudo[]);

#endif
