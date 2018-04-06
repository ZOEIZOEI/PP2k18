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
        ~Graphe();

        std::vector<Sommet*> getSommets() { return m_sommets; } /// POUR TOUT LE VECTEUR
        std::vector<Arete*> getAretes() { return m_aretes; }
        std::vector<Sommet*> getS_Sup() { return m_s_sup; }
        Sommet* getSommet(int val) { return m_sommets[val]; } /// POUR UN SEUL SOMMET

        int getOrdre() { return m_ordre; }
        std::string getNomGraphe() { return m_nom_graphe; }
        BITMAP* getDecor() {return m_decor; }
        BITMAP* getBouton(int val) { return m_boutons[val]; }

        void setSommets(std::vector<Sommet*> val) { m_sommets = val; }
        void setAretes(std::vector<Arete*> val) { m_aretes = val; }
        void setS_Sup(std::vector<Sommet*> val) { m_s_sup = val; }

        void setOrdre(int val) { m_ordre = val; }
        void setNomGraphe(std::string val) { m_nom_graphe = val; }
        void setDecor (BITMAP* val) { m_decor = val; }

        void ajouterBouton(BITMAP* val) { m_boutons.push_back(val); }

        void initSommets(std::string nom_fichier);

        void affichage(BITMAP* buffer, BITMAP* barre, int a);
        void outils(BITMAP* buffer, BITMAP* barre, int a);
        void update(BITMAP* buffer, BITMAP* barre);

        bool is_mouse(int x, int weight, int Y, int height);
        bool is_sommmet(int i);

        void ajouterSommet(BITMAP* buffer, BITMAP* barre);
        void ajouterArete(BITMAP* buffer);
        void supprimerSommet();
        void supprimerArete();

        void recuperation();
        void save();

        void CFC();
        void composanteRecursif(int u, int disc[], int low[], std::stack<int> *st, bool stackMember[]);

        void slider();
        void sliderArete();

    private:
        std::vector<Sommet*> m_sommets;
        std::vector<Sommet*> m_s_sup;
        int m_ordre;
        std::vector<Arete*> m_aretes;
        std::string m_nom_graphe;
        BITMAP* m_decor;
        std::vector<BITMAP*> m_boutons;
        std::list<int> *m_adjacences;
};

#endif // GRAPHE_H
