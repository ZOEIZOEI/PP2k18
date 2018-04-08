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
//        bool getConnexe() { return m_connexe; }
        int getConnexe() { return m_connexe; }
        int getDegre() { return m_degre; }
        bool getMarque() { return m_marque; }

        void setCd_x(int val) { m_cd_x = val; }
        void setCd_y(int val) { m_cd_y = val; }
        void setNum(int val) { m_num = val; }
        void setPoids(int val) { m_poids = val; }
        void setNomImg(std::string val) { m_nom_img = val; }
        void setImg(BITMAP* val) { m_img = val; }
//        void setConnexe(bool val) { m_connexe = val; }
        void setConnexe(int val) { m_connexe = val; }
        void setDegre(int val) { m_degre = val; }
        void setMarque(bool val) { m_marque = val; }

        /** \brief Permet d'augmenter/reduire le poids du Sommet
         *
         * \param val int : valeur du poids à ajouter
         * \return void
         *
         */
        void ajouterpoids(int val) { m_poids += val; }

    private:
        int m_cd_x; /**< Position en x */
        int m_cd_y; /**< Position en y */
        int m_num; /**< Numero de l'image */
        int m_poids; /**< Poids du Sommet */
        std::string m_nom_img; /**< Adresse de l'image du Sommet */
        BITMAP* m_img; /**< Image du Sommet */
//        bool m_connexe; /**< Permet de savoir si le sommet est fortement connexe */
        int m_connexe; /**< Permet de savoir si le sommet est fortement connexe */
        int m_degre; /**< Le degré du sommet */
        bool m_marque;
};

#endif // SOMMET_H
