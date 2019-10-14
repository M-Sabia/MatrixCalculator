#ifndef MATRICE_H
#define MATRICE_H

class Matrice
{
    public:
        Matrice();
        Matrice(int,int);
        Matrice(Matrice &copie);

        void retirerLigne();
        void retirerColonne();
        void ajouterLigne();
        void ajouterColonne();

        void remplir(int numligne, int numcolonne, double coefficients);
        Matrice *somme(Matrice *source);
        Matrice* produit(Matrice *source);
        Matrice* produitReel(double reel);
        double determinant();
        double determinant2x2();
        double determinant3x3();
        Matrice matSansLigCol(int ligne,int colonne);
        Matrice* comatrice();
        Matrice* inverser();
        Matrice* transposer();
        void arrondir();

        int getLigne();
        void setLigne(int ligne);
        int getColonne();
        void setColonne(int colonne);
        QVector<QVector<double> > getcoef();
        double getCoef(int ligne, int colonne);
        void setCoefficients(QVector<QVector<double> > coefficients);
        double getMultiple();
        void setMultiple(double multiple);

    private:
        int ligne;
        int colonne;
        QVector<QVector<double> > coefficients;
        double multiple;
};

#endif // MATRICE_H
