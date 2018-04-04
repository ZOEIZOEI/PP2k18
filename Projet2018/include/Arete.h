#ifndef ARETE_H
#define ARETE_H

#include "Sommet.h"

class Arete
{
    public:
        Arete();
        ~Arete();

        int Getpoids() { return m_poids; }
        Sommet* Getdepart() { return m_depart; }
        Sommet* Getarrive() { return m_arrive; }

        void Setdepart(Sommet* val) { m_depart = val; }
        void Setpoids(int val) { m_poids = val; }
        void Setarrive(Sommet* val) { m_arrive = val; }

    private:
        int m_poids;
        Sommet* m_depart;
        Sommet* m_arrive;
};

#endif // ARETE_H
