#include "Sommet.h"

Sommet::Sommet()
{
    m_cd_x = 0;
    m_cd_y = 0;
    m_num = 0;
    m_poids = 0;
}

Sommet::Sommet(int cd_x, int cd_y, int num, int poids, std::string nom_img)
{
    m_cd_x = cd_x;
    m_cd_y = cd_y;
    m_num = num;
    m_poids = poids;
    m_nom_img = nom_img;
    m_img = load_bitmap(m_nom_img.c_str(), NULL);
}

Sommet::~Sommet()
{
    //dtor
}

