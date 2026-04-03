#include "nv3.h"
#include "COMMON.h"


void animer_retrait_salameche(BITMAP *page, BITMAP *fond, BITMAP *salameche, int decor_x, int decor_y, int screenx) {
    int taille = salameche->w;
    while (taille > 0) {
        blit(fond, page, screenx, 0, 0, 0, SCREEN_W, fond->h);
        masked_stretch_blit(salameche, page, 0, 0, salameche->w, salameche->h,
            decor_x - screenx + salameche->w / 2 - taille / 2,
            decor_y + salameche->h / 2 - taille / 2,
            taille, taille);
        blit(page, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        rest(30);
        taille -= 25;
    }
}


void lancer_niveau3() {

BITMAP *fond, *page, *collisionmap, *salameche, *bonus2,*fresque;
    BITMAP *fusee_img[5]; // fusee0 pour l'activation, fusee1-4 pour animation
    int anim_index = 0;
    int anim_timer = 0;
    int anim_delay = 5; // nb de frames avant de changer de sprite
    int fusee_active_affichee = 0; // 1 pendant le premier affichage
    const float gravite = 0.5;
    const int joueur_largeur = 40;
    const int joueur_hauteur = 40;

    float joueur_x = 330, joueur_y = 800, vitesse_y = 0;
    int en_saut = 0, screenx = 0, hauteur_debug = -1;
    int perdu = 0, niveau_termine = 0;
    const int vitesse_scroll = 6, vitesse_deplacement = 10;

    int salameche_x = 12600, salameche_y = 650, salameche_active = 1;
    float angle_flottement = 0;
    const float amplitude_flottement = 15;

    BITMAP *frames[6];  // tableaux de sprites
    int frame = 0;
    int compteur = 0;
    int delay = 6;

    // Boules de feu
    #define NB_BOULES 8
    int boule_x[NB_BOULES] = { 1900, 2350, 5500,3155,3480,4260,6240,8700 };
    int boule_y[NB_BOULES] = { 1300, 1400, 1200,880,655,650,1200,1200 };
    int boule_base_y[NB_BOULES] = { 1300, 1400, 1200,880,655,650,1200,1200 };
    int boule_amplitude[NB_BOULES] = { 700, 750, 1080,200,250,400,1080,1080 };
    int boule_direction[NB_BOULES] = { -1, -1, -1,-1,-1,-1,-1,-1 };
    int rayon_boule = 55;

    #define NB_PIECES 18
    int piece_x[NB_PIECES] = { 1875, 2130, 2620, 2935, 3200, 3400, 3965, 5130, 5430, 5940,6710,7030,7500,8340,8800,9340,9600,9700 };
    int piece_y[NB_PIECES] = { 670, 720, 630, 550, 540, 750, 860, 620, 540, 460,323,404,430,730,660,640,605,605 };
    int piece_active[NB_PIECES] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    int score = 0;
    BITMAP *piece_bmp;


    // Bonus fusée
    int fusee_x = 4390, fusee_y = 910;
    int fusee_largeur = 40, fusee_hauteur = 40;
    int fusee_active = 1, mode_fusee = 0;
    int duree_fusee_ms = 35000, fusee_timer = 0;

    // Bonus ralentisseur
    int bonus2_x = 1560, bonus2_y = 715;
    int bonus2_active = 1, bonus2_affiche = 0;
    int ralenti_active = 0, ralenti_timer = 0;
    int duree_ralenti_ms = 8000;
    float ralenti_facteur = 0.5;


    fond = load_bitmap("Troisieme_niveau_graphique.bmp", NULL);
    collisionmap = load_bitmap("troisieme_niveau_collisions.bmp", NULL);
    salameche = load_bitmap("salameche.bmp", NULL);
    bonus2 = load_bitmap("bonus2.bmp", NULL);
    piece_bmp = load_bitmap("piece.bmp", NULL);
    fresque = load_bitmap("fondNv3.bmp", NULL);

    for (int i = 0; i < 6; i++) {
        char nom[50];
        sprintf(nom, "personnage%d.bmp", i + 1);
        BITMAP *tmp = load_bitmap(nom, NULL);
        if (!tmp) {
            allegro_message("Erreur chargement %s", nom);
            return;
        }

        frames[i] = create_bitmap(100, 100);
        clear_to_color(frames[i], makecol(255, 0, 255)); // fond transparent
        stretch_blit(tmp, frames[i], 0, 0, tmp->w, tmp->h, 0, 0, 100, 100);
        destroy_bitmap(tmp);
    }

    char filename[32];
    for (int i = 0; i < 5; i++) {
        sprintf(filename, "fusee%d.bmp", i);
        fusee_img[i] = load_bitmap(filename, NULL);
        if (!fusee_img[i]) {
            allegro_message("Erreur de chargement de %s", filename);
            return;
        }
    }
    if (!fond || !collisionmap || !salameche || !bonus2 || !piece_bmp|| !fresque ) {
        allegro_message("Erreur de chargement des bitmaps.");
        return;
    }

    page = create_bitmap(SCREEN_W, SCREEN_H);

    while (!key[KEY_ESC]) {
        if (ralenti_active && (clock() - ralenti_timer > duree_ralenti_ms)) {
            ralenti_active = 0;
        }

        if (perdu || niveau_termine) {
            afficher_ecran_fin();
            if (key[KEY_R]) {
                screenx = 0;
                joueur_x = 330;
                joueur_y = 300;
                vitesse_y = 0;
                en_saut = 0;
                perdu = 0;
                niveau_termine = 0;
                salameche_active = 1;
                for (int i = 0; i < NB_BOULES; i++) {
                    boule_y[i] = boule_base_y[i];
                    boule_direction[i] = -1;
                }
                fusee_active = 1;
                mode_fusee = 0;
                fusee_timer = 0;
                bonus2_active = 1;
                bonus2_affiche = 0;
                ralenti_active = 0;
                rest(300);
            }
            rest(16);
            continue;
        }

        clear_bitmap(page);

        int vitesse_actuelle_scroll = ralenti_active ? vitesse_scroll * ralenti_facteur : vitesse_scroll;
        screenx += vitesse_actuelle_scroll;

        for (int i = 0; i < NB_BOULES; i++) {
            boule_y[i] += boule_direction[i] * 15;
            if (boule_y[i] <= boule_base_y[i] - boule_amplitude[i]) boule_direction[i] = 1;
            else if (boule_y[i] >= boule_base_y[i]) boule_direction[i] = -1;
        }

        if (mode_fusee && (clock() - fusee_timer > duree_fusee_ms)) mode_fusee = 0;

        if ((int)joueur_x <= screenx) {
            perdu = 1;
            continue;
        }

        if (!mode_fusee && key[KEY_RIGHT]) {
            int test_x = joueur_x + vitesse_deplacement;
            int h = hauteur_obstacle(collisionmap, (int)test_x, (int)joueur_y, joueur_hauteur, +1, joueur_largeur);
            hauteur_debug = h;
            if (h <= 3) {
                if (h > 0) joueur_y -= h;
                joueur_x += vitesse_deplacement;
            }
        }

        float facteur = ralenti_active ? ralenti_facteur : 1.0;
        if (mode_fusee) {
            if (key[KEY_SPACE]) vitesse_y = -7;
            joueur_x += vitesse_deplacement-5;
        } else if (key[KEY_SPACE] && !en_saut) {
            vitesse_y = -12;
            en_saut = 1;
        }

        joueur_y += vitesse_y * facteur;
        vitesse_y += gravite * facteur;

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

        blit(fond, page, screenx, 0, 0, 0, SCREEN_W, fond->h);
        int fresque_x = screenx * 0.3; // 0.3 = vitesse plus lente que le fond
        masked_blit(fresque, page, fresque_x, 0, 0, 0, SCREEN_W, fresque->h);
        masked_blit(fond, page, screenx, 0, 0, 0, SCREEN_W, fond->h);
        for (int i = 0; i < NB_PIECES; i++) {
            if (piece_active[i]) {
                int px = piece_x[i] - screenx;
                masked_stretch_blit(piece_bmp, page, 0, 0, piece_bmp->w, piece_bmp->h,
                                    px, piece_y[i], piece_bmp->w/3, piece_bmp->h/3);
            }
        }
        for (int i = 0; i < NB_BOULES; i++) {
            int boule_draw_x = boule_x[i] - screenx;
            circlefill(page, boule_draw_x, boule_y[i], rayon_boule, makecol(255, 0, 0));
        }
        for (int i = 0; i < NB_BOULES; i++) {
            int centre_boule_x = boule_x[i];
            int centre_boule_y = boule_y[i];

            int joueur_centre_x = joueur_x + joueur_largeur / 2;
            int joueur_centre_y = joueur_y + joueur_hauteur / 2;

            int dx = joueur_centre_x - centre_boule_x;
            int dy = joueur_centre_y - centre_boule_y;
            int distance_carre = dx * dx + dy * dy;
            int rayon_collision = rayon_boule + joueur_largeur / 2;

            if (distance_carre < rayon_collision * rayon_collision) {
                perdu = 1;
            }
        }
        for (int i = 0; i < NB_PIECES; i++) {
            if (piece_active[i]) {
                int joueur_centre_x = joueur_x + joueur_largeur / 2;
                int joueur_centre_y = joueur_y + joueur_hauteur / 2;
                int piece_centre_x = piece_x[i] + piece_bmp->w / 2;
                int piece_centre_y = piece_y[i] + piece_bmp->h / 2;

                int dx = joueur_centre_x - piece_centre_x;
                int dy = joueur_centre_y - piece_centre_y;
                int distance2 = dx * dx + dy * dy;

                int rayon_collision = (joueur_largeur / 2) + (piece_bmp->w / 2);
                if (distance2 < rayon_collision * rayon_collision) {
                    piece_active[i] = 0;
                    score++;
                }
            }
        }
        if (fusee_active) {
            int fx = fusee_x - screenx;
            masked_blit(fusee_img[0], page, 0, 0, fx, fusee_y, fusee_img[0]->w, fusee_img[0]->h);
        }


        if (bonus2_active) {
            int bx = bonus2_x - screenx;
            masked_stretch_blit(bonus2, page, 0, 0, bonus2->w, bonus2->h,
                                bx, bonus2_y, bonus2->w/4, bonus2->h/4);
        }

        if (bonus2_affiche) {
            masked_stretch_blit(bonus2, page, 0, 0, bonus2->w, bonus2->h,
                                20, 60, bonus2->w/4, bonus2->h/4);
        }

        if (fusee_active &&
            joueur_x + joueur_largeur > fusee_x &&
            joueur_x < fusee_x + fusee_largeur &&
            joueur_y + joueur_hauteur > fusee_y &&
            joueur_y < fusee_y + fusee_hauteur) {
            fusee_active = 0;
            mode_fusee = 1;
            fusee_timer = clock();
            vitesse_y = 0;
        }

        if (bonus2_active &&
            joueur_x + joueur_largeur > bonus2_x &&
            joueur_x < bonus2_x + bonus2->w &&
            joueur_y + joueur_hauteur > bonus2_y &&
            joueur_y < bonus2_y + bonus2->w) {
            bonus2_active = 0;
            bonus2_affiche = 1;
            ralenti_active = 1;
            ralenti_timer = clock();
        }

        if (salameche_active) {
            int offset_y = salameche_y + (int)(sin(angle_flottement) * amplitude_flottement);
            masked_blit(salameche, page, 0, 0, salameche_x - screenx, offset_y, salameche->w, salameche->h);
            angle_flottement += 0.1;
            if (angle_flottement > 2 * M_PI) angle_flottement -= 2 * M_PI;
        }

        if (salameche_active &&
            joueur_x + joueur_largeur > salameche_x &&
            joueur_x < salameche_x + salameche->w &&
            joueur_y + joueur_hauteur > salameche_y &&
            joueur_y < salameche_y + salameche->h) {
            salameche_active = 0;
            animer_retrait_salameche(page, fond, salameche, salameche_x, salameche_y, screenx);
            niveau_termine = 1;
        }
        int joueur_aff_x = (int)joueur_x - screenx;
        int joueur_aff_y = (int)joueur_y;

        if (mode_fusee) {

            // Animation du sprite fusée
            if (++anim_timer >= anim_delay) {
                anim_timer = 0;
                anim_index = (anim_index + 1) % 4; // fusee1 à fusee4
            }
            masked_blit(fusee_img[1 + anim_index], page, 0, 0,
                        (int)joueur_x - screenx, (int)joueur_y,
                        fusee_img[1 + anim_index]->w, fusee_img[1 + anim_index]->h);
        } else {
            // Sprite normal (rectangle jaune)
            draw_sprite(page, frames[frame],
    (int)(joueur_x - frames[frame]->w / 2 - screenx),
    (int)(joueur_y - frames[frame]->h / 2));

        }

        compteur++;
        if (compteur >= delay) {
            compteur = 0;
            frame = (frame + 1) % 6;
        }



        textprintf_ex(page, font, 10, 10, makecol(255, 255, 255), -1, "Hauteur obstacle: %d", hauteur_debug);
        textprintf_ex(page, font, 10, 30, makecol(255, 255, 255), -1, "X global: %d", (int)(joueur_x));
        textprintf_ex(page, font, 10, 50, makecol(255, 255, 0), -1, "Score: %d", score);
        blit(page, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        rest(16);
    }





    destroy_bitmap(fresque);
    destroy_bitmap(piece_bmp);
    destroy_bitmap(fond);
    destroy_bitmap(collisionmap);
    destroy_bitmap(page);
    destroy_bitmap(salameche);
    destroy_bitmap(bonus2);
    for (int i = 0; i < 5; i++) destroy_bitmap(fusee_img[i]);
    for (int i = 0; i < 6; i++) destroy_bitmap(frames[i]);

}
