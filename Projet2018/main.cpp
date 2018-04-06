#include "Graphe.h"

int main()
{
    std::cout << "Hello world!" << std::endl;

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
    barre = load_bitmap("Graphe1/Images/tabgraph.bmp", NULL);
    if(!barre)
    {
        allegro_message("pas pu trouver barre.png");
        exit(EXIT_FAILURE);
    }

    std::vector<Graphe*> g;
    int num(0);

    int prev_mouse_b = 0;
    int now_mouse_b = mouse_b&1;

    Graphe* g1;
    g1 = new Graphe("Graphe1/Sommets.txt", "Graphe1/Images/Blue.png");
std::cout << "LLZ";
    Graphe* g2;
    g2 = new Graphe("Graphe2/Sommets.txt", "Graphe2/Images/Yellow.png");

    Graphe* g3;
    g3 = new Graphe("Graphe3/Sommets.txt", "Graphe3/Images/Green.png");

    show_mouse(screen);

    g.push_back(g1);
    g.push_back(g2);
    g.push_back(g3);

    for(unsigned int i(0); i < g.size(); ++i)
        g[i]->recuperation();

    while (!key[KEY_ESC])
    {
        g[num]->slider();
        g[num]->update(buffer, barre);
        g[num]->affichage(buffer, barre, 0);

        for(int i(0); i < 3; ++i)
        {
            if(mouse_x >= i*283 && mouse_x <= i*283 + 234  &&  mouse_y >= 547 && mouse_y <= 547 + 51)
            {
                prev_mouse_b = now_mouse_b;
                now_mouse_b = mouse_b&1;

                if(!prev_mouse_b && now_mouse_b)
                {
                    num = i;
                }
            }
        }
    }

    for(unsigned int i(0); i < g.size(); ++i) g[i]->save();

    destroy_bitmap(buffer);

    return 0;
}
END_OF_MAIN();
