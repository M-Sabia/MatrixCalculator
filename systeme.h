#ifndef SYSTEME_H
#define SYSTEME_H

#include "matrice.h"

class Systeme
{
    public:
        Systeme(int equation, int inconnue);

        void resoudre();
        void setCoef(int a, int b, double coefficient);
        Matrice* getCoef();
        void setEgalites(int a, double coefficient);
        Matrice* getEgalites();
        double getMultiple();
        QVector<double> getSolution();

    private:
        Matrice *coefficients;
        Matrice *egalites;
        int nbrEquation;
        int nbrInconnue;
        QVector<double> solution;
        double multiple;
};

#endif // SYSTEME_H
