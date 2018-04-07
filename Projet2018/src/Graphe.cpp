#include "Graphe.h"

Graphe::Graphe(std::string nom_fichier, std::string nom_decor)
{
    setOrdre(0);
    setNomGraphe(nom_fichier);
    BITMAP* bouton;
    setTime(0);
    m_play = false;

    setDecor(load_bitmap(nom_decor.c_str(), NULL));
    if (!getDecor())
    {
        allegro_message("pas pu trouver decor.png");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i<7; ++i)
    {
        if (i == 0) bouton = load_bitmap("Graphe1/Images/addA.png", NULL);
        if (i == 1) bouton = load_bitmap("Graphe1/Images/suppS.png", NULL);
        if (i == 2) bouton = load_bitmap("Graphe1/Images/addS.png", NULL);
        if (i == 3) bouton = load_bitmap("Graphe1/Images/suppA.png", NULL);
        if (i == 4) bouton = load_bitmap("Graphe1/Images/CFC.png", NULL);
        if (i == 5) bouton = load_bitmap("Graphe1/Images/play.png", NULL);
        if (i == 6) bouton = load_bitmap("Graphe1/Images/pause.png", NULL);
        ajouterBouton(bouton);
    }
}

Graphe::~Graphe()
{

}

void Graphe::init()
{

    std::vector<Sommet*> *tmp(new std::vector<Sommet*>);
    std::vector<Arete*> *tmp2(new std::vector<Arete*>);

    setOrdre(0);
    setTime(0);
    setSommets(*tmp);
    tmp = new std::vector<Sommet*>;
    setS_Sup(*tmp);
    setAretes(*tmp2);

    recuperation();
}

void Graphe::recuperation()
{
    int cd_x, cd_y, num, poids, nb_aretes;
    std::string nom_img, nom_img_d, nom_img_a;

    std::string nom_fichier(getNomGraphe());
    std::cout << " Chargement de : " << nom_fichier << std::endl;

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

        thick_line(buffer, xsDep, ysDep, xsArr, ysArr, getAretes()[i]->getPoids()/2, makecol(getAretes()[i]->getPoids()*10,255,0));
        circlefill(buffer, (xsDep + 3*xsArr)/4, (ysDep + 3*ysArr)/4, radius, makecol(255,getAretes()[i]->getPoids()*10,0));
        textprintf_ex(buffer, font, (xsDep + 3*xsArr)/4 - 6, (ysDep + 3*ysArr)/4 - 6, makecol(0,0,0), -1, "%d", getAretes()[i]->getPoids());
    }

    for (unsigned int i(0); i<getSommets().size(); ++i)
    {
        ///Affichage forte connexité
        if(getSommet(i)->getConnexe() == true)
            rectfill(buffer, getSommets()[i]->getCd_x()-4, getSommets()[i]->getCd_y()-4, getSommets()[i]->getImg()->w + getSommets()[i]->getCd_x()+4, getSommets()[i]->getImg()->h + getSommets()[i]->getCd_y()+4, makecol(255, 0, 255));

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
                save();
                init();
                afficherCFC();
                setTime(0);
            }
        }

        if (is_mouse(745, 50, 305, 50))
        {
            rectfill(buffer, 743, 303, 796, 356, makecol(225,0,0));
            if (!prev_mouse_b && now_mouse_b)
            {
                inverserPlay();
                if (getPlay() == true)
                {
                    calc_pop();
                }
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

    if(getPlay() == true)
    {
        if(getTime()%300 == 0)
            calc_pop();
    }

    if(getTime()%3000 == 2999)
    {
        for(unsigned int i(0); i < getSommets().size(); ++i)
            getSommet(i)->setConnexe(false);

        for(unsigned int i(0); i < getS_Sup().size(); ++i)
            getS_Sup()[i]->setConnexe(false);
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

    /// Pour chaque ar�te d�j� existante, on v�rifie si elle est �quivalente � celle que l'on veut ajouter
    for (unsigned int i = 0; i < getAretes().size(); ++i)
    {
        if (a->getArrive() == getAretes()[i]->getArrive() && a->getDepart() == getAretes()[i]->getDepart())
        {
            std::cout << "Cette arete existe deja!" << std::endl;
            deja_ar = true;
        }
    }
    /// Si celle-ci est �quivalente, on ne l'ajoute pas
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
                        /// Si le sommet click� i appartient � au moins une ar�te en tant que sommet de d�part
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
                        /// Si le sommet click� i appartient � la m�me ar�te que le sommet de d�part
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

///Permet de modifier l'épaisseur des aretes en fonction du poids
void Graphe::thick_line(BITMAP *buffer, int xDep, int yDep, int xArr, int yArr, int epaisseur, int col)
{
    int dx = xArr - xDep;
    int dy = yArr - yDep;

    if(epaisseur < 1)   /**< Blindage */
    {
        epaisseur = 1;
    }

    if (abs(dx) > abs(dy))  /**< Valeur absolue des differences des coordonnes et trace autant de ligne que d'epaisseur */
    {
        for (int i = 1 - epaisseur; i < epaisseur; ++i)
        {
            line(buffer, xDep, yDep + i, xArr, yArr + i, col);
        }
    }
    else
    {
        for (int i = 1 - epaisseur; i < epaisseur; ++i)
        {
            line(buffer, xDep + i, yDep, xArr + i, yArr, col);
        }
    }
}

///Methode de sauvegarde
void Graphe::save()
{
    int ordre = getS_Sup().size();
    std::string nom_fichier(getNomGraphe());

    std::ofstream fichier(nom_fichier, std::ios::out);  /**< Ouverture du fichier */
    if (!fichier)
    {
        std::cout << nom_fichier << " ne peut pas etre ouvert!" << std::endl;
        return;
    }
    else
    {
        fichier << ordre << std::endl << std::endl; /**< Ecrit l'odre des images pouvant être ajoute au graphe */
        if (ordre > 0)  /**< Blindage */
        {
            for (int i = 0; i < ordre; i++)     /**< Ajout des informations des images */
            {
                fichier << getS_Sup()[i]->getNomImg() << std::endl; /**< Son nom  (chemin d'acces) */
                fichier << getS_Sup()[i]->getCd_x() << " " << getS_Sup()[i]->getCd_y() << " " << getS_Sup()[i]->getNum() << " " << getS_Sup()[i]->getPoids() << std::endl;
                /**< Ses coordonnees, son index, son poids */

                fichier << std::endl;
            }
        }


        ordre = getSommets().size();
        fichier << ordre << std::endl << std::endl;     /**< Ecrit l'odre des images du graphe */
        if (ordre > 0)  /**< Blindage */
        {
            for (int i = 0; i < ordre; i++)     /**< Ajout des informations des images */
            {
                fichier << getSommet(i)->getNomImg() << std::endl;  /**< Son nom (chemin d'acces) */
                fichier << getSommet(i)->getCd_x() << " " << getSommet(i)->getCd_y() << " " << getSommet(i)->getNum() << " " <<getSommet(i)->getPoids() << std::endl;
                /**< Ses coordonnees, son index, son poids */

                fichier << std::endl;
            }
        }

        ordre = getAretes().size();

        fichier << ordre << std::endl;  /**< Ecrit l'odre des aretes du graphe */

        if (ordre > 0)  /**< Blindage */
        {
            for (int i = 0; i < ordre; i++)     /**< Ajout des informations des aretes */
            {
                fichier << getAretes()[i]->getDepart()->getNomImg() << " "<< getAretes()[i]->getArrive()->getNomImg() << " " << getAretes()[i]->getPoids() << std::endl;
                /**< Nom de l'image de depart (chemin d'acces) et nom de l'image d'arrive (chemin d'acces) et poids de l'arete */
            }
        }
    }
}

///Booleen qui retourne si on est sur une zone
bool is_mouse(int x, int weight, int y, int height)
{
    return     mouse_x >= x && mouse_x <= x + weight  &&  mouse_y >= y && mouse_y <= y + height;
}

///Booleen qui retourne true si l'on est sur l'image du sommet i
bool Graphe::is_sommmet(int i)
{
    return     mouse_x >= getSommets()[i]->getCd_x() && mouse_x <= getSommets()[i]->getCd_x() + getSommets()[i]->getImg()->w
               &&  mouse_y >= getSommets()[i]->getCd_y() && mouse_y <= getSommets()[i]->getCd_y() + getSommets()[i]->getImg()->h;
}

///Modifie le poids des Sommets
void Graphe::slider()
{
    float poids = 0;

    for (unsigned int i = 0; i < getSommets().size(); i++)
    {
        if(is_sommmet(i)) /**< Test si la souris est sur le Sommet i */
        {
            poids = getSommets()[i]->getPoids();

            if (key[KEY_UP])    /**< Augmentation du poids du Sommet */
            {
                if(poids < 100) poids +=1;  /**< Augmente de 1 à 100 en incrementant de 1 */

                else if(poids >= 100) poids += 100;  /**< Augmente de 100 à 500 en incrementant de 100 */

                if(poids > 500.0) poids = 500.0;    /**< Blindage */

                getSommets()[i]->setPoids(poids);   /**< affectation du nouveau poids */
                key[KEY_UP] = 0;    /**< Blindage */
            }
            if (key[KEY_DOWN])  /**< Reduction du poids du Sommet */
            {
                if(poids <= 100) poids -= 1;    /**< Dimunution de 100 à 1 en decrementant de 1 */

                if(poids > 100) poids -=100;    /**< Dimunution de 500 à 100 en decrementant de 100 */

                if (poids < 0.0) poids = 0.0;   /**< Blindage */

                getSommets()[i]->setPoids(poids);   /**< affectation du nouveau poids */
                key[KEY_DOWN] = 0;  /**< Blindage */
            }
        }
    }
}

///Modifie le poids des Arete
void Graphe::sliderArete()
{

    int xsDep = 0, ysDep = 0, xsArr = 0, ysArr = 0;
    float poids = 0;

    for (unsigned int i = 0; i < getAretes().size(); i++)
    {

        xsDep = getAretes()[i]->getDepart()->getCd_x() + getAretes()[i]->getDepart()->getImg()->w/2;    /**< Coordonnees de l'image de départ */
        ysDep = getAretes()[i]->getDepart()->getCd_y() + getAretes()[i]->getDepart()->getImg()->h/2;
        xsArr = getAretes()[i]->getArrive()->getCd_x() + getAretes()[i]->getArrive()->getImg()->w/2;    /**< Coordonnees de l'image de arrive */
        ysArr = getAretes()[i]->getArrive()->getCd_y() + getAretes()[i]->getArrive()->getImg()->h/2;

        if (is_mouse((xsDep + 3*xsArr)/4 - 15, 30, (ysDep + 3*ysArr)/4 - 15, 30))   /**< Coordonnees du cercle situe sur l'arete */
        {
           poids = getAretes()[i]->getPoids();

           if (key[KEY_UP]) /**< Augmentation du Poids */
            {
                poids ++;
                if(poids > 20) poids = 20; /**< Blindage */
                getAretes()[i]->setPoids(poids);    /**< affectation du nouveau poids */
                key[KEY_UP] = 0;
            }
            if (key[KEY_DOWN]) /**< Reduction du Poids */
            {
                poids--;
                if( poids < 1 ) poids = 1; /**< Blindage */
                getAretes()[i]->setPoids(poids);    /**< affectation du nouveau poids */
                key[KEY_DOWN] = 0;
            }
        }
    }
}

///Play/Pause pour la dynamique des populations
void Graphe::inverserPlay()
{
    m_play = !m_play;
}

///calcul du nombre de perte chez la population
float Graphe::Mange(Sommet* s)
{
    int K(0);

    for(unsigned int i(0); i < getAretes().size(); ++i)
    {
        if(s == getAretes()[i]->getDepart() && getAretes()[i]->getDepart()->getPoids() > 0 && getAretes()[i]->getArrive()->getPoids() > 0 ) /**< Cherche le repas et verifie qu'il est en vie */
        {
            K += getAretes()[i]->getPoids()*getAretes()[i]->getDepart()->getPoids();
        }
    }
    return K;
}

///calcul du nombre de naissance pour le sommet
float Graphe::Plat(Sommet* s)
{
    int K(0);

    for(unsigned int i(0); i < getAretes().size(); ++i)
    {
        if(s == getAretes()[i]->getArrive() && getAretes()[i]->getArrive()->getPoids() > 0) /**< Cherche le mangeur et verifie qu'il est en vie */
        {
            K += getAretes()[i]->getPoids()*getAretes()[i]->getDepart()->getPoids();
        }
    }
    return K;
}

///Calcule la prochaine population
void Graphe::calc_pop()
{
    float valeur[getSommets().size()];

    float faim;
    float repas;
    float diff(0);

    if(getSommets().size() > 0) /**< Blindage */
    {
        for(unsigned int i = 0; i < getSommets().size(); i++)
        {
            faim = Mange(getSommets()[i]); /**< Récupere le calcul du nombre de naissance pour le sommet */
            repas = Plat(getSommets()[i]); /**< Récupere le calcul du nombre de perte chez la population */

            if(getSommets()[i]->getNum() == 4 || getSommets()[i]->getNum() == 6) /**< Gain pour les plantes */
            {
                faim = 2;
            }

            diff = faim - repas; /**< Calcul des pertes et gain */

            valeur[i] = getSommets()[i]->getPoids() + diff; /**< Ajoute le nouveau poids dans une variable*/

            if(faim == 0 && repas == 0) /**< Si il ne mange pas alors et on ne les manges pas alors l'espèces commence à disparaitre */
                valeur[i] = getSommets()[i]->getPoids() - 1;

            if(faim == 0) /**< Si l'espece ne peut pas ce nourir elle commence à disparaitre */
                valeur[i] -= 1;
        }

        for(unsigned int i = 0; i < getSommets().size(); i++) /**< affectation des nouveaux poids */
        {
            if(valeur[i] < 0) /**< Blindage */
                valeur[i] = 0;

            if(valeur[i] > 500) /**< Blindage */
                valeur[i] = 500;

            getSommets()[i]->setPoids(valeur[i]); /**< Affectation */
        }
    }

}

void Graphe::afficherCFC()
{
    int nb_recur = 0;
    std::vector<int> lig;
    std::list<int> Stack;
    std::vector<std::vector<int>> adjCFC;
    bool* visited = new bool[m_ordre + m_nb_s_sup];

    for (int i = 0; i < m_ordre + m_nb_s_sup; ++i) visited[i] = false;
    for (int i = 0; i < m_ordre + m_nb_s_sup; ++i)
    {
        adjCFC.push_back(lig);
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
            gr.composanteRecursif(v, visited, nb_recur, adjCFC);
            std::cout << std::endl;
            ++nb_recur;
        }
    }
    std::cout << "Affichage du double vecteur de composantes connexes" << std::endl;
    for (unsigned int i = 0; i< adjCFC.size(); ++i)
    {
        std::cout << "Size adjCFC[ " << i << "] : " << adjCFC.at(i).size() << " --- Sommets connexes : ";
        for (unsigned int j =  0; j < adjCFC.at(i).size(); ++j)
        {
            std::cout << adjCFC[i][j] << " ";
        }
        std::cout << std::endl;
    }

    affichagedesComposantesFortementConnexes(adjCFC);
}

void Graphe::composanteRecursif(int v, bool visited[], int nb_recur, std::vector<std::vector<int>>& adjCFC)
{
    visited[v] = true;
    std::cout << v << " ";
    adjCFC[nb_recur].push_back(v);

    std::list<int>::iterator i;
    for (i = m_adjacences[v].begin(); i != m_adjacences[v].end(); ++i)
    {
        if (!visited[*i]) composanteRecursif(*i, visited, nb_recur, adjCFC);
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

///Modifie le booleen qui affiche la connexite
void Graphe::affichagedesComposantesFortementConnexes(std::vector<std::vector<int> > connexe)
{
    if(connexe.size() > 0) /**< Blindage */
    {
        for(unsigned int i(0); i < connexe.size(); ++i)
        {
            if(connexe[i].size() >= 2) /**< Permet de savoir si le sommet est fortement connexe */
            {
                for(unsigned int j(0); j < connexe[i].size(); ++j)
                {
                    if(getSommets().size() > 0) /**< Blindage */
                    {
                        for(unsigned int k(0); k < getSommets().size(); ++k)
                        {
                            if(getSommet(k)->getNum() ==  connexe[i][j]) /**< Cherche un sommet qui possede le index meme numero que celui du vector de connexite */
                            {
                                getSommet(k)->setConnexe(true);
                            }
                        }
                    }
                }
            }
        }
    }
}
