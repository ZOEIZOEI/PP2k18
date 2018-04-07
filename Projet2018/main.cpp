#include "Graphe.h"

void Menu(BITMAP* buffer, std::vector<BITMAP*> menu, bool* exit, int* num);

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

    BITMAP* menu = load_bitmap("Menu/Menu.png", NULL);
    if(!menu)
    {
        allegro_message("pas pu trouver menu.png");
        exit(EXIT_FAILURE);
    }

    BITMAP* menuOcean = load_bitmap("Menu/menuOcean.png", NULL);
    if(!menuOcean)
    {
        allegro_message("pas pu trouver menuOcean.png");
        exit(EXIT_FAILURE);
    }

    BITMAP* menuSavane = load_bitmap("Menu/menuSavane.png", NULL);
    if(!menuSavane)
    {
        allegro_message("pas pu trouver menuSavane.png");
        exit(EXIT_FAILURE);
    }

    BITMAP* menuHerbe = load_bitmap("Menu/menuHerbe.png", NULL);
    if(!menuHerbe)
    {
        allegro_message("pas pu trouver menuHerbe.png");
        exit(EXIT_FAILURE);
    }

    BITMAP* fin = load_bitmap("Menu/exit.png", NULL);
    if(!fin)
    {
        allegro_message("pas pu trouver exit.png");
        exit(EXIT_FAILURE);
    }


    std::vector<Graphe*> g;
    std::vector<BITMAP*> Image_Menu;
    Image_Menu.push_back(menu);
    Image_Menu.push_back(menuOcean);
    Image_Menu.push_back(menuSavane);
    Image_Menu.push_back(menuHerbe);
    Image_Menu.push_back(fin);

    int num(4);

    int prev_mouse_b = 0;
    int now_mouse_b = mouse_b&1;

    bool exit(false);

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

    while(!exit)
    {
        Menu(buffer, Image_Menu, &exit, &num);

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

void Menu(BITMAP* buffer, std::vector<BITMAP*> Img_Menu, bool* exit, int* num)
{

    blit(Img_Menu[0], buffer, 0,0,0,0,SCREEN_W, SCREEN_H);
    blit(Img_Menu[4], buffer,0, 0, 738, 537, Img_Menu[4]->w, Img_Menu[4]->h);
    textprintf_ex(buffer, font, 400, 23, makecol(255,255,0), -1,"%d", mouse_x);



    if(mouse_x > 738 && mouse_x < Img_Menu[4]->w+738 && mouse_y > 537 && mouse_y < Img_Menu[4]->h+537)
    {
        if(mouse_b&1)
            *exit=true;
    }

    ///Graphe Ocean
    rectfill(buffer, 27, 328, 30+Img_Menu[1]->w+3, 331+Img_Menu[1]->h+3, makecol(255,255,255));
    blit(Img_Menu[1], buffer, 0,0, 30,331, Img_Menu[1]->w, Img_Menu[1]->h);

    if(is_mouse(44, 192 ,330, 162))
    {
        if(mouse_b&1)
        {
            *num = 0;
        }
    }

    ///Image Savane
    rectfill(buffer, 288, 327, 291+Img_Menu[2]->w+3, 327+Img_Menu[2]->h+6, makecol(255,255,255));
    blit(Img_Menu[2], buffer, 0,0, 291,330, Img_Menu[2]->w, Img_Menu[2]->h);

    if(is_mouse(291, 190,330, 162))
    {
        if(mouse_b&1)
        {
            *num = 1;
        }
    }

    ///Image Herbe
    rectfill(buffer, 557, 327, 560+Img_Menu[3]->w+3, 330+Img_Menu[3]->h+3, makecol(255,255,255));
    blit(Img_Menu[3], buffer, 0,0, 560 ,330, Img_Menu[3]->w, Img_Menu[3]->h);

    if(is_mouse(560, 190,330, 162))
    {
        if(mouse_b&1)
        {
            *num = 2;
        }
    }

    blit(buffer, screen, 0,0,0,0,SCREEN_W, SCREEN_H);
}
