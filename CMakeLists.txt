cmake_minimum_required(VERSION 3.26)
project(gestion_menu C)

set(CMAKE_C_STANDARD 99)

# Ajoute TOUS les fichiers sources nécessaires à la compilation :
add_executable(gestion_menu
        main.c
        intro_jeu.c
        init.c
        menu.c
        nv2.c
        nv2.h
        nv1.c
        nv1.h
        COMMON.c
        COMMON.h
        nv3.c
        nv3.h
)

# Lien avec la bibliothèque Allegro
target_link_libraries(gestion_menu mingw32 alleg44)
