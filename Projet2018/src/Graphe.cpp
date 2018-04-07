#include "Graphe.h"

Graphe::Graphe(std::string nom_fichier, std::string nom_decor)
{
    setOrdre(0);
    setNomGraphe(nom_fichier);
    BITMAP* bouton;

    setDecor(load_bitmap(nom_decor.c_str(), NULL));
    if (!getDecor())
    {
        allegro_message("pas pu trouver decor.png");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i<8; ++i)
    {
        if (i == 0) bouton = load_bitmap("Graphe1/Images/addA.png", NULL);
        if (i == 1) bouton = load_bitmap("Graphe1/Images/suppS.png", NULL);
        if (i == 2) bouton = load_bitmap("Graphe1/Images/addS.png", NULL);
        if (i == 3) bouton = load_bitmap("Graphe1/Images/suppA.png", NULL);
        if (i == 4) bouton = load_bitmap("Graphe1/Images/CFC.png", NULL);
        if (i == 5) bouton = load_bitmap("Graphe1/Images/calcK.png", NULL);
        if (i == 6) bouton = load_bitmap("Graphe1/Images/play.png", NULL);
        if (i == 7) bouton = load_bitmap("Graphe1/Images/pause.png", NULL);
        ajouterBouton(bouton);
    }
}

Graphe::~Graphe()
{

}

void Graphe::recuperation()
{
    int cd_x, cd_y, num, poids, nb_aretes;
    std::string nom_img, nom_img_d, nom_img_a;

    std::string nom_fichier(getNomGraphe());
    std::cout << " Chargement de : " << nom_fichier << std::endl;

    std::vector<Arete*> tmp;
//    std::pair<int, int> pair_temp;

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
        fichier >> m_nb_s_sup;
        if (m_nb_s_sup > 0)
        {
            for (int i(0); i<m_nb_s_sup; ++i)
            {
                fichier >> nom_img;
                fichier >> cd_x >> cd_y >> num >> poids;
                if (nom_img != "")
                {
                    s = new Sommet(cd_x, cd_y, num, poids, nom_img);
                    m_s_sup.push_back(s);
                }
            }
        }

        fichier >> m_ordre;
        m_adjacences = new std::list<int>[m_ordre + m_nb_s_sup];
        if (m_ordre > 0)
        {
            for (int i(0); i<m_ordre; ++i)
            {
                fichier >> nom_img;
                fichier >> cd_x >> cd_y >> num >> poids;
//                std::cout << "Nom : " << nom_img << " x " << cd_x << " poids " << poids << std::endl;
                s = new Sommet(cd_x, cd_y, num, poids, nom_img);
                m_sommets.push_back(s);
            }
        }

        fichier >> nb_aretes;
        std::cout << "Nombre d'aretes du graphe : " << nb_aretes << std::endl << std::endl;

        if (nb_aretes > 0)
        {
            for (int k(0); k < nb_aretes; ++k)
            {
                fichier >> nom_img_d >> nom_img_a >> poids;

                if (getSommets().size() > 0 && nom_img_a != "")
                {
                    a = new Arete;

                    for (unsigned int i(0); i < getSommets().size(); ++i)
                    {
                        if (getSommets()[i]->getNomImg() == nom_img_d)
                        {
                            s = new Sommet;
                            s = getSommets()[i];
                            a->setDepart(s);
                        }

                        if (getSommets()[i]->getNomImg() == nom_img_a)
                        {
                            s = new Sommet;
                            s = getSommets()[i];
                            a->setArrive(s);
                            a->setPoids(poids);
                        }
                    }
                    tmp.push_back(a);
                    std::cout << "[" << a->getDepart()->getNum() << "] -> [" << a->getArrive()->getNum();
                    std::cout << "] POIDS : " << a->getPoids() << std::endl;
                    m_adjacences[a->getDepart()->getNum()].push_back(a->getArrive()->getNum());

                    /// Ajout des sommets dans les vecteurs d'adjacences des sommets respectifs (simple connexité)
                    m_adj_adj[a->getDepart()->getNum()].push_back(a->getArrive()->getNum());
                    m_adj_adj[a->getArrive()->getNum()].push_back(a->getDepart()->getNum());
//                    pair_temp.first = a->getDepart()->getNum();
//                    pair_temp.second = a->getArrive()->getNum();
//                    getSommet(a->getDepart()->getNum())->ajouterAdjS(pair_temp);
//
//                    pair_temp.first = a->getArrive()->getNum();
//                    pair_tem.second = a->getDepart()->getNum();
//                    getSommet(a->getArrive()->getNum())->ajouterAdjS(pair_temp);
                }
            }
        }
    }
    setAretes(tmp);
}

void Graphe::affichage(BITMAP* buffer, BITMAP* barre, int a, int prev_mouse_b, int now_mouse_b)
{
    int xsDep = 0, ysDep = 0, xsArr = 0, ysArr = 0;
    int radius = 15;

    sliderArete();
    for (unsigned int i(0); i <getAretes().size(); ++i)
    {
        xsDep = getAretes()[i]->getDepart()->getCd_x() + getAretes()[i]->getDepart()->getImg()->w/2;
        ysDep = getAretes()[i]->getDepart()->getCd_y() + getAretes()[i]->getDepart()->getImg()->h/2;
        xsArr = getAretes()[i]->getArrive()->getCd_x() + getAretes()[i]->getArrive()->getImg()->w/2;
        ysArr = getAretes()[i]->getArrive()->getCd_y() + getAretes()[i]->getArrive()->getImg()->h/2;

        line(buffer, xsDep, ysDep, xsArr, ysArr ,makecol(255,0,0));
        circlefill(buffer, (xsDep + 3*xsArr)/4, (ysDep + 3*ysArr)/4, radius, makecol(255,0,0));
        textprintf_ex(buffer, font, (xsDep + 3*xsArr)/4 - 6, (ysDep + 3*ysArr)/4 - 6, makecol(0,0,0), -1, "%d", getAretes()[i]->getPoids());
    }

    for (unsigned int i(0); i<getSommets().size(); ++i)
    {
        ///Image
        blit(getSommet(i)->getImg(), buffer, 0, 0, getSommet(i)->getCd_x(), getSommet(i)->getCd_y(), getSommet(i)->getImg()->w, getSommet(i)->getImg()->h);
        ///Numero Image
        rectfill(buffer, getSommet(i)->getCd_x() + getSommet(i)->getImg()->w - 20, getSommet(i)->getCd_y() + getSommet(i)->getImg()->h - 15, getSommet(i)->getCd_x() + getSommet(i)->getImg()->w, getSommet(i)->getCd_y() + getSommet(i)->getImg()->h, makecol(0,0,0));
        rect(buffer, getSommet(i)->getCd_x() + getSommet(i)->getImg()->w - 20, getSommet(i)->getCd_y() + getSommet(i)->getImg()->h - 15, getSommet(i)->getCd_x() + getSommet(i)->getImg()->w, getSommet(i)->getCd_y() + getSommet(i)->getImg()->h-2, makecol(255,255,255));
        textprintf_ex(buffer, font, getSommet(i)->getCd_x() + getSommet(i)->getImg()->w - 16, getSommet(i)->getCd_y() + getSommet(i)->getImg()->h - 11, makecol(255,255,255), -1,"%d", getSommet(i)->getNum());
        ///Poids Image
        rectfill(buffer, getSommet(i)->getCd_x(), getSommet(i)->getCd_y() + getSommet(i)->getImg()->h - 14, getSommet(i)->getCd_x() + 26, getSommet(i)->getCd_y() + getSommet(i)->getImg()->h-1, makecol(200,200,200));
        rect(buffer, getSommet(i)->getCd_x(), getSommet(i)->getCd_y() + getSommet(i)->getImg()->h - 14, getSommet(i)->getCd_x() + 26, getSommet(i)->getCd_y() + getSommet(i)->getImg()->h-1, makecol(255,255,255));
        textprintf_ex(buffer, font, getSommet(i)->getCd_x() +2, getSommet(i)->getCd_y() + getSommet(i)->getImg()->h - 10, makecol(0,0,0), -1, "%d", getSommet(i)->getPoids());
    }

//    textprintf_ex(buffer, font, 400, 23, makecol(255,255,0), -1,"%d", getAretes().size());
//    textprintf_ex(buffer, font, 440, 23, makecol(255,255,0), -1,"%d", mouse_y);

    outils(buffer, barre, a, prev_mouse_b, now_mouse_b);

    draw_sprite(buffer, barre, 0, 600-barre->h);
    blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    clear_bitmap(buffer);
}

void Graphe::outils(BITMAP* buffer, BITMAP* barre, int a, int prev_mouse_b, int now_mouse_b)
{
    if (a != 1)
    {
        if (is_mouse(745, 50, 5, 50))
        {
            rectfill(buffer, 743, 3, 796, 56, makecol(76,201,0));
            if (!prev_mouse_b && now_mouse_b)
            {
                ajouterArete(buffer);
            }
        }

        if (is_mouse(745, 50, 65, 50))
        {
            rectfill(buffer, 743, 63, 796, 116, makecol(255,0,0));
            if (!prev_mouse_b && now_mouse_b)
            {
                supprimerSommet();
            }
        }

        if (is_mouse(745, 50, 125, 50))
        {
            rectfill(buffer, 743, 123, 796, 176, makecol(6,201,0));
            if (!prev_mouse_b && now_mouse_b)
            {
                ajouterSommet(buffer, barre);
            }
        }

        if (is_mouse(745, 50, 185, 50))
        {
            rectfill(buffer, 743, 183, 796, 236, makecol(255,0,0));
            if (!prev_mouse_b && now_mouse_b)
            {
                supprimerArete();
            }
        }

        if (is_mouse(745, 50, 245, 50))
        {
            rectfill(buffer, 743, 243, 796, 296, makecol(127,0,0));
            if (!prev_mouse_b && now_mouse_b)
            {
                std::cout << "hello CFC" << std::endl;
                afficherCFC();
            }
        }

        if (is_mouse(745, 50, 305, 50))
        {
            rectfill(buffer, 743, 303, 796, 356, makecol(18,0,124));
            if (!prev_mouse_b && now_mouse_b)
            {
//                std::cout << "hello calcK" << std::endl;
                std::cout << "hello k-connexite" << std::endl;
//                choix_sommet_calc_k();
                K_connexites();
            }
        }

        if (is_mouse(745, 50, 365, 50))
        {
//            rectfill(buffer, 743, 363, 796, 416, makecol(225,0,0));
            if (!prev_mouse_b && now_mouse_b)
            {
                inverserPlay();
                if (getPlay() == true) std::cout << "hello pause" << std::endl;
                if (getPlay() == false) std::cout << "hello play" << std::endl;
            }
        }
    }

    /// Affichage des boutons
    for (unsigned int i = 0; i<m_boutons.size()-1; ++i)
    {
        if (i != m_boutons.size()-2)
        {
            blit(getBouton(i), buffer, 0, 0, 745, 5+(60*i), getBouton(i)->w, getBouton(i)->h);
        }
        else
        {
            if (getPlay() == true)
                blit(getBouton(i), buffer, 0, 0, 745, 5+(60*i), getBouton(i)->w, getBouton(i)->h);
            else blit(getBouton(i+1), buffer, 0, 0, 745, 5+(60*i), getBouton(i+1)->w, getBouton(i+1)->h);
        }
    }
}

void Graphe::ajouterArete(BITMAP* buffer)
{
    Arete* a = new Arete;
    Sommet* s = new Sommet;
    int prev_mouse_b, now_mouse_b;
    std::vector<Arete*> tmp = getAretes();
    bool deja_ar = false;

    while (s->getNomImg() == "")
    {
        prev_mouse_b = now_mouse_b;
        now_mouse_b = mouse_b&1;
        for (unsigned int i(0); i<getSommets().size(); ++i)
        {
            if (is_sommmet(i))
            {
                if (!prev_mouse_b && now_mouse_b)
                {
                    s = getSommets()[i];
                    a->setDepart(s);
                }
            }
        }
    }

    rect(screen, a->getDepart()->getCd_x()-2, a->getDepart()->getCd_y()-2, a->getDepart()->getImg()->w +a->getDepart()->getCd_x()+1,a->getDepart()->getImg()->h + a->getDepart()->getCd_y()+1, makecol(255,0,0));

    s = new Sommet;

    while (s->getNomImg() == "")
    {
        prev_mouse_b = now_mouse_b;
        now_mouse_b = mouse_b&1;
        for (unsigned int i(0); i<getSommets().size(); ++i)
        {
            if (is_sommmet(i))
            {
                if (!prev_mouse_b && now_mouse_b)
                {
                    if (a->getDepart() != getSommets()[i])
                    {
                        std::cout << "test";
                        s = getSommets()[i];
                        a->setArrive(s);
                    }
                }
            }
        }
    }

    /// Pour chaque arête déjà existante, on vérifie si elle est équivalente à celle que l'on veut ajouter
    for (unsigned int i = 0; i < getAretes().size(); ++i)
    {
        if (a->getArrive() == getAretes()[i]->getArrive() && a->getDepart() == getAretes()[i]->getDepart())
        {
            std::cout << "Cette arete existe deja!" << std::endl;
            deja_ar = true;
        }
    }
    /// Si celle-ci est équivalente, on ne l'ajoute pas
    if (deja_ar == false)
    {
        tmp.push_back(a);
        std::cout << "ajou reussi" << std::endl;
    }
    setAretes(tmp);
}

void Graphe::ajouterSommet(BITMAP* buffer, BITMAP* barre)
{
    Sommet* s;
    int prev_mouse_b;
    int now_mouse_b;

    std::vector<Sommet*> som(getS_Sup());
    std::vector<Sommet*> som_graph(getSommets());

    if (som.size() > 0)
    {
        while (som.size() +1 != getS_Sup().size())
        {
            prev_mouse_b = now_mouse_b;
            now_mouse_b = mouse_b&1;

            for (unsigned int i(0); i < som.size(); ++i)
            {
                if (som.size() > 0)
                {
                    stretch_blit(som[i]->getImg(), buffer, 0, 0, som[i]->getImg()->w, som[i]->getImg()->h, 680, 60*i+5, som[i]->getImg()->w/2, som[i]->getImg()->h/2);
                    blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
                }
            }

            for (unsigned int i(0); i < som.size(); ++i)
            {
                if (is_mouse(680, som[i]->getImg()->w/2, 60*i+5, som[i]->getImg()->h/2))
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

    setS_Sup(som);
    setSommets(som_graph);
}

void Graphe::supprimerSommet()
{
    int prev_mouse_b;
    int now_mouse_b;

    std::vector<Sommet*> temp(getSommets());
    std::vector<Sommet*> temp_supp(getS_Sup());
    std::vector<Arete*> temp_ar(getAretes());

    bool stop(false);

    if (temp.size() > 0)
    {
        while (temp.size()+1 != getSommets().size())
        {
            prev_mouse_b = now_mouse_b;
            now_mouse_b = mouse_b&1;

            for (unsigned int i(0); i < temp.size() ; ++i)
            {
                if (i == 0) stop = false;
                if (is_sommmet(i))
                {
                    if (!prev_mouse_b && now_mouse_b && !stop)
                    {
                        for (unsigned int j(0); j < temp_ar.size(); ++j)
                        {
                            if (temp[i] == temp_ar[j]->getDepart() || temp[i] == temp_ar[j]->getArrive())
                            {
                                temp_ar.erase(temp_ar.begin() + j);
                                --j;
                            }
                        }

                        temp_supp.push_back(temp[i]);
                        temp.erase(temp.begin()+i);
                        stop = true;
                    }
                }
            }
        }
    }

    setSommets(temp);
    setS_Sup(temp_supp);
    setAretes(temp_ar);
}

void Graphe::supprimerArete()
{
    Sommet* s1 = new Sommet;
    std::vector<Arete*> temp = getAretes();
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
            for (unsigned int i(0); i<getSommets().size(); ++i)
            {
                /// Si on est sur le sommet et que l'on clic gauche
                if (is_sommmet(i) && !prev_mouse_b && now_mouse_b)
                {
                    for (unsigned int j(0); j<temp.size(); ++j)
                    {
                        /// Si le sommet clické i appartient à au moins une arête en tant que sommet de départ
                        if (temp[j]->getDepart() == getSommets()[i])
                        {
                            s1 = getSommets()[i];
                        }
                    }
                }
            }
        }

        rect(screen, s1->getCd_x()-2, s1->getCd_y()-2, s1->getCd_x() + s1->getImg()->w+1, s1->getCd_y() + s1->getImg()->h+1, makecol(255,12,12));
        rect(screen, s1->getCd_x()-1, s1->getCd_y()-1, s1->getCd_x() + s1->getImg()->w, s1->getCd_y() + s1->getImg()->h, makecol(255,12,12));

        while (temp.size() == getAretes().size())
        {
            prev_mouse_b = now_mouse_b;
            now_mouse_b = mouse_b&1;
            for (unsigned int i(0); i < getSommets().size(); ++i)
            {
                if (is_sommmet(i) && !prev_mouse_b && now_mouse_b)
                {
                    for (unsigned int j(0); j < temp.size(); ++j)
                    {
                        /// Si le sommet clické i appartient à la même arète que le sommet de départ
                        if (temp[j]->getArrive() == getSommets()[i] && temp[j]->getDepart() == s1 && !stop)
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

    setAretes(temp);
}

void Graphe::update(BITMAP* buffer, BITMAP* barre, int prev, int now)
{
    int prev_mouse_b = 0;
    int now_mouse_b = mouse_b&1;

    prev_mouse_b = now_mouse_b;
    now_mouse_b = mouse_b&1;

    blit(getDecor(), buffer, 0, 0, 0, 0, getDecor()->w, getDecor()->h);

    for (int i(getSommets().size()-1); i >= 0 ; --i)
    {
        if (is_sommmet(i))
        {
            if (mouse_b&1)
            {
                while (prev_mouse_b && now_mouse_b)
                {
                    prev_mouse_b = now_mouse_b;
                    now_mouse_b = mouse_b&1;

                    blit(getDecor(), buffer,0,0,0,0,getDecor()->w, getDecor()->h);

                    rectfill(buffer, getSommets()[i]->getCd_x()-2, getSommets()[i]->getCd_y()-2, getSommets()[i]->getImg()->w + getSommets()[i]->getCd_x()+1, getSommets()[i]->getImg()->h + getSommets()[i]->getCd_y()+1, makecol(0,255,0));

                    getSommets()[i]->setCd_x(mouse_x - getSommets()[i]->getImg()->w/2);
                    getSommets()[i]->setCd_y(mouse_y - getSommets()[i]->getImg()->h/2);

                    if (getSommets()[i]->getCd_x() + getSommets()[i]->getImg()->w > 740)
                        getSommets()[i]->setCd_x(740 - getSommets()[i]->getImg()->w);

                    if (getSommets()[i]->getCd_y() + getSommets()[i]->getImg()->h > SCREEN_H-barre->h)
                        getSommets()[i]->setCd_y(SCREEN_H - barre->h - getSommets()[i]->getImg()->h);

                    if (getSommets()[i]->getCd_x() < 0)
                        getSommets()[i]->setCd_x(0);

                    if (getSommets()[i]->getCd_y() < 0)
                        getSommets()[i]->setCd_y(0);

                    affichage(buffer, barre, 1, prev_mouse_b, now_mouse_b);
                }
            }
        }
    }
}

void Graphe::save()
{
    int ordre = getS_Sup().size();
    std::string nom_fichier(getNomGraphe());

    std::ofstream fichier(nom_fichier, std::ios::out);
    if (!fichier)
    {
        std::cout << nom_fichier << " ne peut pas etre ouvert!" << std::endl;
        return;
    }
    else
    {
        fichier << ordre << std::endl << std::endl;
        if (ordre > 0)
        {
           for (int i = 0; i < ordre; i++)
            {
                fichier << getS_Sup()[i]->getNomImg() << std::endl;
                fichier << getS_Sup()[i]->getCd_x() << " " << getS_Sup()[i]->getCd_y() << " " << getS_Sup()[i]->getNum() << " " << getS_Sup()[i]->getPoids() << std::endl;
                fichier << std::endl;
            }
        }


        ordre = getSommets().size();
        fichier << ordre << std::endl << std::endl;
        if (ordre > 0)
        {
            for (int i = 0; i < ordre; i++)
            {
                fichier << getSommet(i)->getNomImg() << std::endl;
                fichier << getSommet(i)->getCd_x() << " " << getSommet(i)->getCd_y() << " " << getSommet(i)->getNum() << " " <<getSommet(i)->getPoids() << std::endl;
                fichier << std::endl;
            }
        }

        ordre = getAretes().size();

        fichier << ordre << std::endl;

        if (ordre > 0)
        {
            for (int i = 0; i < ordre; i++)
            {
                fichier << getAretes()[i]->getDepart()->getNomImg() << " "<< getAretes()[i]->getArrive()->getNomImg() << " " << getAretes()[i]->getPoids() << std::endl;
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
    return     mouse_x >= getSommets()[i]->getCd_x() && mouse_x <= getSommets()[i]->getCd_x() + getSommets()[i]->getImg()->w
               &&  mouse_y >= getSommets()[i]->getCd_y() && mouse_y <= getSommets()[i]->getCd_y() + getSommets()[i]->getImg()->h;
}

void Graphe::slider()
{
    float poids = 0;

    for (unsigned int i = 0; i < getSommets().size(); i++)
    {
        if(is_sommmet(i))
        {
            poids = getSommets()[i]->getPoids();

            if (key[KEY_UP])
            {
                std::cout << "On AUGMENTE la population " << getSommets()[i]->getNomImg() << std::endl;
                poids +=1;

                if(poids > 100.0) poids = 100.0;
                getSommets()[i]->setPoids(poids);
            }
            if (key[KEY_DOWN])
            {
                std::cout << "On DIMINUE la population " << getSommets()[i]->getNomImg() << std::endl;
                poids -= 1;

                if (poids < 0.0) poids = 0.0;
                getSommets()[i]->setPoids(poids);
            }
        }
    }
}

void Graphe::sliderArete()
{

    int xsDep = 0, ysDep = 0, xsArr = 0, ysArr = 0;
    float poids = 0;

    for (unsigned int i = 0; i < getAretes().size(); i++)
    {

        xsDep = getAretes()[i]->getDepart()->getCd_x() + getAretes()[i]->getDepart()->getImg()->w/2;
        ysDep = getAretes()[i]->getDepart()->getCd_y() + getAretes()[i]->getDepart()->getImg()->h/2;
        xsArr = getAretes()[i]->getArrive()->getCd_x() + getAretes()[i]->getArrive()->getImg()->w/2;
        ysArr = getAretes()[i]->getArrive()->getCd_y() + getAretes()[i]->getArrive()->getImg()->h/2;

        if (is_mouse((xsDep + 3*xsArr)/4 - 15, 30, (ysDep + 3*ysArr)/4 - 15, 30))
        {
           poids = getAretes()[i]->getPoids();

           if (key[KEY_UP])
            {
                poids ++;
                if(poids > 200) poids = 200;
                getAretes()[i]->setPoids(poids);
            }
            if (key[KEY_DOWN])
            {
                poids--;
                if( poids < 0) poids = 0;
                getAretes()[i]->setPoids(poids);
            }
        }
    }
}

int Graphe::calcul_K(Sommet* sDep)
{
	int calcul_k = 0;
	for (unsigned int i = 0 ; i<getSommets().size(); ++i)
	{
//	    std::cout << "sommet" << i << std::endl;
		calcul_k += calcul_sommet(sDep, getSommet(i));
	}
	return calcul_k;
}

int Graphe::calcul_sommet(Sommet* sDep, Sommet* sArr)
{
	for (unsigned int i = 0; i<getAretes().size(); ++i)
	{
	    // Si le sommet de départ mange le sommet d'arrive
        if (getAretes()[i]->getDepart() == sDep &&  getAretes()[i]->getArrive() == sArr)
        {
            return (getArete(i)->getPoids() * getArete(i)->getArrive()->getPoids());
        }
        else if (i == getAretes().size()-1) return 0;
	}
}

void Graphe::choix_sommet_calc_k()
{
    int coefK = 0;
    Sommet* s = new Sommet;
    int prev_mouse_b, now_mouse_b;

    while (s->getNomImg() == "")
    {
        prev_mouse_b = now_mouse_b;
        now_mouse_b = mouse_b&1;
        for (unsigned int i = 0; i<getSommets().size(); ++i)
        {
            if (is_sommmet(i) && !prev_mouse_b && now_mouse_b)
            {
                s = getSommets()[i];
                coefK = calcul_K(getSommets()[i]);
                std::cout << "K[" << i << "] = " << coefK << std::endl;
            }
        }
    }

}

void Graphe::afficherCFC()
{
    int nb_recur = 0;
    std::vector<int> lig;
    std::list<int> Stack;
    std::vector<std::vector<int>> m_adjCFC;
    bool* visited = new bool[m_ordre + m_nb_s_sup];

    for (int i = 0; i < m_ordre + m_nb_s_sup; ++i) visited[i] = false;
    for (int i = 0; i < m_ordre + m_nb_s_sup; ++i)
    {
        m_adjCFC.push_back(lig);
        if (visited[i] == false) ordreRemplissage(i, visited, Stack);
    }
    Graphe gr = getTranspose();

    for (int i = 0; i < m_ordre + m_nb_s_sup; ++i) visited[i] = false;
    while (!Stack.empty())
    {

        int v = Stack.front();
        Stack.pop_front();
        if (visited[v] == false)
        {
            gr.composanteRecursif(v, visited, nb_recur, m_adjCFC);
            std::cout << std::endl;
            ++nb_recur;
        }
    }
    std::cout << "Affichage du double vecteur de composantes connexes" << std::endl;
    for (unsigned int i = 0; i< m_adjCFC.size(); ++i)
    {
        std::cout << "Size m_adjCFC[ " << i << "] : " << m_adjCFC.at(i).size() << " --- Sommets connexes : ";
        for (unsigned int j =  0; j < m_adjCFC.at(i).size(); ++j)
        {
            std::cout << m_adjCFC[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void Graphe::composanteRecursif(int v, bool visited[], int nb_recur, std::vector<std::vector<int>>& m_adjCFC)
{
    visited[v] = true;
    std::cout << v << " ";
    m_adjCFC[nb_recur].push_back(v);

    std::list<int>::iterator i;
    for (i = m_adjacences[v].begin(); i != m_adjacences[v].end(); ++i)
    {
        if (!visited[*i]) composanteRecursif(*i, visited, nb_recur, m_adjCFC);
    }
}

Graphe Graphe::getTranspose()
{
    Graphe g(m_ordre + m_nb_s_sup);
    std::cout << "true ordre : " << m_ordre + m_nb_s_sup << std::endl;
    for (int v = 0; v < m_ordre + m_nb_s_sup; ++v)
    {
        std::cout << "SOMMET : " << v;

        std::list<int>::iterator i;
        std::cout << " [g base]        [g transpo]" << std::endl;
        for (i = m_adjacences[v].begin(); i != m_adjacences[v].end(); ++i)
        {

            std::cout << "            " << v << " -> " << *i << "          " << *i << " -> " << v;
            g.m_adjacences[*i].push_back(v);
            std::cout << " reussi" << std::endl;
        }
    }
    return g;
}

void Graphe::ordreRemplissage(int v, bool visited[], std::list<int> &Stack)
{
    visited[v] = true;
    std::list<int>::iterator i;
    for (i = m_adjacences[v].begin(); i != m_adjacences[v].end(); ++i)
    {
        if (!visited[*i]) ordreRemplissage(*i, visited, Stack);
    }
    Stack.push_back(v);
}

void Graphe::K_connexites()
{
    resetMarques();
    int* compteur = new int;
    compteur = 0;
    std::cout << &compteur << std::endl;
    std::cout << "true ordre (vivant) = " << m_ordre << std::endl;
//    for (std::map<int, std::vector<int>>::const_iterator it = m_adj_adj.begin(); it != m_adj_adj.end(); ++it)
    for (const auto& sommet_ex : m_adj_adj)
    {
        if(!getSommet(sommet_ex.first)->getMarque())
        {
            std::cout << "test " << std::endl;
//            if (i != 0) ++compteur;
            getSommet(sommet_ex.first)->setMarque(true);
            for (const auto& s_adj : sommet_ex.second)
            {
                if (!getSommet(s_adj)->getMarque())
                {
                    getSommet(s_adj)->setMarque(true);
                    recursKConnexite(s_adj, compteur);
                }
            }
        }
    }
    if (compteur == 0) std::cout << "le truc est connexe simplement au moins" << std::endl;
    else std::cout << " pas connexe" << std::endl;
}

void Graphe::recursKConnexite(int indice, int* compteur)
{
    for (unsigned int i = 0; i < getSommet(i)->getAdjS().size(); ++i)
    {
        if (!getSommet(getSommet(indice)->getAdjS()[i])->getMarque())
        {
            getSommet(getSommet(indice)->getAdjS()[i])->setMarque(true);
            recursKConnexite(getSommet(indice)->getAdjS()[i], compteur);
        }
    }
}
