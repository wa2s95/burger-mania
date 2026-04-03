#include "init.h"


void initialisation_allegro() {                                                                                         //On initialise les composants externes de l'ordinateur
    allegro_init();
    install_timer();
    install_keyboard();
    install_sound(DIGI_AUTODETECT, MIDI_NONE, NULL);                                                            //On initialise le son pour pouvoir jouer des musiques
    install_mouse();
    set_color_depth(desktop_color_depth());                                                                             //Aisni que la game de couleurs


    if (set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 1920, 1080, 0, 0) != 0) {                             //On paramètre la résolution d'affichage
        allegro_message("Erreur : impossible de passer en mode plein écran 1920x1080.");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
}