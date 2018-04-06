#include "Sommet.h"

Sommet::Sommet()
{
    m_cd_x = 0;
    m_cd_y = 0;
    m_num = 0;
    m_poids = 0;
    m_connexe = false;
}

Sommet::Sommet(int cd_x, int cd_y, int num, int poids, std::string nom_img)
{
    m_cd_x = cd_x;
    m_cd_y = cd_y;
    m_num = num;
    m_poids = poids;
    m_nom_img = nom_img;
    m_img = load_bitmap(m_nom_img.c_str(), NULL);
    if(!m_img)
    {
        allegro_message("pas pu trouver m_nom_img.png");
        exit(EXIT_FAILURE);
    }
}

Sommet::~Sommet()
{
    //dtor
}

