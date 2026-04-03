#include "nv2.h"
#include "COMMON.h"



int est_bleu(int color) {
    return getr(color) == 0 && getg(color) == 0 && getb(color) == 255;
}


void animer_retrait_pasteque(BITMAP *page, BITMAP *fond, BITMAP *pasteque, int decor_x, int decor_y, int screenx) {
    int taille = pasteque->w;
    while (taille > 0) {
        masked_blit(fond, page, screenx, 0, 0, 0, SCREEN_W, fond->h);
        masked_stretch_blit(pasteque, page, 0, 0, pasteque->w, pasteque->h,
                            decor_x - screenx + pasteque->w / 2 - taille / 2,
                            decor_y + pasteque->h / 2 - taille / 2,
                            taille, taille);
        blit(page, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        rest(30);
        taille -= 25;
    }
}

void lancer_niveau2() {
    BITMAP *pasteque, *fond, *page, *collisionmap, *fresque2;
    BITMAP *bonus1;
    BITMAP *frames[6];
    BITMAP *sprite_saut;

    // ==== Variables liées au personnage ====
    float joueur_x = 330, joueur_y = 800, vitesse_y = 0;
    int joueur_largeur = 40, joueur_hauteur = 40;
    int en_saut = 0;
    const float gravite = 0.5;

    // ==== Auto-scrolling ====
    int screenx = 0, scroll_active = 0;
    float vitesse_scroll = 3, vitesse_deplacement = 8;

    // ==== Victoire/Défaite ====
    int perdu = 0, niveau_termine = 0;

    // ==== Animation ====
    int frame = 0, compteur = 0, delay = 6;

    // ==== Éléments décoratifs ====
    int pasteque_x = 8558, pasteque_y = 550, pasteque_active = 1;
    float angle_flottement = 0;
    const float amplitude_flottement = 15;

    int bonus1_x = 3500, bonus1_y = 700, bonus1_active = 1, bonus1_affiche = 0;


    fond = load_bitmap("second_niveau_graphique.bmp", NULL);
    collisionmap = load_bitmap("second_niveau_collisions.bmp", NULL);
    pasteque = load_bitmap("pasteque.bmp", NULL);
    bonus1 = load_bitmap("bonus piment.bmp", NULL);
    fresque2 = load_bitmap("jungle.bmp", NULL);

    if (!fond || !collisionmap || !pasteque || !bonus1 || !fresque2) {
        allegro_message("Erreur de chargement des bitmaps.");
        return;
    }

    for (int i = 0; i < 6; i++) {
        char nom[50];
        sprintf(nom, "personnage%d.bmp", i + 1);
        frames[i] = charger_sprite_redimensionne(nom, 100, 100);
    }
    sprite_saut = charger_sprite_redimensionne("personnage_saut.bmp", 100, 100);



    page = create_bitmap(SCREEN_W, SCREEN_H);

    while (!key[KEY_ESC]) {
        if (perdu || niveau_termine) {
            afficher_ecran_fin();
            if (key[KEY_R]) {
                joueur_x = 330;
                joueur_y = 300;
                vitesse_y = 0;
                en_saut = 0;
                perdu = 0;
                niveau_termine = 0;
                screenx = 0;
                scroll_active = 0;
                vitesse_scroll = 3;
                bonus1_active = 1;
                bonus1_affiche = 0;
                pasteque_active = 1;
                rest(300);
            }
            rest(16);
            continue;
        }

        clear_bitmap(page);
        if (scroll_active && screenx + SCREEN_W < fond->w) screenx += vitesse_scroll;

        int jungle_scroll = screenx / 3;
        if (jungle_scroll + SCREEN_W > fresque2->w) jungle_scroll = fresque2->w - SCREEN_W;
        blit(fresque2, page, jungle_scroll, 0, 0, 0, SCREEN_W, fresque2->h);

        masked_blit(fond, page, screenx, 0, 0, 0, SCREEN_W, fond->h);

        if (pasteque_active &&
            joueur_x + joueur_largeur > pasteque_x &&
            joueur_x < pasteque_x + pasteque->w &&
            joueur_y + joueur_hauteur > pasteque_y &&
            joueur_y < pasteque_y + pasteque->h) {
            pasteque_active = 0;
            animer_retrait_pasteque(page, fond, pasteque, pasteque_x, pasteque_y, screenx);
            niveau_termine = 1;
        }

        if (pasteque_active) {
            int offset_y = pasteque_y + (int)(sin(angle_flottement) * amplitude_flottement);
            masked_blit(pasteque, page, 0, 0, pasteque_x - screenx, offset_y, pasteque->w, pasteque->h);
            angle_flottement += 0.1;
            if (angle_flottement > 2 * M_PI) angle_flottement -= 2 * M_PI;
        }

        if (bonus1_active &&
            joueur_x + joueur_largeur > bonus1_x &&
            joueur_x < bonus1_x + bonus1->w &&
            joueur_y + joueur_hauteur > bonus1_y &&
            joueur_y < bonus1_y + bonus1->h) {
            bonus1_active = 0;
            bonus1_affiche = 1;
            scroll_active = 1;
            vitesse_scroll *= 2;
            vitesse_deplacement *= 1.5;
        }

        if (bonus1_active) {
            masked_stretch_blit(bonus1, page, 0, 0, bonus1->w, bonus1->h,
                                bonus1_x - screenx, bonus1_y,
                                bonus1->w / 4, bonus1->h / 4);
        }

        if (bonus1_affiche) {
            masked_stretch_blit(bonus1, page, 0, 0, bonus1->w, bonus1->h,
                                20, 20, bonus1->w / 4, bonus1->h / 4);
        }

        if ((int)joueur_x <= screenx) {
            perdu = 1;
            continue;
        }

        if (key[KEY_RIGHT]) {
            scroll_active = 1;
            int test_x = joueur_x + vitesse_deplacement;
            int h = hauteur_obstacle(collisionmap, (int)test_x, (int)joueur_y, joueur_hauteur, +1, joueur_largeur);
            if (h <= 3) {
                if (h > 0) joueur_y -= h;
                joueur_x += vitesse_deplacement;
                if (screenx + SCREEN_W < fond->w) screenx += vitesse_scroll;
            }
        }

        joueur_y += vitesse_y;
        vitesse_y += gravite;

        int y_pieds = (int)(joueur_y + joueur_hauteur);

        if ((int)joueur_x >= 2415 && (int)joueur_x <= 2930 && y_pieds >= 595 && y_pieds <= 610) {
            vitesse_y = -15;
            en_saut = 1;
        }

        if ((int)joueur_x >= 4143 && (int)joueur_x <= 4894 && y_pieds >= 952 && y_pieds <= 1080) {
            vitesse_y = -25;
            en_saut = 1;
        }



        if (key[KEY_SPACE] && !en_saut) {
            vitesse_y = -13;
            en_saut = 1;
        }

        if ((int)joueur_y > 1055) {
            perdu = 1;
            continue;
        }

        while (collision_bas(collisionmap, (int)joueur_x, (int)(joueur_y + joueur_hauteur), joueur_largeur)) {
            joueur_y -= 1;
            vitesse_y = 0;
        }

        if (collision_bas(collisionmap, (int)joueur_x, (int)(joueur_y + joueur_hauteur + 1), joueur_largeur)) {
            en_saut = 0;
        }

        dessiner_joueur(page, en_saut, frames, frame, sprite_saut, (int)joueur_x, (int)joueur_y, screenx);

        if (!en_saut) {
            compteur++;
            if (compteur >= delay) {
                compteur = 0;
                frame = (frame + 1) % 6;
            }
        }

        blit(page, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        rest(16);
    }

    destroy_bitmap(fond);
    destroy_bitmap(collisionmap);
    destroy_bitmap(page);
    destroy_bitmap(pasteque);
    destroy_bitmap(bonus1);
    destroy_bitmap(sprite_saut);
    destroy_bitmap(fresque2);
    for (int i = 0; i < 6; i++) destroy_bitmap(frames[i]);

}