

#include "Graphe.h"

int main()
{
    srand(time(NULL));
    set_uformat(U_ASCII);
    allegro_init();
    install_timer();
    install_keyboard();
    install_mouse();

    register_png_file_type();
    jpgalleg_init();
    set_color_depth(desktop_color_depth());
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED,800,600,0,0)!=0)
    {
        allegro_message("Probleme mode GFX");
        allegro_exit();

        exit(EXIT_FAILURE);
    }

    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);

    BITMAP* barre;
    barre = load_bitmap("Graphe1/Images/tabgraph.png", NULL);
    if(!barre)
    {
        allegro_message("pas pu trouver barre.png");
        exit(EXIT_FAILURE);
    }

    //Graphe g1("Graphe1/Sommets.txt");
    //Graphe g2("Graphe2/Sommets.txt");
    Graphe g3("Graphe3/Sommets.txt");
    show_mouse(screen);

    while (!key[KEY_ESC])
    {
        g3.update(buffer, barre);

        g3.affichage(buffer, barre);
    }

   // g1.save("Graphe1/Sommets.txt");
   // g2.save("Graphe2/Sommets.txt");
    g3.save("Graphe3/Sommets.txt");

    destroy_bitmap(buffer);

    return 0;
}
END_OF_MAIN();

