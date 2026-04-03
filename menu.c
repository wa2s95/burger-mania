#include <allegro.h>
#include "menu.h"
#include <string.h>
#include <stdio.h>

#define FICHIER_JOUEURS "joueurs.txt"

void saisir_pseudo(char pseudo[], BITMAP *fond) {


    clear_keybuf();
    int pos = 0;
    int touche;
    int compteur = 0;

    while ((touche = readkey()) >> 8 != KEY_ENTER && pos < 20) {
        if ((touche & 0xff) >= ' ' && (touche & 0xff) <= '~') {
            pseudo[pos++] = touche & 0xff;
            pseudo[pos] = '\0';
        } else if (((touche >> 8) == KEY_BACKSPACE) && pos > 0) {
            pseudo[--pos] = '\0';
        }

        stretch_blit(fond, screen, 0, 0, fond->w, fond->h, 0, 0, SCREEN_W, SCREEN_H);
        rectfill(screen, 90, 190, 550, 230, makecol(0, 0, 0));
        rect(screen, 90, 190, 550, 230, makecol(255, 255, 255));
        textout_ex(screen, font, "Entrez votre pseudo :", 100, 170, makecol(255, 255, 255), -1);
        textprintf_ex(screen, font, 100, 200, makecol(255, 255, 255), -1, "%s", pseudo);

        compteur++;
        if (compteur % 30 < 15) {
            int x = 100 + text_length(font, pseudo);
            vline(screen, x, 200, 210, makecol(255, 255, 255));
        }
    }
}

int verification_pseudo(const char pseudo[]) {
    char pseudoLu[50];
    FILE *fichier = fopen(FICHIER_JOUEURS, "r");
    if (!fichier) return 0;

    while (fgets(pseudoLu, sizeof(pseudoLu), fichier)) {
        pseudoLu[strcspn(pseudoLu, "\n")] = '\0';
        if (strcmp(pseudoLu, pseudo) == 0) {
            fclose(fichier);
            return 1;
        }
    }

    fclose(fichier);
    return 0;
}


int afficher_menu_joueur(BITMAP *buffer, BITMAP *fond) {
    int choix = 0;
    int touche;

    char options[2][50] = {
            "Commencer une nouvelle partie",
            "Reprendre votre ancienne partie"
    };

    while (1) {
        stretch_blit(fond, buffer, 0, 0, fond->w, fond->h, 0, 0, SCREEN_W, SCREEN_H);
        textout_ex(buffer, font, "== Bienvenue, choisissez une option ==", 150, 100, makecol(255,255,255), -1);

        for (int i = 0; i < 2; i++) {
            if (i == choix)
                textout_ex(buffer, font, options[i], 180, 150 + i * 30, makecol(255,0,0), -1);
            else
                textout_ex(buffer, font, options[i], 180, 150 + i * 30, makecol(255,255,255), -1);
        }

        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        touche = readkey() >> 8;

        if (touche == KEY_DOWN || touche == KEY_UP) choix = (choix + 1) % 2;
        else if (touche == KEY_ENTER) return choix;
    }
}

int choisir_niveau(BITMAP *buffer, BITMAP *fond) {
    int choix = 0;
    int touche;

    const char *options[] = {
            "Niveau 1",
            "Niveau 2",
            "Niveau 3",
    };

    while (1) {
        stretch_blit(fond, buffer, 0, 0, fond->w, fond->h, 0, 0, SCREEN_W, SCREEN_H);
        textout_ex(buffer, font, "== Choisissez un niveau ==", 200, 100, makecol(255,255,255), -1);

        for (int i = 0; i < 3; i++) {
            if (i == choix)
                textout_ex(buffer, font, options[i], 240, 160 + i * 40, makecol(255,0,0), -1);
            else
                textout_ex(buffer, font, options[i], 240, 160 + i * 40, makecol(255,255,255), -1);
        }

        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

        touche = readkey() >> 8;
        if (touche == KEY_DOWN) choix = (choix + 1) % 3;
        else if (touche == KEY_UP) choix = (choix + 2) % 3;
        else if (touche == KEY_ENTER) return choix; // 0, 1 ou 2
    }
}