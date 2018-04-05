#include "Graphe.h"

Graphe::Graphe(std::string nom_fichier, std::string nom_decor)
{
    Setordre(0);
    SetNomGraphe(nom_fichier);

    Setdecor(load_bitmap(nom_decor.c_str(), NULL));
    if(!Getdecor())
    {
        allegro_message("pas pu trouver decor.png");
        exit(EXIT_FAILURE);
    }
}

Graphe::~Graphe()
{

}

void Graphe::Recuperation()
{
    int cd_x, cd_y, num, poids;
    std::string nom_img;

    std::string nom_fichier(GetNomGraphe());
    std::cout << nom_fichier << "jijij " << std::endl;

    int ordre;
    std::string nom_img_d, nom_img_a;
    std::vector<Arete*> tmp;
    Arete* a;
    Sommet* s;

    std::ifstream fichier(nom_fichier, std::ios::in);
    if (!fichier)
    {
        std::cout << nom_fichier << " ne peut pas etre ouvert!" << std::endl;
        return;
    }
    else
    {
        fichier >> ordre;
        if(ordre > 0)
        {
            for (int i(0); i<ordre; ++i)
            {
                fichier >> nom_img;
                fichier >> cd_x >> cd_y >> num >> poids;
                if(nom_img != "")
                {
                    s = new Sommet(cd_x, cd_y, num, poids, nom_img);
                    m_sall.push_back(s);
                }

            }
        }

        fichier >> m_ordre;
        if(m_ordre > 0)
        {
            for (int i(0); i<m_ordre; ++i)
            {
                fichier >> nom_img;
                fichier >> cd_x >> cd_y >> num >> poids;
                std::cout << "Nom : " << nom_img << " x " << cd_x << " poids " << poids << std::endl;
                s = new Sommet(cd_x, cd_y, num, poids, nom_img);
                ajouterSommet(s);
            }
        }

        fichier >> ordre;
        std::cout << "Ordre : " << ordre << std::endl;
        if(ordre > 0)
        {
            for (int k(0); k<ordre; ++k)
            {
                fichier >> nom_img_d >> nom_img_a >> poids;

                if(Getsommets().size() > 0 && nom_img_a != "")
                {
                    a = new Arete;

                    for(unsigned int i(0); i < Getsommets().size(); ++i)
                    {
                        if(Getsommets()[i]->getNomImg() == nom_img_d)
                        {
                            s = new Sommet;
                            s = Getsommets()[i];
                            a->Setdepart(s);
                        }

                        if(Getsommets()[i]->getNomImg() == nom_img_a)
                        {
                            s = new Sommet;
                            s = Getsommets()[i];
                            a->Setarrive(s);
                            a->Setpoids(poids);
                        }
                    }
                    tmp.push_back(a);
                }
            }
        }

    }
    Setaretes(tmp);
}

void Graphe::affichage(BITMAP* buffer, BITMAP* barre, int a)
{
    int xsDep = 0, ysDep = 0, xsArr = 0, ysArr = 0;
    int radius = 15;

    for (unsigned int i(0); i <Getaretes().size(); ++i)
    {
        xsDep = Getaretes()[i]->Getdepart()->GetCd_x() + Getaretes()[i]->Getdepart()->GetImg()->w/2;
        ysDep = Getaretes()[i]->Getdepart()->GetCd_y() + Getaretes()[i]->Getdepart()->GetImg()->h/2;
        xsArr = Getaretes()[i]->Getarrive()->GetCd_x() + Getaretes()[i]->Getarrive()->GetImg()->w/2;
        ysArr = Getaretes()[i]->Getarrive()->GetCd_y() + Getaretes()[i]->Getarrive()->GetImg()->h/2;

        line(buffer, xsDep, ysDep, xsArr, ysArr, makecol(255,0,0));
        circlefill(buffer, (xsDep + 3*xsArr)/4, (ysDep + 3*ysArr)/4, radius, makecol(255,0,0));
    }

    for (unsigned int i(0); i<Getsommets().size(); ++i)
    {
        blit(getSommet(i)->GetImg(), buffer, 0, 0, getSommet(i)->GetCd_x(), getSommet(i)->GetCd_y(), getSommet(i)->GetImg()->w, getSommet(i)->GetImg()->h);
        rectfill(buffer, getSommet(i)->GetCd_x() + getSommet(i)->GetImg()->w - 20, getSommet(i)->GetCd_y() + getSommet(i)->GetImg()->h - 15, getSommet(i)->GetCd_x() + getSommet(i)->GetImg()->w, getSommet(i)->GetCd_y() + getSommet(i)->GetImg()->h, makecol(0,0,0));
        rect(buffer, getSommet(i)->GetCd_x() + getSommet(i)->GetImg()->w - 20, getSommet(i)->GetCd_y() + getSommet(i)->GetImg()->h - 15, getSommet(i)->GetCd_x() + getSommet(i)->GetImg()->w-2, getSommet(i)->GetCd_y() + getSommet(i)->GetImg()->h-2, makecol(255,255,255));
        textprintf_ex(buffer, font, getSommet(i)->GetCd_x() + getSommet(i)->GetImg()->w - 16, getSommet(i)->GetCd_y() + getSommet(i)->GetImg()->h - 11, makecol(255,255,255), -1,"%d", getSommet(i)->GetNum());

    }

//    textprintf_ex(buffer, font, 400, 23, makecol(255,255,0), -1,"%d", Getaretes().size());
//    textprintf_ex(buffer, font, 440, 23, makecol(255,255,0), -1,"%d", mouse_y);

    outils(buffer, barre, a);

    draw_sprite(buffer, barre, 0, 600-barre->h);
    blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    clear_bitmap(buffer);
}

void Graphe::outils(BITMAP* buffer, BITMAP* barre, int a)
{
    ///Pour ajouter Arete
    rectfill(buffer, 745, 5, 795, 55, makecol(0,250,0));

    ///Pour supprimer Sommet
    rectfill(buffer, 745, 65, 795, 115, makecol(255, 0, 0));

    ///Pour ajouter Sommet
    rectfill(buffer, 745, 125, 795, 175, makecol(255,0,255));

    ///Pour supprimer Arete
    rectfill(buffer, 745, 185, 795, 235, makecol(255, 255, 0));

    if(a != 1)
    {
        if (is_mouse(745, 50, 5, 50))
        {
            rectfill(buffer, 743, 3, 797, 57, makecol(0,250,0));

            if(mouse_b&1)
                ajouterArete(buffer);
        }

        if (is_mouse(745, 50, 65, 50))
        {
            rectfill(buffer, 743, 63, 797, 117, makecol(255,0,0));

            if(mouse_b&1)
            {
                suppSommet();
            }
        }

        if (is_mouse(745, 50, 125, 50))
        {
            rectfill(buffer, 743, 123, 797, 177, makecol(255,0,255));

            if(mouse_b&1)
            {
                ajouSommet(buffer, barre);
            }
        }

        if (is_mouse(745, 50, 185, 50))
        {
            rectfill(buffer, 743, 183, 797, 237, makecol(255,255,0));
            if(mouse_b&1)
            {
                suppArete();

            }
        }
    }

}

void Graphe::ajouterArete(BITMAP* buffer)
{
    Arete* a;
    Sommet* s;

    int prev_mouse_b;
    int now_mouse_b;

    a= new Arete;
    s= new Sommet;

    std::vector<Arete*> tmp;

    tmp = Getaretes();

    while(s->getNomImg() == "")
    {
        prev_mouse_b = now_mouse_b;
        now_mouse_b = mouse_b&1;
        for (unsigned int i(0); i<Getsommets().size(); ++i)
        {
            if(is_sommmet(i))
            {
                if (!prev_mouse_b && now_mouse_b)
                {
                    s = Getsommets()[i];
                    a->Setdepart(s);
                }
            }
        }
    }

    rect(screen, a->Getdepart()->GetCd_x()-2, a->Getdepart()->GetCd_y()-2, a->Getdepart()->GetImg()->w +a->Getdepart()->GetCd_x()+1,a->Getdepart()->GetImg()->h + a->Getdepart()->GetCd_y()+1, makecol(255,0,0));

    s= new Sommet;

    while(s->getNomImg() == "")
    {
        prev_mouse_b = now_mouse_b;
        now_mouse_b = mouse_b&1;
        for (unsigned int i(0); i<Getsommets().size(); ++i)
        {
            if(is_sommmet(i))
            {
                if (!prev_mouse_b && now_mouse_b)
                {
                    s = Getsommets()[i];
                    a->Setarrive(s);
                }
            }
        }
    }

    tmp.push_back(a);

    Setaretes(tmp);
    std::cout << "ajou reussi" << std::endl;
}

void Graphe::ajouSommet(BITMAP* buffer, BITMAP* barre)
{
    Sommet* s;
    int prev_mouse_b;
    int now_mouse_b;

    std::vector<Sommet*> som(Getsall());
    std::vector<Sommet*> som_graph(Getsommets());

    if(som.size() > 0)
    {
        while(som.size() +1 != Getsall().size())
        {
            prev_mouse_b = now_mouse_b;
            now_mouse_b = mouse_b&1;

            for(unsigned int i(0); i < som.size(); ++i)
            {
                if(som.size() > 0)
                {
                    stretch_blit(som[i]->GetImg(), buffer, 0, 0, som[i]->GetImg()->w, som[i]->GetImg()->h, 680, 60*i+5, som[i]->GetImg()->w/2, som[i]->GetImg()->h/2);
                    blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
                }
            }

            for(unsigned int i(0); i < som.size(); ++i)
            {
                if(is_mouse(680, som[i]->GetImg()->w/2, 60*i+5, som[i]->GetImg()->h/2))
                {
                    if (!prev_mouse_b && now_mouse_b)
                    {
                        s = new Sommet;
                        s = som[i];
                        som.erase(som.begin()+i);
                        som_graph.push_back(s);
                    }
                }
            }
        }
    }

    Setsall(som);
    Setsommets(som_graph);
}

void Graphe::suppSommet()
{
    int prev_mouse_b;
    int now_mouse_b;

    std::vector<Sommet*> tmp(Getsommets());
    std::vector<Sommet*> tmp_supp(Getsall());
    std::vector<Arete*> temp(Getaretes());

    bool stop(false);

    if(tmp.size() > 0)
    {
        while(tmp.size()+1 != Getsommets().size())
        {
            prev_mouse_b = now_mouse_b;
            now_mouse_b = mouse_b&1;

            for(unsigned int i(0); i < tmp.size() ; ++i)
            {
                if(i == 0)
                   stop = false;

                if(is_sommmet(i))
                {
                    if (!prev_mouse_b && now_mouse_b && !stop)
                    {
                        for(unsigned int j(0); j < temp.size(); ++j)
                        {
                            if(tmp[i] == temp[j]->Getdepart() || tmp[i] == temp[j]->Getarrive())
                            {
                                temp.erase(temp.begin() + j);
                                --j;
                            }
                        }

                        tmp_supp.push_back(tmp[i]);
                        tmp.erase(tmp.begin()+i);
                        stop = true;
                    }
                }
            }
        }
    }

    Setsommets(tmp);
    Setsall(tmp_supp);
    Setaretes(temp);
}

void Graphe::suppArete()
{
    Sommet* s1 = new Sommet;
    std::vector<Arete*> temp = Getaretes();
    int prev_mouse_b = 0;
    int now_mouse_b = mouse_b&1;
    bool stop(false);

    if (temp.size() > 0)
    {
        /// Tant qu'on n'a pas choisi d'image
        while (s1->getNomImg() == "")
        {
            prev_mouse_b = now_mouse_b;
            now_mouse_b = mouse_b&1;
            for (unsigned int i(0); i<Getsommets().size(); ++i)
            {
                /// Si on est sur le sommet et que l'on clic gauche
                if (is_sommmet(i) && !prev_mouse_b && now_mouse_b)
                {
                    for (unsigned int j(0); j<temp.size(); ++j)
                    {
                        /// Si le sommet clické i appartient à au moins une arête en tant que sommet de départ
                        if (temp[j]->Getdepart() == Getsommets()[i])
                        {
                            s1 = Getsommets()[i];
                        }
                    }
                }
            }
        }

        rect(screen, s1->GetCd_x()-2, s1->GetCd_y()-2, s1->GetCd_x() + s1->GetImg()->w+1, s1->GetCd_y() + s1->GetImg()->h+1, makecol(255,12,12));
        rect(screen, s1->GetCd_x()-1, s1->GetCd_y()-1, s1->GetCd_x() + s1->GetImg()->w, s1->GetCd_y() + s1->GetImg()->h, makecol(255,12,12));

        while(temp.size() == Getaretes().size())
        {
            prev_mouse_b = now_mouse_b;
            now_mouse_b = mouse_b&1;
            for (unsigned int i(0); i<Getsommets().size(); ++i)
            {
                if (is_sommmet(i) && !prev_mouse_b && now_mouse_b)
                {
                    for (unsigned int j(0); j<temp.size(); ++j)
                    {
                        /// Si le sommet clické i appartient à la même arète que le sommet de départ
                        if (temp[j]->Getarrive() == Getsommets()[i] && temp[j]->Getdepart() == s1 && !stop)
                        {
                            stop = true;
                            temp.erase(temp.begin() + j);
                            --j;
                        }
                    }
                }
            }
        }
    }

    Setaretes(temp);
}

void Graphe::update(BITMAP* buffer, BITMAP* barre)
{
    int prev_mouse_b = 0;
    int now_mouse_b = mouse_b&1;

    prev_mouse_b = now_mouse_b;
    now_mouse_b = mouse_b&1;

    blit(Getdecor(), buffer, 0,0,0,0,Getdecor()->w, Getdecor()->h);

    for(int i(Getsommets().size()-1); i >= 0 ; --i)
    {
        if(is_sommmet(i))
        {
            if(mouse_b&1)
            {
                while(prev_mouse_b && now_mouse_b)
                {
                    prev_mouse_b = now_mouse_b;
                    now_mouse_b = mouse_b&1;

                    blit(Getdecor(), buffer, 0,0,0,0,Getdecor()->w, Getdecor()->h);

                    rectfill(buffer, Getsommets()[i]->GetCd_x()-2, Getsommets()[i]->GetCd_y()-2, Getsommets()[i]->GetImg()->w +Getsommets()[i]->GetCd_x()+1,Getsommets()[i]->GetImg()->h + Getsommets()[i]->GetCd_y()+1, makecol(0,255,0));

                    Getsommets()[i]->SetCd_x(mouse_x-Getsommets()[i]->GetImg()->w/2);
                    Getsommets()[i]->SetCd_y(mouse_y-Getsommets()[i]->GetImg()->h/2);

                    if (Getsommets()[i]->GetCd_x() + Getsommets()[i]->GetImg()->w > 740)
                        Getsommets()[i]->SetCd_x(740 - Getsommets()[i]->GetImg()->w);

                    if (Getsommets()[i]->GetCd_y() + Getsommets()[i]->GetImg()->h > SCREEN_H-barre->h)
                        Getsommets()[i]->SetCd_y(SCREEN_H - barre->h - Getsommets()[i]->GetImg()->h);

                    if (Getsommets()[i]->GetCd_x() < 0)
                        Getsommets()[i]->SetCd_x(0);

                    if (Getsommets()[i]->GetCd_y() < 0)
                        Getsommets()[i]->SetCd_y(0);

                    affichage(buffer, barre, 1);
                }
            }
        }
    }

}

void Graphe::save()
{
    int ordre = Getsall().size();
    std::string nom_fichier(GetNomGraphe());

    std::ofstream fichier(nom_fichier, std::ios::out);
    if (!fichier)
    {
        std::cout << nom_fichier << " ne peut pas etre ouvert!" << std::endl;
        return;
    }
    else
    {
        fichier << ordre << std::endl << std::endl;
        if(ordre > 0)
        {
           for(int i = 0; i < ordre; i++)
            {
                fichier << Getsall()[i]->getNomImg() << std::endl;
                fichier << Getsall()[i]->GetCd_x() << " " << Getsall()[i]->GetCd_y() << " " << Getsall()[i]->GetNum() << " " <<Getsall()[i]->GetPoids() << std::endl;
                fichier << std::endl;
            }
        }


        ordre = Getsommets().size();
        fichier << ordre << std::endl << std::endl;
        if(ordre > 0)
        {
            for(int i = 0; i < ordre; i++)
            {
                fichier << getSommet(i)->getNomImg() << std::endl;
                fichier << getSommet(i)->GetCd_x() << " " << getSommet(i)->GetCd_y() << " " << getSommet(i)->GetNum() << " " <<getSommet(i)->GetPoids() << std::endl;
                fichier << std::endl;
            }
        }

        ordre = Getaretes().size();

        fichier << ordre << std::endl;

        if(ordre > 0)
        {
            for(int i = 0; i < ordre; i++)
            {
                fichier << Getaretes()[i]->Getdepart()->getNomImg() << " "<< Getaretes()[i]->Getarrive()->getNomImg() << " "<< Getaretes()[i]->Getpoids() << std::endl;
            }
        }
    }
}

bool Graphe::is_mouse(int x, int weight, int y, int height)
{
    return     mouse_x >= x && mouse_x <= x + weight  &&  mouse_y >= y && mouse_y <= y + height;
}

bool Graphe::is_sommmet(int i)
{
    return     mouse_x >= Getsommets()[i]->GetCd_x() && mouse_x <= Getsommets()[i]->GetCd_x() + Getsommets()[i]->GetImg()->w
               &&  mouse_y >= Getsommets()[i]->GetCd_y() && mouse_y <= Getsommets()[i]->GetCd_y() + Getsommets()[i]->GetImg()->h;
}
