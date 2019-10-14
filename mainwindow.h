#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLayout>
#include <QMap>
#include "matrice.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initialisation();
    void afficherMatrice(Matrice &matrice);
    void clearMatrice(QLayout *layout);
    int additionner();
    int multiplier();
    int multiplierReel();
    int determiner();
    int comatrice();
    int inverser();
    int transposer();
    int systemeHomogene();

    void saveMatriceActive();
    void disabledGui();
    int saveData();
    int loadData();

private slots:
    void on_listWidget_clicked();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    int on_pushButton_4_clicked();
    int on_pushButton_5_clicked();
    void on_spinBox_2_valueChanged(int arg1);
    void on_spinBox_valueChanged(int arg1);
    void on_listWidget_2_doubleClicked();
    void on_pushButton_6_clicked();

private:
    Ui::MainWindow *ui;
    QMap<int,QMap<QString,Matrice*> > mapMatrice;
    QMap<QString,Matrice*> listMatriceSave;
    QString nomMatriceActive;
    int actionActive;
};

#endif // MAINWINDOW_H
