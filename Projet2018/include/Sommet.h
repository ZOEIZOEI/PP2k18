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

        int getCd_x() { return m_cd_x; }
        int getCd_y() { return m_cd_y; }
        int getNum() { return m_num; }
        int getPoids() { return m_poids; }
        std::string getNomImg() { return m_nom_img; }
        BITMAP* getImg() { return m_img; }
        bool getConnexe() { return m_connexe; }

        void setCd_x(int val) { m_cd_x = val; }
        void setCd_y(int val) { m_cd_y = val; }
        void setNum(int val) { m_num = val; }
        void setPoids(int val) { m_poids = val; }
        void setNomImg(std::string val) { m_nom_img = val; }
        void setImg(BITMAP* val) { m_img = val; }
        void setConnexe(bool val) { m_connexe = val; }

        void ajouterpoids(int val) { m_poids += val; }

    private:
        int m_cd_x;
        int m_cd_y;
        int m_num;
        int m_poids;
        std::string m_nom_img;
        BITMAP* m_img;
        bool m_connexe;
};

#endif // SOMMET_H
