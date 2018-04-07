#ifndef GRAPHE_H
#define GRAPHE_H

#include "Sommet.h"
#include "Arete.h"
#include <vector>
#include <list>
#include <stack>
#include <fstream>
#include <iostream>
#include <allegro.h>
#include <png.h>
#include <loadpng.h>
#include <jpgalleg.h>
#include <ctime>
#include <stack>
#include <list>

class Graphe
{
    public:
        Graphe(std::string nom_fichier, std::string nom_decor);
        Graphe(int ordre) { m_ordre = ordre; m_adjacences = new std::list<int>[m_ordre + m_nb_s_sup]; }
        ~Graphe();

        std::vector<Sommet*> getSommets() { return m_sommets; } /// POUR TOUT LE VECTEUR
        std::vector<Arete*> getAretes() { return m_aretes; } /// POUR TOUT LE VECTEUR
        std::vector<Sommet*> getS_Sup() { return m_s_sup; }
        Sommet* getSommet(int val) { return m_sommets[val]; } /// POUR UN SEUL SOMMET
        Arete* getArete(int val) { return m_aretes[val]; } /// POUR UNE SEULE ARETE

        int getOrdre() { return m_ordre; }
        std::string getNomGraphe() { return m_nom_graphe; }
        BITMAP* getDecor() {return m_decor; }
        BITMAP* getBouton(int val) { return m_boutons[val]; }
        bool getPlay() { return m_play; }
        int getTime() { return m_time; }

        void setSommets(std::vector<Sommet*> val) { m_sommets = val; }
        void setAretes(std::vector<Arete*> val) { m_aretes = val; }
        void setS_Sup(std::vector<Sommet*> val) { m_s_sup = val; }

        void setOrdre(int val) { m_ordre = val; }
        void setNomGraphe(std::string val) { m_nom_graphe = val; }
        void setDecor(BITMAP* val) { m_decor = val; }
        void setTime(int val) { m_time = val; }

        void init();
        void augmentationTime() { m_time++; }

        void ajouterBouton(BITMAP* val) { m_boutons.push_back(val); }
        void inverserPlay();

        void initSommets(std::string nom_fichier);

        void affichage(BITMAP* buffer, BITMAP* barre, int a, int prev_mouse_b, int now_mouse_b);
        void outils(BITMAP* buffer, BITMAP* barre, int a, int prev_mouse_b, int now_mouse_b);
        void update(BITMAP* buffer, BITMAP* barre, int prev, int now);
        void thick_line(BITMAP *buffer, int xDep, int yDep, int xArr, int yArr, int epaisseur, int col);

        bool is_mouse(int x, int weight, int Y, int height);
        bool is_sommmet(int i);
        bool is_areteD(Sommet* s);
        bool is_areteA(Sommet* s);

        void ajouterSommet(BITMAP* buffer, BITMAP* barre);
        void ajouterArete(BITMAP* buffer);
        void supprimerSommet();
        void supprimerArete();

        void recuperation();
        void save();

        void composanteRecursif(int v, bool visited[], int nb_recur, std::vector<std::vector<int>>& m_adjCFC);
        Graphe getTranspose();
        void ordreRemplissage(int v, bool visited[], std::list<int> &Stack);
        void afficherCFC();
        void affichagedesComposantesFortementConnexes(std::vector<std::vector<int> > connexe);

        void slider();
        void sliderArete();

        float Mange(Sommet* s);
        float Plat(Sommet* s);
        void calc_pop();

    private:
        std::vector<Sommet*> m_sommets;
        std::vector<Sommet*> m_s_sup;
        int m_ordre;
        std::vector<Arete*> m_aretes;
        std::string m_nom_graphe;
        BITMAP* m_decor;
        std::vector<BITMAP*> m_boutons;
        std::list<int> *m_adjacences;
        bool m_play;
        int m_time;
        int m_nb_s_sup;
};

#endif // GRAPHE_H
