#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractButton>
#include <QKeyEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void numberGroup_clicked(QAbstractButton*);
    void actionGroup_clicked(QAbstractButton*);

    void on_actionDel_clicked();
    void on_actionCalc_clicked();
    void on_comma_clicked();
    void on_actionClear_clicked();
    void on_actionPercent_clicked();
    void on_actionSign_clicked();

private:
    Ui::MainWindow *ui;
    //Limite de chiffres
    const int DIGIT_LIMIT = 16;
    //Marquer pour vérifier si le bouton précédent sur lequel on a cliqué était un opérateur
    bool operatorClicked;
    //Dernier opérateur demandé
    QChar storedOperator;
    //Drapeau pour vérifier si un numéro est stocké en mémoire
    bool hasStoredNumber;
    //Numéro enregistré
    double storedNumber;
    //Calculer le résultat en fonction du nombre stocké et du nombre affiché
    void calculate_result();

protected:
    void keyPressEvent(QKeyEvent *e);
};

#endif // MAINWINDOW_H
