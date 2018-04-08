#include "Graphe.h"

void Menu(BITMAP* buffer, std::vector<BITMAP*> menu, bool* exit, int* num, int* idx);

int main()
{
    std::cout << "Hello world!" << std::endl;

    srand(time(NULL));
    set_uformat(U_ASCII);
    allegro_init();
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
    BITMAP* barre = load_bitmap("Graphe1/Images/tabgraph.bmp", NULL);
    if(!barre)
    {
        allegro_message("pas pu trouver barre.png");
        exit(EXIT_FAILURE);
    }

    BITMAP* menu = load_bitmap("Menu/Menu.png", NULL);
    if(!menu)
    {
        allegro_message("pas pu trouver menu.png");
        exit(EXIT_FAILURE);
    }

    BITMAP* aide = load_bitmap("Menu/Aide.png", NULL);
    if(!aide)
    {
        allegro_message("pas pu trouver aide.png");
        exit(EXIT_FAILURE);
    }

    std::vector<Graphe*> g;
    std::vector<BITMAP*> Image_Menu;
    Image_Menu.push_back(menu);
    Image_Menu.push_back(aide);

    int num(4);
    int idx(0);

    int prev_mouse_b = 0;
    int now_mouse_b = mouse_b&1;

    bool exit(false);

    Graphe* g1 = new Graphe("Graphe1/Sommets.txt", "Graphe1/Images/Blue.png");
    Graphe* g2 = new Graphe("Graphe2/Sommets.txt", "Graphe2/Images/Yellow.png");
    Graphe* g3 = new Graphe("Graphe3/Sommets.txt", "Graphe3/Images/Green.png");

    show_mouse(screen);

    g.push_back(g1);
    g.push_back(g2);
    g.push_back(g3);

    for(unsigned int i(0); i < g.size(); ++i)  g[i]->recuperation();

    while(!exit)
    {
        Menu(buffer, Image_Menu, &exit, &num, &idx);

        if(num < 3)
        {
            while (!key[KEY_ESC])
            {
                prev_mouse_b = now_mouse_b;
                now_mouse_b = mouse_b&1;

                g[num]->slider();
                g[num]->update(buffer, barre, prev_mouse_b, now_mouse_b);
                g[num]->affichage(buffer, barre, 0, prev_mouse_b, now_mouse_b);

                for(int i(0); i < 3; ++i)
                {
                    if(mouse_x >= i*283 && mouse_x <= i*283 + 234  &&  mouse_y >= 547 && mouse_y <= 547 + 51)
                    {
                        if(!prev_mouse_b && now_mouse_b) num = i;
                    }
                }
                g[num]->augmentationTime();
            }
            num = 4;
        }
    }


    for(unsigned int i(0); i < g.size(); ++i) g[i]->save();
    destroy_bitmap(buffer);
    return 0;
}
END_OF_MAIN();

void Menu(BITMAP* buffer, std::vector<BITMAP*> Img_Menu, bool* exit, int* num, int* idx)
{

    blit(Img_Menu[*idx], buffer, 0,0,0,0,SCREEN_W, SCREEN_H);
    textprintf_ex(buffer, font, 400, 23, makecol(255,255,0), -1,"%d", mouse_x);

    ///Bouton Exit
    if(mouse_x > 714 && mouse_x < 793 && mouse_y > 540 && mouse_y < 590 && *idx == 0)
    {
        for(int i(710); i<797;++i )
        {
            for(int j(536); j < 594; ++j)
            {
                if(getpixel(buffer, i, j) != makecol(255,255,255))
                {
                     putpixel(buffer, i, j, makecol(34,177,76));
                }
            }
        }

        if(mouse_b&1)
            *exit=true;
    }

    ///Bouton Aide
    if(mouse_x > 20 && mouse_x < 141 && mouse_y > 531 && mouse_y < 587 && *idx == 0)
    {
        for(int i(20); i<141;++i )
        {
            for(int j(531); j < 587; ++j)
            {
                if(getpixel(buffer, i, j) != makecol(255,255,255))
                {
                     putpixel(buffer, i, j, makecol(34,177,76));
                }
            }
        }

        if(mouse_b&1)
        {
            *idx = 1;
        }
    }

    ///Bouton Retour
    if(mouse_x > 355 && mouse_x < 453 && mouse_y > 536 && mouse_y < 593 && *idx == 1)
    {
        if(mouse_b&1)
        {
            *idx = 0;
        }
    }

    ///Bouton Edit
    if(mouse_x > 219 && mouse_x < 588 && mouse_y > 366 && mouse_y < 478 && *idx == 0)
    {
        for(int i(214); i<593;++i )
        {
            for(int j(364); j < 482; ++j)
            {
                if(getpixel(buffer, i, j) != makecol(255,255,255))
                {
                     putpixel(buffer, i, j, makecol(34,177,76));
                }
            }
        }

        if(mouse_b&1)
        {
            *idx = 0;
            *num = 0;
        }
    }

    blit(buffer, screen, 0,0,0,0,SCREEN_W, SCREEN_H);
}
