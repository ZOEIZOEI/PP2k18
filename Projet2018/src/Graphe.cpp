#include "Graphe.h"

Graphe::Graphe(std::string nom_fichier, std::string nom_decor)
{
    Setordre(0);
    SetNomGraphe(nom_fichier);
    BITMAP* bouton;

    Setdecor(load_bitmap(nom_decor.c_str(), NULL));
    if(!Getdecor())
    {
        allegro_message("pas pu trouver decor.png");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i<4; ++i)
    {
        if (i == 0) bouton = load_bitmap("Graphe1/Images/addA.png", NULL);
        if (i == 1) bouton = load_bitmap("Graphe1/Images/suppS.png", NULL);
        if (i == 2) bouton = load_bitmap("Graphe1/Images/addS.png", NULL);
        if (i == 3) bouton = load_bitmap("Graphe1/Images/suppA.png", NULL);

        ajouterBouton(bouton);
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

    std::vector<std::vector<int> > adjacence;

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

        ///Matrice temp d'adjacence
        m_adjacence = new std::list<int>[Getordre()+ordre];

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

                    if(a->Getdepart() && a->Getarrive())
                    {
                        int dep(a->Getdepart()->GetNum());
                        int arr(a->Getarrive()->GetNum());
                        ajoutadjacen(dep, arr);
                        std::cout << "Ordre de la list : " << Getordre() + ordre << std::endl;
                    }

                    tmp.push_back(a);
                }
            }
        }

    }
    Setaretes(tmp);


    std::cout << "***" << std::endl;
    if(!Getadjacence().empty())
    {
        for(std::list<int>::iterator it=Getadjacence().begin(); it!=Getadjacence().end(); ++it)
        {
            std::cout << *it << " ";
        }

    }
    else
    {
        std::cout << "La liste est vide" << std::endl;
    }
    std::cout << "***" << std::endl;

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
    if(a != 1)
    {
        rectfill(buffer, 745, 245, 795, 295, makecol(123,123,0));

        if (is_mouse(745, 50, 5, 50))
        {
            rectfill(buffer, 743, 3, 797, 57, makecol(76,201,0));

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
            rectfill(buffer, 743, 123, 797, 177, makecol(6,201,0));

            if(mouse_b&1)
            {
                ajouSommet(buffer, barre);
            }
        }

        if (is_mouse(745, 50, 185, 50))
        {
            rectfill(buffer, 743, 183, 797, 237, makecol(255,0,0));
            if(mouse_b&1)
            {
                suppArete();

            }
        }

        if (is_mouse(745, 50, 245, 50))
        {
            rectfill(buffer, 743, 243, 797, 297, makecol(123,123,0));

            if(mouse_b&1)
            {
                CFC();

            }
        }
    }

    ///Pour ajouter, supprimer Arêtes/Sommets
    for (unsigned int i = 0; i<m_boutons.size(); ++i)
    {
        blit(getBouton(i), buffer, 0, 0, 745, 5+(60*i), getBouton(i)->w, getBouton(i)->h);
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

                    blit(Getdecor(), buffer,0,0,0,0,Getdecor()->w, Getdecor()->h);

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

void Graphe::ComposanteRecursif(int u, int disc[], int low[], std::stack<int> *st, bool stackMember[])
{
    // A static variable is used for simplicity, we can avoid use of static variable by passing a pointer.
    static int time = 0;

    // Initialize discovery time and low value
    disc[u] = low[u] = ++time;
    st->push(u);
    stackMember[u] = true;

    // Go through all vertices adjacent to this
    std::list<int>::iterator i;
    for (i = m_adjacence[u].begin(); i != m_adjacence[u].end(); ++i)
    {
        int v = *i;  // v is current adjacent of 'u'

        // If v is not visited yet, then recur for it
        if (disc[v] == -1)
        {
            ComposanteRecursif(v, disc, low, st, stackMember);

            // Check if the subtree rooted with 'v' has a
            // connection to one of the ancestors of 'u'
            // Case 1 (per above discussion on Disc and Low value)
            low[u]  = std::min(low[u], low[v]);
        }

        // Update low value of 'u' only of 'v' is still in stack
        // (i.e. it's a back edge, not cross edge).
        // Case 2 (per above discussion on Disc and Low value)
        else if (stackMember[v] == true)
            low[u]  = std::min(low[u], disc[v]);
    }

    // head node found, pop the stack and print an CFC
    int w = 0;  // To store stack extracted vertices
    if (low[u] == disc[u])
    {
        while (st->top() != u)
        {
            w = (int) st->top();
//            std::cout << w << " ";
            std::cout << Getsommets()[w]->GetNum() << " ";
            stackMember[w] = false;
            st->pop();
        }
        w = (int) st->top();
        std::cout << Getsommets()[w]->GetNum() << "\n";
        stackMember[w] = false;
        st->pop();
    }

}

void Graphe::CFC()
{
    int *disc = new int[m_ordre];
    int *low = new int[m_ordre];
    bool *stackMember = new bool[m_ordre];
    std::stack<int> *st = new std::stack<int>();

    // Initialize disc and low, and stackMember arrays
    for (int i = 0; i < m_ordre; i++)
    {
        disc[i] = -1;
        low[i] = -1;
        stackMember[i] = false;
    }

    // Call the recursive helper function to find strongly
    // connected components in DFS tree with vertex 'i'
    for (int i = 0; i < m_ordre; i++)
        if (disc[i] == -1)
            ComposanteRecursif(i, disc, low, st, stackMember);
}
