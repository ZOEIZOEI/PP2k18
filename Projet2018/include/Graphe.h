#ifndef GRAPHE_H
#define GRAPHE_H

#include "Sommet.h"
#include "Arete.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <allegro.h>
#include <png.h>
#include <loadpng.h>
#include <jpgalleg.h>
#include <ctime>

class Graphe
{
    public:
        Graphe(std::string nom_fichier, std::string nom_decor);
        ~Graphe();

        std::vector<Sommet*> Getsommets() { return m_sommets; } /// POUR TOUT LE VECTEUR
        int Getordre() { return m_ordre; }
        std::vector<Arete*> Getaretes() { return m_aretes; }
        std::vector<Sommet*> Getsall() { return m_sall; }
        std::string GetNomGraphe() { return m_nom_graphe; }
        BITMAP* Getdecor() {return m_decor; }
        BITMAP* getBouton(int val) { return m_boutons[val]; }

        void Setaretes(std::vector<Arete*> val) { m_aretes = val; }
        void Setsommets(std::vector<Sommet*> val) { m_sommets = val; }
        void Setsall(std::vector<Sommet*> val) { m_sall = val; }
        void Setordre(int val) { m_ordre = val; }
        void SetNomGraphe(std::string val) { m_nom_graphe = val; }
        void Setdecor (BITMAP* val) { m_decor = val; }

        void ajouterBouton(BITMAP* val) { m_boutons.push_back(val); }

        void init_sommets(std::string nom_fichier);

        void ajouterSommet(Sommet* val) { m_sommets.push_back(val); }
        Sommet* getSommet(int val) { return m_sommets[val]; } /// POUR UN SEUL SOMMET

        void affichage(BITMAP* buffer, BITMAP* barre, int a);
        void outils(BITMAP* buffer, BITMAP* barre, int a);

        void update(BITMAP* buffer, BITMAP* barre);

        bool is_mouse(int x, int weight, int Y, int height);
        bool is_sommmet(int i);

        void ajouterArete(BITMAP* buffer);
        void ajouSommet(BITMAP* buffer, BITMAP* barre);

        void suppSommet();
        void suppArete();

        void Recuperation();
        void save();

    private:
        std::vector<Sommet*> m_sommets;
        std::vector<Sommet*> m_sall;
        int m_ordre;
        std::vector<Arete*> m_aretes;
        std::string m_nom_graphe;
        BITMAP* m_decor;
        std::vector<BITMAP*> m_boutons;
};

#endif // GRAPHE_H
