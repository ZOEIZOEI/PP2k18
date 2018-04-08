#include "Graphe.h"

/**
 * \date 8 Avril 2018
 * \version 3.1
 * \file Graphe.h
 * \author Elias Aras, Baptiste Pillet, Nathan Lancman
 * \brief La classe Graphe permet de gerer et d'afficher tous les graphes (et leurs parametres)
 *
 */

Graphe::Graphe(std::string nom_fichier, std::string nom_decor)
{
    setOrdre(0); //On initialise le nombre de sommets a 0
    setNomGraphe(nom_fichier);
    BITMAP* bouton;
    setTime(0);
    m_play = false; //On ne commence pas encore la dynamique des population
    m_reduit = false;

    setDecor(load_bitmap(nom_decor.c_str(), NULL));
    if (!getDecor())
    {
        allegro_message("pas pu trouver decor.png");
        exit(EXIT_FAILURE);
    }
//Ajout des bitmap de chaque bouton
    for (int i = 0; i < 11; ++i)
    {
        if (i == 0) bouton = load_bitmap("Graphe1/Images/addA.png", NULL);
        if (i == 1) bouton = load_bitmap("Graphe1/Images/suppS.png", NULL);
        if (i == 2) bouton = load_bitmap("Graphe1/Images/addS.png", NULL);
        if (i == 3) bouton = load_bitmap("Graphe1/Images/suppA.png", NULL);
        if (i == 4) bouton = load_bitmap("Graphe1/Images/CFC.png", NULL);
        if (i == 5) bouton = load_bitmap("Graphe1/Images/cancel.png", NULL);
        if (i == 6) bouton = load_bitmap("Graphe1/Images/calcK.png", NULL);
        if (i == 7) bouton = load_bitmap("Graphe1/Images/play.png", NULL);
        if (i == 8) bouton = load_bitmap("Graphe1/Images/pause.png", NULL);
        if (i == 9) bouton = load_bitmap("Graphe1/Images/reduit2.png", NULL);
        if (i == 10) bouton = load_bitmap("Graphe1/Images/reduit.png", NULL);



        ajouterBouton(bouton);
    }
}

Graphe::~Graphe()
{

}

///Initialisation du graphe
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

    recuperation(); //On recupere le fichier de sauvegarde pour remplir les vecteurs
}

///Recuperation du fichier de sauvegarde
void Graphe::recuperation()
{
    int cd_x, cd_y, num, poids, nb_aretes;
    std::string nom_img, nom_img_d, nom_img_a;

    std::string nom_fichier(getNomGraphe());
    std::cout << " Chargement de : " << nom_fichier << std::endl;

    std::vector<Arete*> tmp;

    Arete* a;
    Sommet* s;
///FICHIER :
    std::ifstream fichier(nom_fichier, std::ios::in);   //Ouverture en Lecture
    if (!fichier)
    {
        std::cout << nom_fichier << " ne peut pas etre ouvert!" << std::endl;
        return;
    }
    else
    {
///PREMIERE PARTIE : sommets supprimes du graphe
        fichier >> m_nb_s_sup;  //Le premier chiffre en tete du fichier de sauvegarde correspond a la partie des sommets existants mais qui ne font pas partie du graphe (qui ont ete supprimes)
        if (m_nb_s_sup > 0) //On lance la boucle uniquement s'il y a des donnees
        {
            for (int i(0); i < m_nb_s_sup; ++i)  //On enregistre dans le vecteur les sommets que nous n'afficherons pas (ceux supprimé par l'utilisateur)
            {
                fichier >> nom_img;
                fichier >> cd_x >> cd_y >> num >> poids;
                if (nom_img != "")
                {
                    s = new Sommet(cd_x, cd_y, num, poids, nom_img); //on creer un nouveau sommet a chaqeu fois
                    m_s_sup.push_back(s);
                }
            }
        }
///DEUXIEME PARTIE : sommets du graphe
        fichier >> m_ordre; //Le second chiffre correspond au nombre de sommets presents dans le graphe
        m_adjacences = new std::list<int>[m_ordre + m_nb_s_sup]; //La liste d'adjacence cree regroupe les sommets presents et ceux suppries du graphe (pour sa taille)
        if (m_ordre > 0)
        {
            for (int i(0); i < m_ordre; ++i)
            {
                fichier >> nom_img;
                fichier >> cd_x >> cd_y >> num >> poids;
                s = new Sommet(cd_x, cd_y, num, poids, nom_img); //Apres avoir recuperer les données du sommet, on le creer
                m_sommets.push_back(s); //Et on l'ajoute au vecteur de sommet
            }
        }
///TROISIEME PARTIE : aretes du graphe
        fichier >> nb_aretes;   //Le troisieme chiffre correspond au nombre d'arete du graphe
        std::cout << "Nombre d'aretes du graphe : " << nb_aretes << std::endl << std::endl;

        if (nb_aretes > 0)
        {
            for (int k(0); k < nb_aretes; ++k)
            {
                fichier >> nom_img_d >> nom_img_a >> poids; //On recupere les noms des images des sommets d'arrivee et de depart de l'arets + le poids de l'arete

                if (getSommets().size() > 0 && nom_img_a != "")
                {
                    a = new Arete;  //On creer une arete

                    for (unsigned int i(0); i < getSommets().size(); ++i)
                    {
                        if (getSommets()[i]->getNomImg() == nom_img_d)  //Si l'image du sommet d'arriver du fichier de sauvegarde correspond a celle du vecteur de sommets...
                        {
                            s = new Sommet;     //On creer un nouveau sommet
                            s = getSommets()[i];    //on lui donne la valeur de celui du vecteur
                            a->setDepart(s);    //On lui donne la meme coordonnees
                        }

                        if (getSommets()[i]->getNomImg() == nom_img_a) //Meme chose pour le sommet d'arriver
                        {
                            s = new Sommet;
                            s = getSommets()[i];
                            a->setArrive(s);
                            a->setPoids(poids); //on lui attribut le poids
                        }
                    }
                    tmp.push_back(a);  //On ajoute l'arete cree dans le vecteur d'arete
                    std::cout << "[" << a->getDepart()->getNum() << "] -> [" << a->getArrive()->getNum();
                    std::cout << "] POIDS : " << a->getPoids() << std::endl;
                    m_adjacences[a->getDepart()->getNum()].push_back(a->getArrive()->getNum()); // On crée la matrice d'adjacence de sommet si on a un "ariver" et un "depart" pour une arete
                }
            }
        }
    }
    setAretes(tmp);
    initAdjAdj();
}

///Affichage des
void Graphe::affichage(BITMAP* buffer, BITMAP* barre, int a, int prev_mouse_b, int now_mouse_b)
{
    int xsDep = 0, ysDep = 0, xsArr = 0, ysArr = 0;
    int radius = 15;

    sliderArete(); //sous programme permettant de faire varier le poids des aretes avec les touches directionnelles

    for (unsigned int i(0); i < getAretes().size(); ++i) //On parcours le vecteur d'arete...
    {
        xsDep = getAretes()[i]->getDepart()->getCd_x() + getAretes()[i]->getDepart()->getImg()->w/2; //initialisation des coordonnées des aretes
        ysDep = getAretes()[i]->getDepart()->getCd_y() + getAretes()[i]->getDepart()->getImg()->h/2;
        xsArr = getAretes()[i]->getArrive()->getCd_x() + getAretes()[i]->getArrive()->getImg()->w/2;
        ysArr = getAretes()[i]->getArrive()->getCd_y() + getAretes()[i]->getArrive()->getImg()->h/2;

        thick_line(buffer, xsDep, ysDep, xsArr, ysArr, getAretes()[i]->getPoids()/2, makecol(getAretes()[i]->getPoids()*10,255,0)); //L'epaisseur de l'arete varie selon le poids de celle si, sa couleur egalement
        circlefill(buffer, (xsDep + 3*xsArr)/4, (ysDep + 3*ysArr)/4, radius, makecol(255,getAretes()[i]->getPoids()*10,0)); //Cercle permettant l'oriantation des aretes (au 3/4 de l'arete)
        textprintf_ex(buffer, font, (xsDep + 3*xsArr)/4 - 6, (ysDep + 3*ysArr)/4 - 6, makecol(0,0,0), -1, "%d", getAretes()[i]->getPoids()); //Affichage du poids de l'arete dans le cercle d'orientation

    }

    for (unsigned int i(0); i<getSommets().size(); ++i) //On parcours le vecteur de sommets...
    {
        //Affichage forte connexité
        if(getSommet(i)->getConnexe() > 0)
        {
            rectfill(buffer, getSommets()[i]->getCd_x()-4, getSommets()[i]->getCd_y()-4, getSommets()[i]->getImg()->w + getSommets()[i]->getCd_x()+4, getSommets()[i]->getImg()->h + getSommets()[i]->getCd_y()+4, makecol(100+10*getSommet(i)->getConnexe(), 255-50*getSommet(i)->getConnexe(), 0));
        }
        //Image d'illustration des sommet
        blit(getSommet(i)->getImg(), buffer, 0, 0, getSommet(i)->getCd_x(), getSommet(i)->getCd_y(), getSommet(i)->getImg()->w, getSommet(i)->getImg()->h);
        //Numero Image
        rectfill(buffer, getSommet(i)->getCd_x() + getSommet(i)->getImg()->w - 20, getSommet(i)->getCd_y() + getSommet(i)->getImg()->h - 15, getSommet(i)->getCd_x() + getSommet(i)->getImg()->w, getSommet(i)->getCd_y() + getSommet(i)->getImg()->h, makecol(0,0,0));
        rect(buffer, getSommet(i)->getCd_x() + getSommet(i)->getImg()->w - 20, getSommet(i)->getCd_y() + getSommet(i)->getImg()->h - 15, getSommet(i)->getCd_x() + getSommet(i)->getImg()->w, getSommet(i)->getCd_y() + getSommet(i)->getImg()->h-2, makecol(255,255,255));
        textprintf_ex(buffer, font, getSommet(i)->getCd_x() + getSommet(i)->getImg()->w - 16, getSommet(i)->getCd_y() + getSommet(i)->getImg()->h - 11, makecol(255,255,255), -1,"%d", getSommet(i)->getNum());
        //Poids Image
        rectfill(buffer, getSommet(i)->getCd_x(), getSommet(i)->getCd_y() + getSommet(i)->getImg()->h - 14, getSommet(i)->getCd_x() + 26, getSommet(i)->getCd_y() + getSommet(i)->getImg()->h-1, makecol(200,200,200));
        rect(buffer, getSommet(i)->getCd_x(), getSommet(i)->getCd_y() + getSommet(i)->getImg()->h - 14, getSommet(i)->getCd_x() + 26, getSommet(i)->getCd_y() + getSommet(i)->getImg()->h-1, makecol(255,255,255));
        textprintf_ex(buffer, font, getSommet(i)->getCd_x() +2, getSommet(i)->getCd_y() + getSommet(i)->getImg()->h - 10, makecol(0,0,0), -1, "%d", getSommet(i)->getPoids());



    }
    outils(buffer, barre, a, prev_mouse_b, now_mouse_b);
    draw_sprite(buffer, barre, 0, 600-barre->h);
    blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    clear_bitmap(buffer);
}

///Action de chaque boutons
void Graphe::outils(BITMAP* buffer, BITMAP* barre, int a, int prev_mouse_b, int now_mouse_b)
{
    int cdx = 745, taille = 50, cdy = 60;
    if (a != 1)
    {
        /// Ajouter une arête
        if (is_mouse(cdx, taille, 5, taille)) //Si l'on est sur les coordonnees suivantes ...
        {
            rectfill(buffer, cdx-2, 3, cdx+taille+1, taille+6, makecol(76,201,0));   //On creer un rectangle autour du bouton pour l'indiquer
            if (!prev_mouse_b && now_mouse_b)
            {
                ajouterArete(buffer);   //On lance le sous programme (ici ajout d'arete)
            }
        }

        /// Supprimer un sommet
        if (is_mouse(cdx, taille, cdy+5, taille))  //Pareil pour les boutons suivants
        {
            rectfill(buffer, cdx-2, cdy+3, cdx+taille+1, cdy+taille+6, makecol(255,0,0));
            if (!prev_mouse_b && now_mouse_b)
            {
                supprimerSommet();
            }
        }

        /// Ajouter un sommet
        if (is_mouse(cdx, taille, (2*cdy)+5, taille))
        {
            rectfill(buffer, cdx-2, (2*cdy)+3, cdx+taille+1, (2*cdy)+taille+6, makecol(6,201,0));
            if (!prev_mouse_b && now_mouse_b)
            {
                ajouterSommet(buffer, barre);
            }
        }

        /// Supprimer une arête
        if (is_mouse(cdx, taille, (3*cdy)+5, taille))
        {
            rectfill(buffer, cdx-2, (3*cdy)+3, cdx+taille+1, (3*cdy)+taille+6, makecol(255,0,0));
            if (!prev_mouse_b && now_mouse_b)
            {
                supprimerArete();
            }
        }

        /// Afficher les composantes fortements connexes
        if (is_mouse(cdx, taille, (4*cdy)+5, taille))
        {
            rectfill(buffer, cdx-2, (4*cdy)+3, cdx+taille+1, (4*cdy)+taille+6, makecol(127,0,0));
            if (!prev_mouse_b && now_mouse_b)
            {
                save();
                init();
                afficherCFC();
                setTime(0);
            }
        }

        /// Afficher la K-Connexité
        if (is_mouse(cdx, taille, (6*cdy)+5, taille))
        {
            rectfill(buffer, cdx-2, (6*cdy)+3, cdx+taille+1, (6*cdy)+taille+6, makecol(18,0,124));
            if (!prev_mouse_b && now_mouse_b)
            {
                save();
                init();
                K_Connexites();
            }
        }

        /// Temporalité
        if (is_mouse(cdx, taille, (7*cdy)+5, taille))
        {
            rectfill(buffer, cdx-2, (7*cdy)+3, cdx+taille+1, (7*cdy)+taille+6, makecol(225,0,0));
            if (!prev_mouse_b && now_mouse_b)
            {
                inverserPlay();
                if (getPlay() == true)
                {
                    calc_pop();
                }
            }
        }

        /// Afficher le graphe réduit
        if (is_mouse(cdx, taille, (8*cdy)+5, taille))
        {
            rectfill(buffer, cdx-2, (8*cdy)+3, cdx+taille+1, (8*cdy)+taille+6, makecol(0,148,255));
            if (!prev_mouse_b && now_mouse_b)
            {
                inverserReduit();
                if (getReduit() == true)
                {
                    save();
                    reduit();
                }
                else init();
            }
        }
    }

    // Affichage des boutons
    for (unsigned int i = 0; i < m_boutons.size()-1; ++i)
    {
        if (i < 7)
        {
            blit(getBouton(i), buffer, 0, 0, 745, 5+(60*i), getBouton(i)->w, getBouton(i)->h);
        }
        if (i == 7)
        {
            if (getPlay() == true) blit(getBouton(i), buffer, 0, 0, 745, 5+(60*i), getBouton(i)->w, getBouton(i)->h);
            else blit(getBouton(i+1), buffer, 0, 0, 745, 5+(60*i), getBouton(i+1)->w, getBouton(i+1)->h);
        }
        if (i == 9)
        {
            if (getReduit() == true) blit(getBouton(i), buffer, 0, 0, 745, 5+(60*8), getBouton(i)->w, getBouton(i)->h);
            else blit(getBouton(i+1), buffer, 0, 0, 745, 5+(60*(i-1)), getBouton(i)->w, getBouton(i)->h);
        }
    }

    if(getPlay() == true)   //Bouton play/pause permet de calculer la dynamique des population
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

///Ajoute une arete entre deux sommets du graphe
void Graphe::ajouterArete(BITMAP* buffer)
{
    Arete* a = new Arete;
    Sommet* s = new Sommet;
    int prev_mouse_b, now_mouse_b;
    std::vector<Arete*> tmp = getAretes();
    bool deja_ar = false;

    int stop = 0;   //defini la boucle du sous programme

    while (s->getNomImg() == "" && stop == 0)
    {
        prev_mouse_b = now_mouse_b; //On recupere le clic de la souris
        now_mouse_b = mouse_b&1;
        for (unsigned int i(0); i < getSommets().size(); ++i)   //Parcours du vecteur de sommets ...
        {
            if (is_sommmet(i))  //Test : si la souris est sur le sommet i ...
            {
                if (!prev_mouse_b && now_mouse_b)   //Si on clique ...
                {
                    s = getSommets()[i];
                    a->setDepart(s);    //On relie l'arete au sommet
                }
            }
        }

        annuler(&stop);
    }

    if(stop == 0)
    {
        rect(screen, a->getDepart()->getCd_x()-2, a->getDepart()->getCd_y()-2, a->getDepart()->getImg()->w +a->getDepart()->getCd_x()+1,a->getDepart()->getImg()->h + a->getDepart()->getCd_y()+1, makecol(255,0,0));

        s = new Sommet;

        while (s->getNomImg() == "" && stop == 0)
        {
            prev_mouse_b = now_mouse_b;
            now_mouse_b = mouse_b&1;
            for (unsigned int i(0); i < getSommets().size(); ++i)
            {
                if (is_sommmet(i))
                {
                    if (!prev_mouse_b && now_mouse_b)
                    {
                        if (a->getDepart() != getSommets()[i])
                        {
                            s = getSommets()[i];
                            a->setArrive(s);
                        }
                    }
                }
            }

            annuler(&stop);
        }
    }

    if(stop == 0)
    {
        // Pour chaque arete deja existante, on verifie si elle est equivalente a celle que l'on veut ajouter
        for (unsigned int i = 0; i < getAretes().size(); ++i)
        {
            if (a->getArrive() == getAretes()[i]->getArrive() && a->getDepart() == getAretes()[i]->getDepart())
            {
                std::cout << "Cette arete existe deja!" << std::endl;
                deja_ar = true;
            }
        }
        // Si celle-ci est equivalente, on ne l'ajoute pas
        if (deja_ar == false)
        {
            tmp.push_back(a);
        }
        setAretes(tmp);
    }

}

///ajout de sommets dans le graphe selon un nombre fixe de sommets
void Graphe::ajouterSommet(BITMAP* buffer, BITMAP* barre)
{
    Sommet* s;
    int prev_mouse_b;
    int now_mouse_b;

    int stop(0);

    std::vector<Sommet*> som(getS_Sup()); //Vecteur de sommets supprimes du graphe (mais existant)
    std::vector<Sommet*> som_graph(getSommets()); //Vecteur de ssommets presents dans le graphe

    if (som.size() > 0)
    {
        while (som.size() +1 != getS_Sup().size() && stop == 0)
        {
            prev_mouse_b = now_mouse_b;
            now_mouse_b = mouse_b&1;

            for (unsigned int i(0); i < som.size(); ++i)
            {
                if (som.size() > 0) //Affichage de l'image associe au sommet et du rectangle de selection
                {
                    stretch_blit(som[i]->getImg(), buffer, 0, 0, som[i]->getImg()->w, som[i]->getImg()->h, 680, 60*i+5, som[i]->getImg()->w/2, som[i]->getImg()->h/2);
                    blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
                }
            }

            for (unsigned int i(0); i < som.size(); ++i)
            {
                if (is_mouse(680, som[i]->getImg()->w/2, 60*i+5, som[i]->getImg()->h/2))
                {
                    if (!prev_mouse_b && now_mouse_b)   //Si on selectionne le sommet voulu...
                    {
                        s = new Sommet;
                        s = som[i];
                        som.erase(som.begin()+i);   //On enleve le sommet supprime du vecteur de sommets supprimes
                        som_graph.push_back(s); //On l'ajoute au vecteur de sommets presents dans le graphe
                    }
                }
            }

            blit(getBouton(5), buffer, 0, 0, 745, 5+(60*5), getBouton(5)->w, getBouton(5)->h);
            annuler(&stop);
        }
    }

    if(stop == 0)
    {
        setS_Sup(som);
        setSommets(som_graph);
    }

}

///suppression de sommet du graphe
void Graphe::supprimerSommet()
{
    int prev_mouse_b;
    int now_mouse_b;

    int cancel(0);

    std::vector<Sommet*> temp(getSommets());    //Vecteur de sommets
    std::vector<Sommet*> temp_supp(getS_Sup()); //Vecteur de sommets supprimes
    std::vector<Arete*> temp_ar(getAretes());   //Vecteur d'aretes (pour pouvoir supprimer les arretes associees au sommet!)

    bool stop(false);

    if (temp.size() > 0)
    {
        while (temp.size()+1 != getSommets().size() && cancel == 0) //tant que l'on a pas supprime de sommet...
        {
            prev_mouse_b = now_mouse_b; //on recupere la souris
            now_mouse_b = mouse_b&1;

            for (unsigned int i(0); i < temp.size() ; ++i)  //On parcours le vecteur de sommet presents dans le graphe
            {
                if (i == 0) stop = false;
                if (is_sommmet(i))  //Si on est sur un sommet...
                {
                    if (!prev_mouse_b && now_mouse_b && !stop) //...Et qu'on clique dessus...
                    {
                        for (unsigned int j(0); j < temp_ar.size(); ++j) //On parcours le vecteur d'aretes
                        {
                            if (temp[i] == temp_ar[j]->getDepart() || temp[i] == temp_ar[j]->getArrive()) //Si les aretes du vecteur sont connectes au sommet...
                            {
                                temp_ar.erase(temp_ar.begin() + j);  //...On supprime les aretes
                                --j;
                            }
                        }

                        temp_supp.push_back(temp[i]); //On met le sommet dans le vecteur de sommets supprimes
                        temp.erase(temp.begin()+i); //On supprime le sommet
                        stop = true; //On arete la fonction
                    }
                }
            }

            annuler(&cancel);
        }
    }

    if(cancel == 0)
    {
        setSommets(temp);
        setS_Sup(temp_supp);
        setAretes(temp_ar);
    }
}

///Supprime une arete en selectionnant ses deux sommets
void Graphe::supprimerArete()
{
    Sommet* s1 = new Sommet;
    std::vector<Arete*> temp = getAretes(); //On recupere les aretes
    int prev_mouse_b = 0;
    int now_mouse_b = mouse_b&1;    //Souris
    bool stop(false);

    int cancel(0);

    if (temp.size() > 0)
    {
        // Tant qu'on n'a pas choisi d'image
        while (s1->getNomImg() == "" && cancel == 0)
        {
            prev_mouse_b = now_mouse_b;
            now_mouse_b = mouse_b&1;
            for (unsigned int i(0); i<getSommets().size(); ++i)
            {
                // Si on est sur le sommet et que l'on clique gauche
                if (is_sommmet(i) && !prev_mouse_b && now_mouse_b)
                {
                    for (unsigned int j(0); j<temp.size(); ++j)
                    {
                        // Si le sommet clique i appartient a au moins une arete en tant que sommet de depart
                        if (temp[j]->getDepart() == getSommets()[i])
                        {
                            s1 = getSommets()[i];
                        }
                    }
                }
            }

            annuler(&cancel);
        }


        if(cancel == 0)
        {

            rect(screen, s1->getCd_x()-2, s1->getCd_y()-2, s1->getCd_x() + s1->getImg()->w+1, s1->getCd_y() + s1->getImg()->h+1, makecol(255,12,12));
            rect(screen, s1->getCd_x()-1, s1->getCd_y()-1, s1->getCd_x() + s1->getImg()->w, s1->getCd_y() + s1->getImg()->h, makecol(255,12,12));

            while (temp.size() == getAretes().size() && cancel == 0)
            {
                prev_mouse_b = now_mouse_b;
                now_mouse_b = mouse_b&1;
                for (unsigned int i(0); i < getSommets().size(); ++i)
                {
                    if (is_sommmet(i) && !prev_mouse_b && now_mouse_b)
                    {
                        for (unsigned int j(0); j < temp.size(); ++j)
                        {
                            // Si le sommet clicke i appartient a la meme arete que le sommet de depart
                            if (temp[j]->getArrive() == getSommets()[i] && temp[j]->getDepart() == s1 && !stop)
                            {
                                stop = true;
                                temp.erase(temp.begin() + j); //On supprime l'arete et on arrete le processus de suppression
                                --j;
                            }
                        }
                    }
                }
                annuler(&cancel);
            }
        }

    }

    if(cancel == 0 )
        setAretes(temp);
}

void Graphe::update(BITMAP* buffer, BITMAP* barre, int prev, int now)
{
    int prev_mouse_b = 0;
    int now_mouse_b = mouse_b&1;

    prev_mouse_b = now_mouse_b;
    now_mouse_b = mouse_b&1;

    blit(getDecor(), buffer, 0, 0, 0, 0, getDecor()->w, getDecor()->h); //On affiche le decor sur le buffer (rafraichissement)

    for (int i(getSommets().size()-1); i >= 0 ; --i)
    {
        if (is_sommmet(i))  //Rafraichissment lors du deplacement d'un sommet
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
                    //Blindage de la fenetre
                    if (getSommets()[i]->getCd_x() + getSommets()[i]->getImg()->w > 740)
                        getSommets()[i]->setCd_x(740 - getSommets()[i]->getImg()->w);

                    if (getSommets()[i]->getCd_y() + getSommets()[i]->getImg()->h > SCREEN_H-barre->h)
                        getSommets()[i]->setCd_y(SCREEN_H - barre->h - getSommets()[i]->getImg()->h);

                    if (getSommets()[i]->getCd_x() < 0)
                        getSommets()[i]->setCd_x(0);

                    if (getSommets()[i]->getCd_y() < 0)
                        getSommets()[i]->setCd_y(0);

                    affichage(buffer, barre, 1, prev_mouse_b, now_mouse_b);  //On relance affichage
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

void Graphe::annuler(int* stop)
{
    if(is_mouse(745, 50, 305, 50))
    {
        if(mouse_b&1)
        {
            *stop = 1;
        }
    }
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

/// Play / Pause pour l'affichage du graphe réduit
void Graphe::inverserReduit()
{
     m_reduit = !m_reduit;
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
    std::cout << "Faim K ===>: " << K << std::endl;
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
//            K += getAretes()[i]->getPoids()*getAretes()[i]->getDepart()->getPoids();
            K += getAretes()[i]->getPoids()*getAretes()[i]->getArrive()->getPoids();
        }
    }
    std::cout << "Repas K ===>: " << K << std::endl;
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
                valeur[i] = 500;

            getSommets()[i]->setPoids(valeur[i]); /**< Affectation */
        }
    }

}

void Graphe::afficherCFC()
{
    int nb_recur = 0;
    std::stack<int> Stack;
    std::vector<std::vector<int>> adjCFC;
    bool* visited = new bool[m_ordre];

    for (int i = 0; i < m_ordre; ++i) visited[i] = false;
    for (int i = 0; i < m_ordre; ++i)
    {
        adjCFC.push_back(std::vector<int>());
        if (visited[i] == false) ordreRemplissage(i, visited, Stack);
    }
    Graphe gT = getTranspose();

    for (int i = 0; i < m_ordre; ++i) visited[i] = false;
    while (!Stack.empty())
    {
        int v = Stack.top();
        Stack.pop();
        if (visited[v] == false)
        {
            gT.composanteRecursif(v, visited, nb_recur, adjCFC);
            std::cout << std::endl;
            ++nb_recur;
        }
    }
    std::cout << "Affichage du double vecteur de composantes connexes" << std::endl;
    for (unsigned int i = 0; i< adjCFC.size(); ++i)
    {
        std::cout << "Taille adjCFC[" << i << "] : " << adjCFC.at(i).size() << " --- Sommets connexes : ";
        for (unsigned int j =  0; j < adjCFC.at(i).size(); ++j)
        {
            std::cout << getSommet(adjCFC[i][j])->getNum() << " ";
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
    Graphe g(m_ordre);
    std::cout << "true ordre : " << m_ordre << std::endl;
    for (int v = 0; v < m_ordre; ++v)
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

void Graphe::ordreRemplissage(int v, bool visited[], std::stack<int> &Stack)
{
    visited[v] = true;
    std::list<int>::iterator i;
    for (i = m_adjacences[v].begin(); i != m_adjacences[v].end(); ++i)
    {
        if (!visited[*i]) ordreRemplissage(*i, visited, Stack);
    }
    Stack.push(v);
}

///Modifie le booleen qui affiche la connexite
void Graphe::affichagedesComposantesFortementConnexes(std::vector<std::vector<int> > connexe)
{
    int conn(1);
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
                            if(k == connexe[i][j]) /**< Cherche un sommet qui possede le index meme numero que celui du vector de connexite */
                            {
                                getSommet(k)->setConnexe(conn);
                                std::cout << "conn : " << conn << std::endl;
                            }
                        }
                    }
                }
                conn++;
            }
        }
    }
}

void Graphe::K_Connexites()
{
    resetMarques();
    std::vector<int> s_reliants;
    int compteur = 0, idx_s = 0;
    for (int i = 0; i < m_ordre; ++i)
    {
        if(!getSommet(i)->getMarque())
        {
            if (i != 0) ++compteur;
            getSommet(i)->setMarque(true);
            for (unsigned int j = 0; j < m_adj_adj[i].size(); ++j)
            {
                if (!getSommet(m_adj_adj[i][j])->getMarque())
                {
                    getSommet(m_adj_adj[i][j])->setMarque(true);
                    recursKConnexite(m_adj_adj[i][j], compteur);
                }
            }
        }
    }

    /// Si le graphe n'est pas connexe
    if (compteur != 0)
    {
        std::cout << "Le graphe n'est pas connexe" << std::endl;
        std::cout << "Nombre de composantes (simplement) connexes au moins : "  << compteur+1 << std::endl << std::endl;
    }
    else
    {
        std::cout << "Le graphe est (simplement) connexe au moins" << std::endl << std::endl;
        /// On cherche le sommet avec le moins d'arêtes allant vers lui mais aussi > 0
        for (int i = 0; i < m_ordre-1; ++i)
        {
            if (getSommet(idx_s)->getDegre() >= getSommet(i+1)->getDegre()) idx_s = i+1;
        }

        /// On retrouve l'indice du/des sommet(s) reliant le sommet de degré minimal a partir de leur numéro
        for (unsigned int i = 0; i < getAretes().size(); ++i)
        {
            if (getArete(i)->getArrive()->getNum() == getSommet(idx_s)->getNum())
            {
                for (int j = 0; j < m_ordre; ++j)
                {
                    if (getArete(i)->getDepart()->getNum() == getSommet(j)->getNum())
                    {
                        s_reliants.push_back(j);
                    }
                }
            }

            if (getArete(i)->getDepart()->getNum() == getSommet(idx_s)->getNum())
            {
                for (int j = 0; j < m_ordre; ++j)
                {
                    if (getArete(i)->getArrive()->getNum() == getSommet(j)->getNum())
                    {
                        s_reliants.push_back(j);
                    }
                }
            }

        }
        /// Pour chaque sommet reliant se sommet avec le moins d'arêtes allant vers lui
        /// K-Connexité sommets
        std::cout << "K-Connexite Sommets :" << std::endl;
        for (unsigned int i = 0; i < s_reliants.size(); ++i)
        {
            if (i == 0) std::cout << "Si vous supprimez ";
            std::cout << "le sommet " <<getSommet(s_reliants[i])->getNum() << std::endl;
            if (i != s_reliants.size()-1) std::cout << "puis ";
            else std::cout << "alors le graphe ne sera plus connexe" << std::endl << std::endl;
        }
        /// K-Connexité arêtes
        std::cout << "K-Connexite Aretes :" << std::endl;
        for (unsigned int i = 0; i < s_reliants.size(); ++i)
        {
            if (i == 0) std::cout << "Si vous supprimez ";
            std::cout << "l'arete reliant le sommet " << getSommet(idx_s)->getNum();
            std::cout << " et " << getSommet(s_reliants[i])->getNum() << std::endl;
            if (i != s_reliants.size()-1) std::cout << "puis " << std::endl;
            else std::cout << "alors le graphe ne sera plus connexe" << std::endl;
        }
    }
}

void Graphe::recursKConnexite(int indice, int& compteur)
{
    for (unsigned int k = 0; k < m_adj_adj[indice].size(); ++k)
    {
        if (!getSommet(m_adj_adj[indice][k])->getMarque())
        {
            getSommet(m_adj_adj[indice][k])->setMarque(true);
            recursKConnexite(m_adj_adj[indice][k], compteur);
        }
    }
}

/// On initialise les valeurs des matrices d'adjacences
/// L'une d'elles ne prends pas en compte l'orientation des arcs (m_adj_adj)
void Graphe::initAdjAdj()
{
    m_adjacences->clear();
    m_adj_adj.clear();
    m_adjacences = new std::list<int>[m_ordre];
    std::vector<int> v_test;
    int a, b;
    for (int i = 0; i < m_ordre; ++i) m_adj_adj.push_back(v_test);

    for (unsigned int i = 0; i < getAretes().size(); ++i)
    {
        for (int j = 0; j < m_ordre; ++j)
        {
            /// On retrouve l'indice du sommet de départ dans le vecteur m_sommets à partir de son numéro
            if (getArete(i)->getDepart()->getNum() == getSommet(j)->getNum()) a = j;
            /// On retrouve l'indice du sommet d'arrivée dans le vecteur m_sommets à partir de son numéro
            if (getArete(i)->getArrive()->getNum() == getSommet(j)->getNum()) b = j;
        }
        m_adj_adj[a].push_back(b);
        m_adj_adj[b].push_back(a);
        m_adjacences[a].push_back(b);
    }
    for (int i = 0; i < m_ordre; ++i) getSommet(i)->setDegre(m_adj_adj[i].size());
}

void Graphe::reduit()
{
    std::vector<Sommet*> som(getS_Sup()); //Vecteur de sommets supprimes du graphe (mais existant)
    std::vector<Sommet*> som_graph(getSommets()); //Vecteur de ssommets presents dans le graphe
    std::vector<Arete*> aret(getAretes());

    for(unsigned int i(0); i < som_graph.size(); ++i)
    {
        for(unsigned int j(0); j < som_graph.size(); ++j)
        {
            if(som_graph[i]->getConnexe() == som_graph[j]->getConnexe())
            {
                if( som_graph[j]->getConnexe() > 0 && som_graph[i] != som_graph[j])
                {
                    std::cout << "Sommet i : " << som_graph[i]->getNum() << "  Sommet j : " << som_graph[j]->getNum() << std::endl;
                    for(unsigned int k(0); k < getAretes().size(); ++k)
                    {
                        if(som_graph[j] == getArete(k)->getDepart())
                        {
                            getArete(k)->setDepart(som_graph[i]);
                        }

                        if(som_graph[j] == getArete(k)->getArrive())
                        {
                            getArete(k)->setArrive(som_graph[i]);
                        }
                    }

                    som.push_back(som_graph[j]);
                    som_graph.erase(som_graph.begin()+j);
                    j=0;
                }

            }
        }
    }

    setS_Sup(som);
    setSommets(som_graph);

    for(unsigned int i(0); i < som.size(); ++i)
    {
        for(unsigned int k(0); k < aret.size(); ++k)
        {
            if(aret[k]->getArrive() == aret[k]->getDepart() || som[i] == aret[k]->getDepart() || som[i] == aret[k]->getArrive())
            {
                aret.erase(aret.begin()+k);
                k=0;
            }
        }
    }
    setAretes(aret);
}
