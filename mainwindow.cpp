#include <QVector>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QListWidget>
#include <QList>
#include <QTextStream>
#include "systeme.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->menuBar->hide();
    ui->mainToolBar->hide();
    ui->statusBar->hide();
    ui->listWidget->setFixedWidth(150);
    ui->listWidget_2->setFixedWidth(150);
    ui->lineEdit->setFixedWidth(150);
    ui->spinBox->setFixedWidth(40);
    ui->spinBox_2->setFixedWidth(40);

    ui->listWidget->addItem("1. Additionner");
    ui->listWidget->addItem("2. Multiplier");
    ui->listWidget->addItem("3. Multiplier (Scalaire)");
    ui->listWidget->addItem("4. Determinant");
    ui->listWidget->addItem("5. Comatrice");
    ui->listWidget->addItem("6. Inversion");
    ui->listWidget->addItem("7. Transposer");
    ui->listWidget->addItem("8. Systeme homogene");
    ui->listWidget->setCurrentRow(0);
    ui->listWidget->item(0)->setSelected(true);

    actionActive = ui->listWidget->currentRow();
    nomMatriceActive = "Matrice1";

    initialisation();
    afficherMatrice(*mapMatrice[actionActive].value(nomMatriceActive));

    loadData();
}

MainWindow::~MainWindow()
{
    saveData();

    for(QMap<QString,Matrice*> item : mapMatrice.values())
        for(Matrice *matrice : item.values())
            delete matrice;

    for(Matrice *matrice : listMatriceSave.values())
            delete matrice;

    clearMatrice(ui->gridLayout_4);

    delete ui;
}

void MainWindow::initialisation()
{
    // Addition
    mapMatrice[0].insert("Matrice1",new Matrice(3,3));
    mapMatrice[0].insert("Matrice2",new Matrice(3,3));
    mapMatrice[0].insert("Matrice3",new Matrice(3,3));

    // Multiplication
    mapMatrice[1].insert("Matrice1",new Matrice(3,3));
    mapMatrice[1].insert("Matrice2",new Matrice(3,3));
    mapMatrice[1].insert("Matrice3",new Matrice(3,3));

    // Multiplication (Scalaire)
    mapMatrice[2].insert("Matrice1",new Matrice(3,3));
    mapMatrice[2].insert("Matrice2",new Matrice(1,1));
    mapMatrice[2].insert("Matrice3",new Matrice(3,3));

    // Déterminant
    mapMatrice[3].insert("Matrice1",new Matrice(3,3));
    mapMatrice[3].insert("Matrice3",new Matrice(1,1));

    // Comatrice
    mapMatrice[4].insert("Matrice1",new Matrice(3,3));
    mapMatrice[4].insert("Matrice3",new Matrice(3,3));

    // Inverse
    mapMatrice[5].insert("Matrice1",new Matrice(3,3));
    mapMatrice[5].insert("Matrice3",new Matrice(3,3));

    // Transposer
    mapMatrice[6].insert("Matrice1",new Matrice(3,3));
    mapMatrice[6].insert("Matrice3",new Matrice(3,3));

    // Système homogène
    mapMatrice[7].insert("Matrice1",new Matrice(3,3));
    mapMatrice[7].insert("Matrice2",new Matrice(3,1));
    mapMatrice[7].insert("Matrice3",new Matrice(3,1));
}

void MainWindow::afficherMatrice(Matrice &matrice)
{
    ui->spinBox_2->setValue(matrice.getLigne());
    ui->spinBox->setValue(matrice.getColonne());

    if(ui->gridLayout_4->count() != 0)
        clearMatrice(ui->gridLayout_4);

    for(int row=0; row<matrice.getLigne(); ++row)
    {
        for(int column=0; column<matrice.getColonne(); ++column)
        {
            auto coef = new QLineEdit(this);
            coef->setFixedWidth(50);

            ui->gridLayout_4->addWidget(coef, row, column);
            coef->setText(QString::number(matrice.getCoef(row,column)));
        }
    }
}

void MainWindow::clearMatrice(QLayout *layout)
{
    if (!layout)
       return;

    while(auto item = layout->takeAt(0))
    {
        delete item->widget();
        clearMatrice(item->layout());
    }
}

int MainWindow::additionner()
{
    Matrice *matrice = mapMatrice[0].value("Matrice1")->somme(mapMatrice[0].value("Matrice2"));

    if(matrice == nullptr)
    {
        QMessageBox::critical(this, "Erreur...", "Matrice incompatible");
        return 0;
    }

    nomMatriceActive = "Matrice3";
    mapMatrice[actionActive].insert(nomMatriceActive,matrice);

    return 1;
}

int MainWindow::multiplier()
{
    Matrice *matrice = mapMatrice[1].value("Matrice1")->produit(mapMatrice[1].value("Matrice2"));

    if(matrice == nullptr)
    {
        QMessageBox::critical(this, "Erreur...", "Matrice incompatible");
        return 0;
    }

    nomMatriceActive = "Matrice3";
    mapMatrice[actionActive].insert(nomMatriceActive,matrice);

    return 1;
}

int MainWindow::multiplierReel()
{
    Matrice *matrice = mapMatrice[2].value("Matrice1")->produitReel(mapMatrice[2].value("Matrice2")->getCoef(0,0));

    if(matrice == nullptr)
    {
        QMessageBox::critical(this, "Erreur...", "Impossible de réaliser l'opération");
        return 0;
    }

    nomMatriceActive = "Matrice3";
    mapMatrice[actionActive].insert(nomMatriceActive,matrice);

    return 1;
}

int MainWindow::determiner()
{
    Matrice *matrice = new Matrice(1,1);

    double determinant = mapMatrice[3].value("Matrice1")->determinant();

    matrice->remplir(0,0,determinant);

    nomMatriceActive = "Matrice3";
    mapMatrice[actionActive].insert(nomMatriceActive,matrice);

    return 1;
}

int MainWindow::comatrice()
{
    Matrice *matrice = mapMatrice[4].value("Matrice1")->comatrice();

    if(matrice == nullptr)
    {
        QMessageBox::critical(this, "Erreur...", "Impossible de réaliser l'opération");
        return 0;
    }

    nomMatriceActive = "Matrice3";
    mapMatrice[actionActive].insert(nomMatriceActive,matrice);

    return 1;
}

int MainWindow::inverser()
{
    Matrice *matrice = mapMatrice[5].value("Matrice1")->inverser();

    if(matrice == nullptr)
    {
        QMessageBox::critical(this, "Erreur...", "Impossible de réaliser l'opération");
        return 0;
    }

    nomMatriceActive = "Matrice3";
    mapMatrice[actionActive].insert(nomMatriceActive,matrice);

    return 1;
}

int MainWindow::transposer()
{
    Matrice *matrice = mapMatrice[6].value("Matrice1")->transposer();

    if(matrice == nullptr)
    {
        QMessageBox::critical(this, "Erreur...", "Impossible de réaliser l'opération");
        return 0;
    }

    nomMatriceActive = "Matrice3";
    mapMatrice[actionActive].insert(nomMatriceActive,matrice);

    return 1;
}

int MainWindow::systemeHomogene()
{
    if(mapMatrice[7].value("Matrice1")->getLigne() != mapMatrice[7].value("Matrice2")->getLigne())
    {
        QMessageBox::critical(this, "Erreur...", "Systeme non-homogene");
        return 0;
    }

    Matrice *matrice = nullptr;

    matrice = mapMatrice[7].value("Matrice1");

    Systeme *syst = new Systeme(matrice->getLigne(), matrice->getColonne());

    for(int i=0; i<matrice->getLigne(); i++)
        for(int j=0; j<matrice->getColonne(); j++)
            syst->setCoef(i,j,matrice->getCoef(i,j));

    matrice = mapMatrice[7].value("Matrice2");

    for(int i=0; i<matrice->getLigne(); i++)
        syst->setEgalites(i,matrice->getCoef(i,0));

    QVector<double> solution = syst->getSolution();

    Matrice *newMatrice = new Matrice(matrice->getLigne(),matrice->getColonne());

    for(int i=0; i<solution.count(); ++i)
        newMatrice->remplir(i,0,solution[i]);

    nomMatriceActive = "Matrice3";
    mapMatrice[actionActive].insert(nomMatriceActive,newMatrice);

    return 1;
}

void MainWindow::on_listWidget_clicked()
{
    if(!ui->gridLayout_4->isEmpty())
        saveMatriceActive();

    actionActive = ui->listWidget->currentRow();
    nomMatriceActive = "Matrice1";

    afficherMatrice(*mapMatrice[actionActive].value("Matrice1"));

    disabledGui();
}

void MainWindow::on_listWidget_2_doubleClicked()
{
    Matrice *matrice = new Matrice(*listMatriceSave[ui->listWidget_2->currentItem()->text()]);

    Matrice *temp = mapMatrice[actionActive][nomMatriceActive];
    mapMatrice[actionActive][nomMatriceActive] = matrice;
    delete temp;

    afficherMatrice(*matrice);
}

void MainWindow::on_pushButton_clicked()
{
    Matrice *matrice = mapMatrice[actionActive].value(nomMatriceActive);

    for(int row=0; row<matrice->getLigne(); ++row)
    {
        for(int column=0; column<matrice->getColonne(); ++column)
        {
            QLineEdit *qle = qobject_cast<QLineEdit *>(ui->gridLayout_4->itemAtPosition(row,column)->widget());
            qle->setText("0");
        }
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    if(!ui->gridLayout_4->isEmpty())
        saveMatriceActive();

    nomMatriceActive = "Matrice1";

    afficherMatrice(*mapMatrice[actionActive].value(nomMatriceActive));

    disabledGui();
}

void MainWindow::on_pushButton_3_clicked()
{
    if(!ui->gridLayout_4->isEmpty())
        saveMatriceActive();

    nomMatriceActive = "Matrice2";

    afficherMatrice(*mapMatrice[actionActive].value(nomMatriceActive));

    disabledGui();
}

int MainWindow::on_pushButton_4_clicked()
{
    if(!ui->gridLayout_4->isEmpty())
        saveMatriceActive();

    switch(actionActive)
    {
        case 0: if(!additionner()) return 1;
            break;
        case 1: if(!multiplier()) return 1;
            break;
        case 2: if(!multiplierReel()) return 1;
            break;
        case 3: if(!determiner()) return 1;
            break;
        case 4: if(!comatrice()) return 1;
            break;
        case 5: if(!inverser()) return 1;
            break;
        case 6: if(!transposer()) return 1;
            break;
        case 7: if(!systemeHomogene()) return 1;
            break;
        default:
            break;
    }

    afficherMatrice(*mapMatrice[actionActive].value(nomMatriceActive));

    disabledGui();

    return 0;
}

int MainWindow::on_pushButton_5_clicked()
{
    //QString nomMatrice = ui->listWidget_2->findItems(ui->lineEdit->text(), Qt::MatchExactly);

    if(ui->lineEdit->text().isEmpty())
    {
        QMessageBox::critical(this, "Erreur...", "Entrez un nom pour votre matrice.");
        return 1;
    }

    for(int i=0; i<ui->listWidget_2->count(); ++i)
    {
        if(ui->listWidget_2->item(i)->text() == ui->lineEdit->text())
        {
            QMessageBox::critical(this, "Erreur...", "Ce nom est déja pris.");
            return 1;
        }
    }

    if(!ui->gridLayout_4->isEmpty())
        saveMatriceActive();

    Matrice *matrice = new Matrice(*mapMatrice[actionActive].value(nomMatriceActive));
    listMatriceSave.insert(ui->lineEdit->text(),matrice);
    ui->listWidget_2->addItem(ui->lineEdit->text());

    return 0;
}

void MainWindow::on_pushButton_6_clicked()
{
    if(ui->listWidget_2->count() > 0)
    {
        QString nomMatrice = ui->listWidget_2->currentItem()->text();

        delete listMatriceSave[nomMatrice];
        listMatriceSave.remove(nomMatrice);


        for(int i=0; i<ui->listWidget_2->count(); ++i)
        {
            if(ui->listWidget_2->item(i)->text() == nomMatrice)
            {
                delete ui->listWidget_2->item(i);
            }
        }
    }
}

void MainWindow::saveMatriceActive()
{
    Matrice *matrice = mapMatrice[actionActive].value(nomMatriceActive);

    for(int row=0; row<matrice->getLigne(); ++row)
    {
        for(int column=0; column<matrice->getColonne(); ++column)
        {
            QLineEdit *qle = qobject_cast<QLineEdit *>(ui->gridLayout_4->itemAtPosition(row,column)->widget());
            matrice->remplir(row,column,qle->text().toDouble());
        }
    }
}

void MainWindow::on_spinBox_2_valueChanged(int arg1)
{
    Matrice *matrice = mapMatrice[actionActive].value(nomMatriceActive);

    if(matrice->getLigne() != arg1)
    {
        if(!ui->gridLayout_4->isEmpty())
            saveMatriceActive();

        if(matrice->getLigne() < arg1)
            matrice->ajouterLigne();
        else if(matrice->getLigne() > arg1)
            matrice->retirerLigne();

        afficherMatrice(*matrice);
    }
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    Matrice *matrice = mapMatrice[actionActive].value(nomMatriceActive);

    if(matrice->getColonne() != arg1)
    {
        if(!ui->gridLayout_4->isEmpty())
            saveMatriceActive();

        if(matrice->getColonne() < arg1)
            matrice->ajouterColonne();
        else if(matrice->getColonne() > arg1)
            matrice->retirerColonne();

        afficherMatrice(*matrice);
    }
}

void MainWindow::disabledGui()
{
    ui->spinBox->setEnabled(true);
    ui->spinBox_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);

    if(nomMatriceActive == "Matrice3")
    {
        ui->spinBox->setEnabled(false);
        ui->spinBox_2->setEnabled(false);
    }

    if(actionActive == 2 && nomMatriceActive == "Matrice2")
    {
        ui->spinBox->setEnabled(false);
        ui->spinBox_2->setEnabled(false);
    }

    if(actionActive == 7 && nomMatriceActive == "Matrice2")
    {
        ui->spinBox->setEnabled(false);
    }

    if(actionActive == 3 || actionActive == 4 || actionActive == 5 || actionActive == 6)
    {
        ui->pushButton_3->setEnabled(false);
    }
}

int MainWindow::saveData()
{
    QFile saveMatrice("saveMatrice.save");

    if(saveMatrice.open(QIODevice::WriteOnly | QIODevice::Text) == false)
        return 1;

    QTextStream flux(&saveMatrice);
    flux << listMatriceSave.count() << endl;

    int pos = 0;

    for(Matrice *matrice : listMatriceSave.values())
    {
        flux << ui->listWidget_2->item(pos)->text() << endl;
        flux << matrice->getLigne() << endl;
        flux << matrice->getColonne() << endl;
        flux << matrice->getMultiple() << endl;

        for(QVector<double> item : matrice->getcoef())
        {
            for(double nbr : item)
            {
                flux << nbr << endl;
            }
        }

        pos++;
    }

    return 0;
}

int MainWindow::loadData()
{
    QFile saveMatrice("saveMatrice.save");

    if(saveMatrice.open(QIODevice::ReadOnly | QIODevice::Text) == false)
        return 1;

    QTextStream flux(&saveMatrice);
    int size;
    size = flux.readLine().toInt();

    QString nomMatrice;
    int ligne = 0;
    int colonne = 0;
    int multiple = 0;
    double coefTemp = 0;

    for(int i=0; i<size; i++)
    {
        nomMatrice = flux.readLine();
        ligne = flux.readLine().toInt();
        colonne = flux.readLine().toInt();
        multiple = flux.readLine().toInt();

        Matrice *matrice = new Matrice(ligne,colonne);
        matrice->setMultiple(multiple);

        for(int i=0; i<ligne; i++)
        {
            for(int j=0; j<colonne; j++)
            {
                coefTemp = flux.readLine().toDouble();
                matrice->remplir(i,j,coefTemp);
            }
        }

        listMatriceSave.insert(nomMatrice,matrice);
        ui->listWidget_2->addItem(nomMatrice);
    }

    return 0;
}
