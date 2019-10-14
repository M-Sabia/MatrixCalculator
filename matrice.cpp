#include <QString>
#include <QVector>
#include <iostream>
#include <cmath>
#include <QDebug>
#include <QtGlobal>
#include "matrice.h"

Matrice::Matrice()
{

}

Matrice::Matrice(int ligne, int colonne)
{
    this->ligne = ligne;
    this->colonne = colonne;
    this->multiple = 1;

    for(int i=0; i<ligne; i++)
    {
        this->coefficients.append(QVector<double>());

        for(int j=0; j<colonne; j++)
            this->coefficients[i].append(0);
    }
}

Matrice::Matrice(Matrice &copie)
{
    this->ligne = copie.getLigne();
    this->colonne = copie.getColonne();
    this->multiple = copie.getMultiple();

    for(int i=0; i<ligne; i++)
    {
        this->coefficients.append(QVector<double>());

        for(int j=0; j<colonne; j++)
            this->coefficients[i].append(0);
    }

    for(int i=0; i<this->getLigne(); i++)
    {
        for(int j=0; j<this->getColonne(); j++)
        {
            this->remplir(i,j,copie.getCoef(i,j));
        }
    }
}

void Matrice::retirerLigne()
{
    this->coefficients.removeLast();
    this->ligne--;
}

void Matrice::retirerColonne()
{
    for(int i=0; i<this->coefficients.count(); ++i)
        this->coefficients[i].removeLast();

    this->colonne--;
}

void Matrice::ajouterLigne()
{
    this->coefficients.append(QVector<double>());

    for(int i=0; i<colonne; i++)
        this->coefficients[this->coefficients.size()-1].append(0);

    this->ligne++;
}

void Matrice::ajouterColonne()
{
    for(int i=0; i<this->coefficients.count(); ++i)
        this->coefficients[i].append(0);

    this->colonne++;
}

void Matrice::remplir(int numligne, int numcolonne, double coefficients)
{
    this->coefficients[numligne][numcolonne] = coefficients;
}

Matrice* Matrice::somme(Matrice *source)
{
    if(this->getLigne() != source->getLigne() || this->getColonne() != source->getColonne())
        return nullptr;

    Matrice *newMatrice = new Matrice(this->getLigne(),this->getColonne());

    for(int i=0; i<this->getLigne(); i++)
    {
        for(int j=0; j<this->getColonne(); j++)
        {
            newMatrice->remplir(i,j,((this->coefficients[i][j]/this->getMultiple()) + (source->getCoef(i,j)/source->getMultiple())));
        }
    }

    return newMatrice;
}

Matrice* Matrice::produit(Matrice *source)
{
    if(this->getColonne() != source->getLigne())
        return nullptr;

    Matrice *newMatrice = new Matrice(this->getLigne(),source->getColonne());

    for(int i=0; i<newMatrice->getLigne(); i++)
    {
        for(int j=0; j<newMatrice->getColonne(); j++)
        {
            double coefTemp=0;

            for(int k=0; k<this->getColonne(); k++)
            {
                coefTemp += this->coefficients[i][k]*source->getCoef(k,j);
            }

            newMatrice->remplir(i,j,coefTemp);
        }
    }

    newMatrice->setMultiple(this->getMultiple()*source->getMultiple());

    newMatrice->arrondir();

    return newMatrice;
}

Matrice* Matrice::produitReel(double reel)
{
    Matrice *newMatrice = new Matrice(this->getLigne(),this->getColonne());

    double reel2 = reel/this->getMultiple();

    for(int i=0; i<this->getLigne(); i++)
    {
        for(int j=0; j<this->getColonne(); j++)
        {
            newMatrice->remplir(i,j,(reel2*this->coefficients[i][j]));
        }
    }

    return newMatrice;
}

double Matrice::determinant()
{
    this->setMultiple(1);

    if(this->getLigne() != this->getColonne())
        return 0;

    if(this->getLigne() == 1)
        return coefficients[0][0]*multiple;

    if(this->getLigne() == 2)
        return determinant2x2()*multiple;

    if(this->getLigne() == 3)
        return determinant3x3()*multiple;

    double determinant = 0;

    for(int i=0; i<this->getColonne(); i++)
    {
        if(i%2==0){
            determinant += this->coefficients[0][i]*this->matSansLigCol(0,i).determinant();
        }
        else{
            determinant -= this->coefficients[0][i]*this->matSansLigCol(0,i).determinant();
        }
    }

    return determinant;
}

double Matrice::determinant2x2()
{
    if(this->getLigne()!=2 || this->getColonne()!=2)
        return 0;

    return coefficients[0][0]*coefficients[1][1]-coefficients[0][1]*coefficients[1][0];
}

double Matrice::determinant3x3()
{
    if(this->getLigne()!=3 || this->getColonne()!=3)
        return 0;

    double determinant = coefficients[0][0]*coefficients[1][1]*coefficients[2][2];
    determinant += coefficients[0][1]*coefficients[1][2]*coefficients[2][0];
    determinant += coefficients[0][2]*coefficients[1][0]*coefficients[2][1];
    determinant -= coefficients[2][0]*coefficients[1][1]*coefficients[0][2];
    determinant -= coefficients[1][0]*coefficients[0][1]*coefficients[2][2];
    determinant -= coefficients[0][0]*coefficients[2][1]*coefficients[1][2];

    return determinant;
}

Matrice Matrice::matSansLigCol(int ligne,int colonne)
{
    Matrice newMatrice(this->getLigne()-1,this->getColonne()-1);

    int compteurL = -1;
    int compteurC = -1;

    for(int i=0; i<this->getLigne(); i++)
    {
        if(i != ligne)
        {
            compteurL++;

            for(int j=0; j<this->getColonne(); j++)
            {
                if(j != colonne)
                {
                    compteurC++;
                    newMatrice.remplir(compteurL,compteurC,this->coefficients[i][j]);
                }
            }

            compteurC = -1;
        }
    }

    return newMatrice;
}

Matrice* Matrice::comatrice()
{
    if(this->getLigne() != this->getColonne())
        return nullptr;

    Matrice *comatrice = new Matrice(this->getLigne(), this->getColonne());

    for(int i=0; i<this->getLigne(); i++)
    {
        for(int j=0; j<this->getColonne(); j++)
        {
            if(((i+j)%2)==0)
                comatrice->remplir(i,j,(matSansLigCol(i,j).determinant()));
            if(((i+j)%2)==1)
                comatrice->remplir(i,j,-(matSansLigCol(i,j).determinant()));
        }
    }

    return comatrice;
}

Matrice* Matrice::inverser()
{
    double determinant = this->determinant();

    if(determinant==0)
        return nullptr;

    Matrice *newMatrice = new Matrice(this->getLigne(),this->getColonne());

    newMatrice = comatrice()->transposer()->produitReel((1.0/determinant));

    newMatrice = newMatrice->produitReel(this->getMultiple());

    return newMatrice;
}

Matrice* Matrice::transposer()
{
    Matrice *newMatrice = new Matrice(this->getLigne(),this->getColonne());

    for(int i=0; i<this->getColonne(); i++)
    {
        for(int j=0; j<this->getLigne(); j++)
        {
            newMatrice->remplir(i,j,this->coefficients[j][i]);
        }
    }

    newMatrice->setMultiple(this->getMultiple());

    return newMatrice;
}

void Matrice::arrondir()
{
    bool divparmultiple = true;

    for(int a=0; a<this->getLigne(); a++)
    {
        for(int b=0; b<this->getColonne(); b++)
        {
            coefficients[a][b] = qRound(coefficients[a][b]);

            if(fmod(coefficients[a][b],multiple)!=0)
                divparmultiple = false;
        }
    }

    if(divparmultiple)
    {
        for(int a=0; a<ligne; a++)
        {
            for(int b=0; b<colonne; b++)
            {
                if(coefficients[a][b] != 0)
                coefficients[a][b] = coefficients[a][b]/multiple;
            }
        }

        multiple = 1;
    }
}

int Matrice::getLigne()
{
    return ligne;
}

void Matrice::setLigne(int ligne)
{
    this->ligne = ligne;
}

int Matrice::getColonne()
{
    return colonne;
}

void Matrice::setColonne(int colonne)
{
    this->colonne = colonne;
}

QVector<QVector<double> > Matrice::getcoef()
{
    return coefficients;
}

double Matrice::getCoef(int ligne, int colonne)
{
    return coefficients[ligne][colonne];
}

void Matrice::setCoefficients(QVector<QVector<double> > coefficients)
{
    this->coefficients = coefficients;
}

double Matrice::getMultiple()
{
    return multiple;
}

void Matrice::setMultiple(double multiple)
{
    this->multiple = multiple;
}
