#include <allegro.h>
#include <stdio.h>
#include "init.h"
#include "intro_jeu.h"
#include "menu.h"
#include "COMMON.h"

#include "nv1.h"
#include "nv2.h"
#include "nv3.h"






int main() {                                                                                                            //=================== MAIN ======================\\


        BITMAP *nom_jeu, *map, *nv1, *nv2, *nv3, *buffer;                                                               // On charge les différents Bitmaps utilisés




        SAMPLE* musiqueNV1 = load_sample("musiqueNV1.wav");
        if (!musiqueNV1) {
            allegro_message("Erreur lors du chargement du fichier musiqueNV1.wav");
        }
        SAMPLE* musiqueNV2 = load_sample("musiqueNV2.wav");
        if (!musiqueNV1) {
            allegro_message("Erreur lors du chargement du fichier musiqueNV2.wav");
        }
        SAMPLE* musiqueNV3 = load_sample("musiqueNV3.wav");
        if (!musiqueNV1) {
            allegro_message("Erreur lors du chargement du fichier musiqueNV3.wav");
        }                                                                                                               //On charge  les différentes musiques utilisées

       char pseudo[21];


        initialisation_allegro();                                                                                       // Initialisation Allegro (sous-programme à cet effet)


                                                                                                                         // Chargement des images
        nom_jeu = load_bitmap("nom_jeu.bmp", NULL);
        map     = load_bitmap("maps.bmp", NULL);
        nv1     = load_bitmap("nv_1.bmp", NULL);
        nv2     = load_bitmap("nv_2.bmp", NULL);
        nv3     = load_bitmap("nv_3.bmp", NULL);
        buffer  = create_bitmap(SCREEN_W, SCREEN_H);



        if (!nom_jeu || !map || !nv1 || !nv2 || !nv3 || !buffer) {
            allegro_message("Erreur : chargement des images.");                                                     //Blindage au cas où
            return 1;
        }


        intro_jeu();                                                                                                    //  On affiche l'introduction de BurgerMania



        SAMPLE* musiqueMenu = load_sample("musiqueMenu.wav");                                                   //Dès l'introduction du jeu, on joue une musique d'intro
        if (!musiqueMenu) {
            allegro_message("Erreur lors du chargement du fichier musiqueMenu.wav");
        } else {play_sample(musiqueMenu, 255, 128, 1000, TRUE);
        }




        saisir_pseudo(pseudo, map);                                                                                     //On appelle une fonction qui permet au joueur de s'enregistrer


        if (verification_pseudo(pseudo)) {                                                                              //On vérifie si le psuedo existe déjà ou non
            int choix = afficher_menu_joueur(buffer, map);


            if (choix == 0) {
                textout_ex(screen, font, "Nouvelle partie commencée !", 500, 100, makecol(0,255,0), -1);  // Si non, on ne peut que commencer une nouvelle partie.

            } else {

                textout_ex(screen, font, "Reprise de votre partie !", 500, 100, makecol(0,255,0), -1);     //Si oui, on peut reprendre notre progression.
            }

        } else {


            FILE *fichier = fopen("joueurs.txt", "a");                                                                  //On ouvre un fichier txt pour noter les pseudos enregistrés.
            if (fichier) {
                fprintf(fichier, "%s\n", pseudo);
                fclose(fichier);
            }
            textout_ex(screen, font, "Bienvenue nouveau joueur, partie lancée !", 500, 100, makecol(0,255,0), -1);
        }



        rest(1000);                                                                                                 // Petite pause avant de passer à la sélection du niveau



        int quitter = 0;
        while (!quitter) {
            play_sample(musiqueMenu, 20, 128, 1000, TRUE);                                             // Remet la musique du menu

            int niveau = choisir_niveau(buffer, map);
            clear_keybuf();
            stop_sample(musiqueMenu);

            if (niveau == 0) {                                                                                          //Si le choix "Niveau1" (choix 0) est choisi, le niveau 1 se lance
                play_sample(musiqueNV1, 20, 128, 1000, TRUE);
                lancer_niveau1();

                stop_sample(musiqueNV1);
            }
            else if (niveau == 1) {                                                                                     //Si le choix "Niveau2" (choix 1) est choisi, le niveau 1 se lance
                play_sample(musiqueNV2, 20, 128, 1000, TRUE);
                lancer_niveau2();

                stop_sample(musiqueNV2);
            }
            else if (niveau == 2) {                                                                                     //Si le choix "Niveau3" (choix 2) est choisi, le niveau 1 se lance

                play_sample(musiqueNV3, 20, 128, 1000, TRUE);
                lancer_niveau3();
                stop_sample(musiqueNV3);
                rest(2000);
            }
            else {
                quitter = 1; // Cas par défaut si le joueur appuie sur une touche spéciale pour quitter
            }
        }



        destroy_sample(musiqueNV2);                                                                                     //Tous les Bitmaps se détruisent
        destroy_sample(musiqueNV1);
        destroy_sample(musiqueNV3);
        destroy_sample(musiqueMenu);
        destroy_bitmap(nom_jeu);
        destroy_bitmap(map);
        destroy_bitmap(nv1);
        destroy_bitmap(nv2);
        destroy_bitmap(nv3);
        destroy_bitmap(buffer);

        allegro_exit();
        return 0;
    }


END_OF_MAIN();





























 
 
 
 
 
 


 

 
 
 

 

 
 
 
 
 

 





