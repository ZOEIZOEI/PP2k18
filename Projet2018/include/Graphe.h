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

/** \brief Verifie les coordonnees de la souris selon ses parametres
 *  Renvoie true si la souris est situee dans les coordonnees en parametre
 * \param x int : x de depart de la zone
 * \param weight int : largeur de la zone
 * \param Y int
 * \param height int : hauteur de la zone
 * \return bool : true si la condition est verifiee
 *
 */
bool is_mouse(int x, int weight, int Y, int height);

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

        /** \brief initialisation des vecteurs Sommets et Aretes
         *  Cette fonction initialise l'ordre et le temps a 0, creer des vecteurs de sommets et d'aretes vides fait un setArete() et setSommets().
         * Utilise Recuperation() pour recuperer le fichier de sauvegarde et initialiser les sommets et aretes
         * \return void
         *
         */
        void init();
        void augmentationTime() { m_time++; }   /**< \brief  */

        /** \brief ajoute un bouton a l'interface
         *
         * \param val
         * \return void ajouterBouton(BITMAP* val){
         *
         */
        void ajouterBouton(BITMAP* val) { m_boutons.push_back(val); }

        void inverserPlay();

        /** \brief Initialisation des sommets selon le fichier de sauvegarde
         *
         * \param nom_fichier std::string : image associée au sommet a recuperer
         * \return void
         *
         */
        void initSommets(std::string nom_fichier);

        /** \brief affichage des elements graphiques sur le buffer
         *
         * \param buffer BITMAP*
         * \param barre BITMAP* : barres d'affichage des graphes (graphe 1, graphe 2, graphe 3)
         * \param a int : blindage outils
         * \param prev_mouse_b int : coordonnees de la souris a l'instant t-1
         * \param now_mouse_b int : coordonnees de la souris a l'instant t
         * \return void
         *
         */
        void affichage(BITMAP* buffer, BITMAP* barre, int a, int prev_mouse_b, int now_mouse_b);

        /** \brief interaction des boutons sur l'ecran apres un clic
         *  memes parametres que le sous programme d'affichage
         * \param buffer BITMAP*
         * \param barre BITMAP*
         * \param a int
         * \param prev_mouse_b int
         * \param now_mouse_b int
         * \return void
         *
         */
        void outils(BITMAP* buffer, BITMAP* barre, int a, int prev_mouse_b, int now_mouse_b);

        /** \brief mise a jour de l'ecran apres une action
         *
         * \param buffer BITMAP*
         * \param barre BITMAP*
         * \param prev int : avant l'action
         * \param now int : pendant l'action
         * \return void
         *
         */
        void update(BITMAP* buffer, BITMAP* barre, int prev, int now);

        /** \brief ligne avec epaisseur variable selon le parametre saisi
         *
         * \param buffer BITMAP*
         * \param xDep int
         * \param yDep int
         * \param xArr int
         * \param yArr int
         * \param epaisseur int
         * \param col int : couleur du trait
         * \return void : renvoi un trait sur la bitmap selon les coordonnees et l'epaisseur
         *
         */
        void thick_line(BITMAP *buffer, int xDep, int yDep, int xArr, int yArr, int epaisseur, int col);

        /** \brief Verifie si les coordonnees de la souris sont similaires aux coordonnees d'un sommet i
         *  Fonction similaire a is_mouse
         * \param i int : indice du sommet selectionne
         * \return bool : true si la souris est bien dans la zone du sommet i
         *
         */
        bool is_sommmet(int i);

        /** \brief Ajoute un sommet dans le vecteur de Sommet* pour l'afficher a l'ecran
         *
         * \param buffer BITMAP*
         * \param barre BITMAP*
         * \return void
         *
         */
        void ajouterSommet(BITMAP* buffer, BITMAP* barre);

        /** \brief Ajoute une arete dans le vecteur d'Arete* pour l'afficher a l'ecran
         *
         * \param buffer BITMAP*
         * \return void
         *
         */
        void ajouterArete(BITMAP* buffer);

        /** \brief Supprime un sommet du vecteur de Sommet*
         *
         * \return void
         *
         */
        void supprimerSommet();

        /** \brief Supprime une arete du vecteur d'Arete*
         *
         * \return void
         *
         */
        void supprimerArete();

        /** \brief Charge le fichier de sauvegarde
         *  Lis le fichier selon l'ordre et le nombre d'aretes pour remplir et afficher les sommets et aretes
         * \return void
         *
         */
        void recuperation();

        /** \brief Sauvegarde les sommets et aretes dans un fichier
         *
         * \return void
         *
         */
        void save();

        void composanteRecursif(int v, bool visited[], int nb_recur, std::vector<std::vector<int>>& m_adjCFC);

        /** \brief Renvoi un graphe transposé pour la forte connexite
         *
         * \return Graphe
         *
         */
        Graphe getTranspose();
        void ordreRemplissage(int v, bool visited[], std::list<int> &Stack);
        void afficherCFC();
        /** \brief Set toutes les composantes fortements connexes pour les afficher
         *
         * \param connexe std::vector<std::vector<int> > : Matrice de forte connexité
         * \return void
         *
         */
        void affichagedesComposantesFortementConnexes(std::vector<std::vector<int> > connexe);

        /** \brief Permet de modifier le poids d'un sommet
         *  en utilisant les fleches du clavier et lorsque la souris est sur le sommets
         * \return void
         *
         */
        void slider();

        /** \brief Permet de modifier le poids d'une arete
         *  lorsque la souris est sur le cercle d'affichage du poids
         * \return void
         *
         */
        void sliderArete();

        /** \brief facteur selon les poids aretes/sommets : qui mange qui ?
         * Multiplie le poids des aretes reliees au sommet s par son propre poids
         * \param s Sommet* : sommet selectionne
         * \return float
         *
         */
        float Mange(Sommet* s);
        /** \brief facteur selon les poids aretes/sommets : qui est mange par qui ?
         *  Multiplie le poids des aretes reliees au sommet s par les poids des sommets connectes a celui ci
         * \param s Sommet*
         * \return float
         *
         */
        float Plat(Sommet* s);
        /** \brief calcul de dynamique de population
         *  Calcul final de dynamique des population
         * \return void
         *
         */
        void calc_pop();

    private:
        std::vector<Sommet*> m_sommets; /**< Vecteur de sommets du graphe*/
        std::vector<Sommet*> m_s_sup; /**< Vecteur de sommets pouvant etre ajoutés au graphe (ayant ete supprimes) */
        int m_ordre; /**< Nombre de sommet du graphe */
        std::vector<Arete*> m_aretes; /**< Vecteur d'arete du grahe */
        std::string m_nom_graphe; /**< pour acceder au document de sauvegarde */
        BITMAP* m_decor; /**< Bitmap de fond */
        std::vector<BITMAP*> m_boutons;
        std::list<int> *m_adjacences; /**< liste d'adjacence */
        bool m_play; /**< lance la dynamique en temps reel */
        int m_time; /**< Dynamique en temps reel  */
        int m_nb_s_sup; /**< Ordre des sommets supprimes */
};

#endif // GRAPHE_H
