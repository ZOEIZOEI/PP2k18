#ifndef ARETE_H
#define ARETE_H

#include "Sommet.h"

class Arete
{
    public:
        Arete();
        ~Arete();

        int getPoids() { return m_poids; }
        Sommet* getDepart() { return m_depart; }
        Sommet* getArrive() { return m_arrive; }

        void setPoids(int val) { m_poids = val; }
        void setDepart(Sommet* val) { m_depart = val; }
        void setArrive(Sommet* val) { m_arrive = val; }

    private:
        int m_poids; /**< Poids de l'arête */
        Sommet* m_depart; /**< Sommet de départ (Mangeur) */
        Sommet* m_arrive; /**< Sommet d'arrivé (Mangé) */
};

#endif // ARETE_H
