#include <QString>
#include <QVector>
#include <iostream>
#include "systeme.h"

using namespace std;

Systeme::Systeme(int equation, int inconnue)
{
    this->nbrEquation = equation;
    this->nbrInconnue = inconnue;

    this->coefficients = new Matrice(nbrEquation, nbrInconnue);
    this->egalites = new Matrice(nbrEquation, 1);

    this->multiple = 1;
}

void Systeme::resoudre()
{
    solution = QVector<double>(nbrInconnue);

    double detCoefficients = coefficients->determinant();
    bool besoinMultiple = false;

    for(int i=0; i<nbrEquation; i++)
    {
        Matrice *temp = new Matrice(*coefficients);
        QVector<double> temp2 = QVector<double>(nbrInconnue);

        for(int j=0; j<nbrEquation; j++)
        {
            temp2[j] = coefficients->getCoef(j,i);
            temp->remplir(j,i,egalites->getCoef(j,0));
        }

        double determinant = temp->determinant();

        for(int j=0; j<nbrEquation; j++)
        {
            coefficients->remplir(j,i,temp2[j]);
        }

        solution[i] = determinant/detCoefficients;

        if((int)solution[i] != solution[i])
        {
            besoinMultiple = true;
        }

        delete temp;
    }

    if(besoinMultiple)
    {
        for(int i=0; i<solution.length(); i++)
        {
            solution[i] = solution[i]*detCoefficients;
        }

        multiple = detCoefficients;
    }
}

void Systeme::setCoef(int a, int b, double coefficient)
{
    coefficients->remplir(a,b,coefficient);
}

Matrice* Systeme::getCoef()
{
    return coefficients;
}

void Systeme::setEgalites(int a, double coefficient)
{
    egalites->remplir(a,0,coefficient);
}

Matrice* Systeme::getEgalites()
{
    return egalites;
}

double Systeme::getMultiple()
{
    return multiple;
}

QVector<double> Systeme::getSolution()
{
    try
    {
        resoudre();
        return solution;
    }
    catch(string const& e)
    {
        throw string("Systeme impossible");
    }
}
