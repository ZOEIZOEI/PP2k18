#ifndef SOMMET_H
#define SOMMET_H
#include <allegro.h>
#include <string>

class Sommet
{
    public:
        Sommet();
        Sommet(int cd_x, int cd_y, int num, int poids, std::string nom_img);
        ~Sommet();

        int GetCd_x() { return m_cd_x; }
        int GetCd_y() { return m_cd_y; }
        int GetNum() { return m_num; }
        int GetPoids() { return m_poids; }
        std::string getNomImg() { return m_nom_img; }
        BITMAP* GetImg() { return m_img; }

        void SetCd_x(int val) { m_cd_x = val; }
        void SetCd_y(int val) { m_cd_y = val; }
        void SetNum(int val) { m_num = val; }
        void SetPoids(int val) { m_poids = val; }
        void setNomImg(std::string val) { m_nom_img = val; }
        void SetImg(BITMAP* val) { m_img = val; }

    private:
        int m_cd_x;
        int m_cd_y;
        int m_num;
        int m_poids;
        std::string m_nom_img;
        BITMAP* m_img;
};

#endif // SOMMET_H
