#include "nv1.h"
#include "COMMON.h"


int collision_collectible(int xj, int yj, int wj, int hj, int xc, int yc, int wc, int hc) { // Permets de déterminer si le joueur est en collision avec l'objet de finde niveau ou des pièces
    return xj < xc + wc && xj + wj > xc &&
           yj < yc + hc && yj + hj > yc;
}



void animer_retrait(BITMAP *page, BITMAP *fresque, BITMAP *fond, BITMAP *requin, int decor_x, int decor_y, int screenx) { // Cette fonction permet de faire l'animation de rétrecissement du collectible de fin de niveau
    int taille = requin->w;
    while (taille > 0) { 												// On fait une boucle tant que la taille du collectible est > à 0
        int fresque_x = screenx / 4;
        blit(fresque, page, fresque_x, 0, 0, 0, SCREEN_W, fresque->h);
        masked_blit(fond, page, screenx, 0, 0, 0, SCREEN_W, fond->h);
        masked_stretch_blit(requin, page, 0, 0, requin->w, requin->h,
            decor_x - screenx + requin->w / 2 - taille / 2,
            decor_y + requin->h / 2 - taille / 2,
            taille, taille);
        blit(page, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        rest(30);													// On fait une pause pour faire appraitre l'animation
        taille -= 25;													//On réduit la taille de 25 pixels à chaque itération
    }
}


void lancer_niveau1() {

BITMAP *fond, *fresque, *page, *collisionmap, *requin, *img_piece;
    const float gravite = 0.5;
    const int joueur_largeur = 40;										//Permets de définir la HITBOX du perso
    const int joueur_hauteur = 40;										//Permets de définir la HITBOX du perso

    float joueur_x = 550, joueur_y = 800, vitesse_y = 0;							// Initialise les coordonées de base du perso
    int en_saut = 0, screenx = 0, hauteur_debug = -1, perdu = 0, niveau_termine = 0;				//Variable permettant de faire fonctionner les différentes conditions du personnages
    const int vitesse_scroll = 5, vitesse_deplacement = 10;
    int collectible_x = 10500, collectible_y = 500, collectible_active = 1;
    float angle_flottement = 0.0, amplitude_flottement = 15.0;							//Variable servant à faire l'animation de flottement des collectibles
    int score = 0;
    int fresque_x = screenx / 2;  // effet de défilement plus lent

    BITMAP *frames[6];       // sprites du joueur
    BITMAP *sprite_saut;     // sprite spécifique pour le saut


    int frame = 0;           // frame actuelle de l'animation
    int compteur = 0;        // pour temporiser de changement de frame
    int delay = 6;           // délai entre frames


    Piece pieces[NB_PIECES] = {
        {906, 817, 1}, {975, 765, 1}, {1040, 719, 1}, {1780, 828, 1}, {1860, 828, 1}, {1940, 828, 1},
        {2070, 1080, 1}, {2520, 555, 1}, {3500, 585, 1}, {3600, 585, 1}, {3700, 585, 1}, {4590, 640, 1},
        {4650, 640, 1}, {4710, 640, 1}, {4770, 640, 1}, {4830, 640, 1}, {5860, 700, 1}, {5930, 670, 1},
        {6000, 640, 1}, {6390, 795, 1}, {7125, 680, 1}, {7200, 615, 1}, {7275, 585, 1}, {8655, 675, 1},
        {8735, 630, 1}, {8825, 670, 1}, {9740, 580, 1}, {9835, 525, 1}, {9935, 580, 1}
    };



    fond = load_bitmap("niveau1_graphisme.bmp", NULL);			                                                	// On charge les bitmaps
    fresque = load_bitmap("fond_1.bmp", NULL);
    collisionmap = load_bitmap("niveau1_collisions.bmp", NULL);
    requin = load_bitmap("requin.bmp", NULL);
    img_piece = load_bitmap("piece.bmp", NULL);

    for (int i = 0; i < 6; i++) {
        char nom[50];
        sprintf(nom, "personnage%d.bmp", i + 1);
        frames[i] = charger_sprite_redimensionne(nom, 100, 100);
    }
    sprite_saut = charger_sprite_redimensionne("personnage_saut.bmp", 100, 100);


    if (!fond || !fresque || !collisionmap || !requin || !img_piece) {
        allegro_message("Erreur de chargement des bitmaps.");
        return;
    }



    page = create_bitmap(SCREEN_W, SCREEN_H);

    while (!key[KEY_ESC]) {         				                                                                	//Boucle qui ne s'arrete pas tant que la touche echap n'est pas pressé
        if (perdu || niveau_termine) {
            afficher_ecran_fin();
            if (key[KEY_R]) {
                joueur_x = 330; joueur_y = 300; vitesse_y = 0;
                screenx = perdu = niveau_termine = en_saut = 0;
                collectible_active = 1; score = 0;
                for (int i = 0; i < NB_PIECES; i++) pieces[i].active = 1;
                rest(300);
            }
            rest(16); continue;
        }

        screenx += vitesse_scroll;				                                                                    	// Permets de faire le scroll de l'écran


        int fresque_x = screenx / 2;					                                                                    //Effet de perspective du personnage
        blit(fresque, page, fresque_x, 0, 0, 0, SCREEN_W, fresque->h);

        masked_blit(fond, page, screenx, 0, 0, 0, SCREEN_W, fond->h);	//Pour afficher les graphismes en premier-plan

        if ((int)joueur_x <= screenx) { perdu = 1; continue; }		                                                    // Si le joueur touche la gauche de l'écran il meurt
        if (key[KEY_RIGHT]) {
            int test_x = joueur_x + vitesse_deplacement;
            int h = hauteur_obstacle(collisionmap, test_x, joueur_y, joueur_hauteur, +1, joueur_largeur);
            hauteur_debug = h;
            if (h <= 3) { if (h > 0) joueur_y -= h; joueur_x += vitesse_deplacement; }
        }

        joueur_y += vitesse_y;				                                                                            // permets de gérer  les mouvements en verticalité
        vitesse_y += gravite;				                                                                            // permets de gérer  les mouvements en verticalité
        if ((int)joueur_y > 1055) { perdu = 1; continue; }
        while (collision_bas(collisionmap, joueur_x, joueur_y + joueur_hauteur, joueur_largeur)) {                      // Si les collision en bas sont actif le personnage ne peut descendre
            joueur_y--; vitesse_y = 0;
        }
        if (key[KEY_SPACE] && !en_saut) { vitesse_y = -12; en_saut = 1; }
        if (collision_bas(collisionmap, joueur_x, joueur_y + joueur_hauteur + 1, joueur_largeur)) en_saut = 0;




        for (int i = 0; i < NB_PIECES; i++) {					// permets de faire l'affichage des pièces
            if (pieces[i].active) {
                int px = pieces[i].x - screenx;
                masked_stretch_blit(img_piece, page, 0, 0, img_piece->w, img_piece->h,
                                    px, pieces[i].y, img_piece->w / 5, img_piece->h / 5);
                if (collision_collectible(joueur_x, joueur_y, joueur_largeur, joueur_hauteur,
                                          pieces[i].x, pieces[i].y, img_piece->w, img_piece->h)) {
                    pieces[i].active = 0; score++;
                }
            }
        }


        if (collectible_active) {
            int offset_y = collectible_y + sin(angle_flottement) * amplitude_flottement;
            draw_sprite(page, requin, collectible_x - screenx, offset_y);
            angle_flottement += 0.1; if (angle_flottement > 2 * M_PI) angle_flottement -= 2 * M_PI;
            if (collision_collectible(joueur_x, joueur_y, joueur_largeur, joueur_hauteur,
                                      collectible_x, collectible_y, requin->w, requin->h)) {
                collectible_active = 0;
                animer_retrait(page, fresque, fond, requin, collectible_x, collectible_y, screenx);
                niveau_termine = 1;
            }
        }

        dessiner_joueur(page, en_saut, frames, frame, sprite_saut, (int)joueur_x, (int)joueur_y, screenx);

        if (!en_saut) {
            compteur++;
            if (compteur >= delay) {
                compteur = 0;
                frame = (frame + 1) % 6;
            }
        }


        textprintf_ex(page, font, 10, 10, makecol(255, 255, 255), -1, "Hauteur obstacle: %d", hauteur_debug);
        textprintf_ex(page, font, 10, 30, makecol(255, 255, 255), -1, "X global: %d", (int)(joueur_x));
        textprintf_ex(page, font, 10, 50, makecol(255, 255, 0), -1, "Pieces: %d", score);

        blit(page, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        rest(16);
    }

    destroy_bitmap(fond); destroy_bitmap(fresque); destroy_bitmap(collisionmap);
    destroy_bitmap(requin); destroy_bitmap(img_piece); destroy_bitmap(page);



}