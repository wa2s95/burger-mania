#include "intro_jeu.h"

#define NIMAGE 8
#define IMAGE 3


void intro_jeu() {                                                                                                      //On initialise les bitmaps et sample de musiques
    BITMAP *fond, *logo_intro, *logo_redim;
    BITMAP *buffer, *texte_temp;
    BITMAP *palmier_g[NIMAGE], *palmier_d[NIMAGE], *palmier_g1[NIMAGE], *palmier_d1[NIMAGE];
    BITMAP *noix_anim[3], *noix_temp;
    SAMPLE* musiqueIntro = load_sample("musiqueIntro.wav");


    if (!musiqueIntro) {
        allegro_message("Erreur lors du chargement du fichier musiqueIntro.wav");

    } else {

        play_sample(musiqueIntro, 50, 128, 1000, TRUE);                                                 //On joue la musique d'intro


    }
    char filename[256];
    int i, frame = 0, compteur = 0;
    int delai_anim_g = 8;
    int intro_y = -100, intro_y_final = 150, intro_vitesse = 4;


    logo_intro = load_bitmap("logo_intro.bmp", NULL);
    fond       = load_bitmap("plage_1.bmp", NULL);
    if (!logo_intro || !fond) {
        allegro_message("Erreur chargement intro");
        exit(EXIT_FAILURE);
    }


    logo_redim = create_bitmap(1200, 600);                                                                      //On initialise le logo BurgerMania redimensionné
    clear_to_color(logo_redim, makecol(255, 0, 255));
    stretch_blit(logo_intro, logo_redim, 0, 0, logo_intro->w, logo_intro->h,0, 0, 600, 400);


    for (i = 0; i < NIMAGE; i++) {                                                                                      //On charge les diférents sprites des palmiers gauches puis droites
        sprintf(filename, "palmier_gauche_%d.bmp", i);
        palmier_g[i]  = load_bitmap(filename, NULL);
        sprintf(filename, "palmier_droite_%d.bmp", i);
        palmier_d[i]  = load_bitmap(filename, NULL);
        sprintf(filename, "palmier_gauche_%d.bmp", i);
        palmier_g1[i] = load_bitmap(filename, NULL);
        sprintf(filename, "palmier_droite_%d.bmp", i);
        palmier_d1[i] = load_bitmap(filename, NULL);
        if (!palmier_g[i] || !palmier_d[i] || !palmier_g1[i] || !palmier_d1[i]) {
            allegro_message("Erreur chargement %s", filename);
            exit(EXIT_FAILURE);
        }
    }


    for (i = 0; i < 3; i++) {
        sprintf(filename, "noixcoco_%d.bmp", i+1);
        noix_anim[i] = load_bitmap(filename, NULL);
        if (!noix_anim[i]) {
            allegro_message("Erreur chargement %s", filename);
            exit(EXIT_FAILURE);
        }
    }

                                                                                                                        // Création des buffers
    buffer     = create_bitmap(SCREEN_W, SCREEN_H);
    texte_temp = create_bitmap(400, 30);
    noix_temp  = create_bitmap(noix_anim[0]->w, noix_anim[0]->h);

                                                                                                                        // Boucle d'intro
    while (1) {

                                                                                                                        // Affichage du fond
        stretch_blit(fond, buffer, 0, 0, fond->w, fond->h, 0, 0, SCREEN_W, SCREEN_H);

                                                                                                                        // Animation palmiers
        compteur++;
        if (compteur >= delai_anim_g) {
            compteur = 0;
            frame = (frame + 1) % NIMAGE;
        }
                                                                                                                        // Affichage des palmiers
        draw_sprite(buffer, palmier_g[frame],  -180, 220);
        draw_sprite(buffer, palmier_g1[frame], -130, 20);
        draw_sprite(buffer, palmier_d[frame],  SCREEN_W - palmier_d[frame]->w - 50, SCREEN_H - palmier_d[frame]->h - 150);
        draw_sprite(buffer, palmier_d1[frame], SCREEN_W - palmier_d1[frame]->w - 20, SCREEN_H - palmier_d1[frame]->h);

                                                                                                                        // Mouvement du logo
        if (intro_y < intro_y_final) {
            intro_y += intro_vitesse;
            if (intro_y > intro_y_final) intro_y = intro_y_final;
        }
        masked_blit(logo_redim, buffer, 0, 0, (SCREEN_W - 600) / 2, intro_y, 600, 400);

                                                                                                                        // Texte "Appuyez sur une touche"
        if (intro_y == intro_y_final) {
            clear_to_color(texte_temp, makecol(255, 0, 255));
            textout_centre_ex(texte_temp, font, "Appuyez sur une TOUCHE pour COMMENCER", texte_temp->w / 2, 0, makecol(255, 0, 0), -1);
            stretch_sprite(buffer, texte_temp, (SCREEN_W - 400) / 2, intro_y + 430, 400, 30);
            if (keypressed()) {
                clear_keybuf();
                break;
            }
        }

                                                                                                                        // Sortie rapide avec Échap
        if (key[KEY_ESC]) {
            clear_keybuf();
            break;
        }

                                                                                                                         // Affichage final
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        rest(20);
    }

                                                                                                                        // Libération mémoire
    stop_sample(musiqueIntro);
    destroy_sample(musiqueIntro);
    destroy_bitmap(buffer);
    destroy_bitmap(texte_temp);
    destroy_bitmap(logo_redim);
    destroy_bitmap(logo_intro);
    destroy_bitmap(fond);
    destroy_bitmap(noix_temp);
    for (i = 0; i < NIMAGE; i++) {
        destroy_bitmap(palmier_g[i]);
        destroy_bitmap(palmier_d[i]);
        destroy_bitmap(palmier_g1[i]);
        destroy_bitmap(palmier_d1[i]);
    }

    for (i = 0; i < 3; i++) destroy_bitmap(noix_anim[i]);
}
