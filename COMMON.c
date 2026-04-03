#include "COMMON.h"

int detection_noir(int color) {
    return getr(color) == 0 && getg(color) == 0 && getb(color) == 0;                                                    //On détecte la couleur du pixel souhaité
}

int collision_bas(BITMAP *bmp, int x, int y, int w) {
    for (int i = 0; i < w; i++) {
        int cx = x + i;
        if (cx >= 0 && y >= 0 && cx < bmp->w && y < bmp->h) {
            if (detection_noir(getpixel(bmp, cx, y))) return 1;                                                  //On scanne les pixels de la ligne du bas de la hitbox du personage
                                                                                                                            // pour savoir si il y a collision ou non
        }
    }
    return 0;
}

int hauteur_obstacle(BITMAP *bmp, int x, int y, int h, int direction, int joueur_largeur) {                             // On scanne les pixels devant le personnage (sa hitbbox) à partir du bas:
                                                                                                                        // et si 4 pixels ou plus de hauteur sont rencontrés, le personnage est bloqué.
    int bord_x = (direction > 0) ? x + joueur_largeur : x;
    int base_y = y + h - 1;
    for (int i = 0; i <= 4; i++) {
        int cy = base_y - i;
        if (bord_x >= 0 && cy >= 0 && bord_x < bmp->w && cy < bmp->h) {
            if (!detection_noir(getpixel(bmp, bord_x, cy))) return i;
        }
    }
    return 5;
}

void afficher_ecran_fin() {                                                                                                                            //Lorsque la victoire est atteinte, un écran de fin s'affiche avec les différentes options possibles
    clear_to_color(screen, makecol(0, 0, 0));
    textprintf_centre_ex(screen, font, SCREEN_W / 2, SCREEN_H / 2 - 40, makecol(255, 0, 0), -1, "NIVEAU TERMINE !");
    textprintf_centre_ex(screen, font, SCREEN_W / 2, SCREEN_H / 2, makecol(255, 255, 255), -1, "[R] Recommencer");
    textprintf_centre_ex(screen, font, SCREEN_W / 2, SCREEN_H / 2 + 30, makecol(255, 255, 255), -1, "[ESC] Quitter");
}



BITMAP* charger_sprite_redimensionne(const char* nom_fichier, int largeur, int hauteur) {                                       //On charge un fichier .bmp, puis on le redimensione selon le besoin
    BITMAP *original = load_bitmap(nom_fichier, NULL);
    if (!original) {
        allegro_message("Erreur chargement %s", nom_fichier);
        exit(1);
    }
    BITMAP *resized = create_bitmap(largeur, hauteur);
    clear_to_color(resized, makecol(255, 0, 255));
    stretch_blit(original, resized, 0, 0, original->w, original->h, 0, 0, largeur, hauteur);
    destroy_bitmap(original);
    return resized;
}

void dessiner_joueur(BITMAP *page, int en_saut, BITMAP **frames, int frame,                                                         //On dessine le joueur selon son état : si il n'est pas en saut, on affiche successivement
                                                                                                                        // les différents sprites du tableau frame[) qui contient les frames du personnage (de 1 à 6).
                     BITMAP *sprite_saut, int joueur_x, int joueur_y, int screenx) {
    if (en_saut) {
        draw_sprite(page, sprite_saut, joueur_x - sprite_saut->w / 2 - screenx, joueur_y - sprite_saut->h / 2);     // Si il est en saut, on affiche le sprite_saut, sinon les sprites normaux.
    } else {
        draw_sprite(page, frames[frame], joueur_x - frames[frame]->w / 2 - screenx, joueur_y - frames[frame]->h / 2);
    }
}