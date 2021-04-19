#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Configurer un panneau d'affichage vide
    ui->displayPanel->clear();

    //Définir l'opérateur et stocker les indicateurs de numéro sur false
    operatorClicked = false;
    hasStoredNumber = false;

    //Définir un écouteur de groupe de touches numériques
    ui->numberGroup->connect(ui->numberGroup,SIGNAL(buttonClicked(QAbstractButton*)),
                             this, SLOT(numberGroup_clicked(QAbstractButton*)));
    //Définir un écouteur de groupe de boutons opérateur
    ui->actionGroup->connect(ui->actionGroup,SIGNAL(buttonClicked(QAbstractButton*)),
                             this, SLOT(actionGroup_clicked(QAbstractButton*)));

    //Définir la largeur et la hauteur de la fixation de la fenêtre
    this->setFixedSize(QSize(306, 319));
}

MainWindow::~MainWindow()
{
    delete ui;
}


//==================================================================================
//Fonctions Slot
//==================================================================================
// Cette fonction sera appelée chaque fois qu'un bouton numérique est cliqué
void MainWindow::numberGroup_clicked(QAbstractButton* button)
{
    //Obtenir la string de l'affichage
    QString displayLabel = ui->displayPanel->text();

    /* Vérifiez si le bouton précédent sur lequel on a cliqué était un bouton opérateur(+;-;*;/;...).
     * Si touche = operateur, on efface le display et l'indicateur passe sur false.
     * Puis, Ajout du chiffre demandé */
    if (operatorClicked) {
        displayLabel.clear();
        operatorClicked = false;
    }

    //On ajoute alors le chiffre uniquement si nous ne dépassons pas la limite des chiffres
    if (displayLabel.length() >= DIGIT_LIMIT) {
        return;
    }

    //Ajouter le chiffre demandé
    displayLabel.append(button->text());

    //Mettre le numero à afficher sur le display
    ui->displayPanel->setText(displayLabel);
}
//--------------------------------------------------------------
// cette fonction est appelée chaque fois qu'un bouton d'action est cliqué
void MainWindow::actionGroup_clicked(QAbstractButton* button)
{
    /* Si le bouton précédent sur lequel on a cliqué n'était pas un opérateur,
     * alors sauvegarder l'opérateur qui a été demandé et quitter.
     * Sinon si c'était le cas, nous devons voir si nous devons simplement enregistrer le numéro qui est
     * affiché ou s'il y a déjà un nombre stocké en mémoire, effectuez le calcul et
     * stocker le résultat.
     * Exemple pour ce cas: 5 + 7 + -> Nous devons enregistrer 12 en mémoire puis enregistrer l'opérateur. */

    if (operatorClicked) {
        storedOperator = button->text().at(0);
    }
    else {
        if (hasStoredNumber) {
            calculate_result();
        }
        else {
            //Définir le drapeau pour indiquer que nous avons maintenant un numéro stocké en mémoire
            hasStoredNumber = true;
            //On prends la chaîne de l'affichage
            QString displayLabel = ui->displayPanel->text();
            //On convertit la chaîne en double et on l'enregistrer
            storedNumber = displayLabel.toDouble();
        }
        //Definir l'indicateur indiquant que le dernier bouton sur lequel l'on a cliqué était un opérateur
        operatorClicked = true;
        //Stocker l'opérateur en mémoire
        storedOperator = button->text().at(0);
    }
}
//-------------------------------------------------------------
void MainWindow::on_actionDel_clicked()
{
    //Obtenir la chaîne de l'affichage
    QString displayLabel = ui->displayPanel->text();

    //Vérifiez si label est vide
    if (displayLabel.length() == 0) {
        return;
    }

    //Supprimer le dernier chiffre de la chaîne
    displayLabel.QString::chop(1);
    //Régler le numéro à afficher
    ui->displayPanel->setText(displayLabel);
}
//-------------------------------------------------------
void MainWindow::on_comma_clicked()
{
    //Obtenir la chaîne de la display
    QString displayLabel = ui->displayPanel->text();

    /* Ajout du chiffre uniquement si nous ne dépassons pas la limite de chiffres.
     * Plus précisément dans ce cas, nous avons besoin de 2 chiffres pour être disponibles.
     * Un pour la virgule et au moins un autre pour un chiffre restant.
     * On véerifie également si une autre virgule est déjà présente. */
    if (displayLabel.length() >= (DIGIT_LIMIT - 1) ||
        displayLabel.contains('.', Qt::CaseSensitive)) {
        return;
    }

    //
    if (displayLabel.length() == 0) {
        displayLabel = "0";
    }

    //Ajout de la virgule
    displayLabel.append('.');
    //Remettre le numero à l'écran
    ui->displayPanel->setText(displayLabel);
}
//-------------------------------------------------
void MainWindow::on_actionClear_clicked()
{
    //Effacer l'écran (pour l'instant)
    ui->displayPanel->clear();
    //Définir l'opérateur et stocker les indicateurs de numéro sur false
    operatorClicked = false;
    hasStoredNumber = false;
}
//-----------------------------------------------------
void MainWindow::on_actionPercent_clicked()
{
    //Prendre la chaîne de l'ecran
    QString displayLabel = ui->displayPanel->text();
    //Convertir en double
    double percentage = displayLabel.toDouble();
    //Il suffit de multiplier par 0,01 pour en faire un pourcentage
    percentage *= 0.01;
    //Puisqu'il peut y avoir un débordement, il est préférable de convertir le nombre avec soin
    displayLabel = QString::number(percentage,'g', DIGIT_LIMIT);
    //
    ui->displayPanel->setText(displayLabel);
}
//----------------------------------------------------------
void MainWindow::on_actionSign_clicked()
{
    //Prendre la chaîne de l'ecran
    QString displayLabel = ui->displayPanel->text();
    //Convertir en double
    double percentage = displayLabel.toDouble();
    //Il suffit de multiplier par -1 pour changer son signe
    percentage *= -1;
    //Convertir le nombre avec soin
    displayLabel = QString::number(percentage,'g', DIGIT_LIMIT);
    //Remettre le numero à l'ecran
    ui->displayPanel->setText(displayLabel);
}


//==================================================================================
//Fonctions Autre
//==================================================================================
void MainWindow::calculate_result() {
    //Prendre la chaine de caractere de l'ecran
    QString displayLabel = ui->displayPanel->text();

    //Si le nombre affiché se termine par une virgule, supprimez la virgule.
     if (displayLabel.endsWith('.',Qt::CaseSensitive)) {
         displayLabel.QString::chop(1);
     }

     //On decide maintenant que faire en fonction de l'operation
     if (storedOperator == '+') {//Pour l'operateur de l'addition
         storedNumber += displayLabel.toDouble();
     }
     else if (storedOperator == '-') {//Pour l'operateur de soustraction
         storedNumber -= displayLabel.toDouble();
     }
     else if (storedOperator == 'x') {//Pour l'operateur de la multiplicatin
         storedNumber *= displayLabel.toDouble();
     }
     else if (storedOperator == '/') {//Pour l'operateur de la division
         storedNumber /= displayLabel.toDouble();
     }

     //Puisqu'il peut y avoir un débordement, il est préférable de convertir le nombre avec précaution
     displayLabel = QString::number(storedNumber,'g', DIGIT_LIMIT);

     //Mettre nnumero à afficher
     ui->displayPanel->setText(displayLabel);
}

//Faire en sorte que les boutons du clavier doivent appeler les fonctions correspondantes
void MainWindow::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {
        //Les nombres
        case Qt::Key_1:
            numberGroup_clicked(ui->num1);
            break;
        case Qt::Key_2:
            numberGroup_clicked(ui->num2);
            break;
        case Qt::Key_3:
            numberGroup_clicked(ui->num3);
            break;
        case Qt::Key_4:
            numberGroup_clicked(ui->num4);
            break;
        case Qt::Key_5:
            numberGroup_clicked(ui->num5);
            break;
        case Qt::Key_6:
            numberGroup_clicked(ui->num6);
            break;
        case Qt::Key_7:
            numberGroup_clicked(ui->num7);
            break;
        case Qt::Key_8:
            numberGroup_clicked(ui->num8);
            break;
        case Qt::Key_9:
            numberGroup_clicked(ui->num9);
            break;
        case Qt::Key_0:
            numberGroup_clicked(ui->num0);
            break;
        //Les signes d'operations
        case Qt::Key_Plus:
            actionGroup_clicked(ui->actionPlus);
            break;
        case Qt::Key_Minus:
            actionGroup_clicked(ui->actionMinus);
            break;
        case Qt::Key_Asterisk:
            actionGroup_clicked(ui->actionMul);
            break;
        case Qt::Key_Slash:
            actionGroup_clicked(ui->actionDiv);
            break;
        //Virgule
        case Qt::Key_Period:
            on_comma_clicked();
            break;
        //La touche Entrée
        case Qt::Key_Enter:
        case Qt::Key_Return:
            on_actionCalc_clicked();
            break;
        //La touche de retour
        case Qt::Key_Backspace:
            on_actionDel_clicked();
            break;
        case Qt::Key_Delete:
            on_actionClear_clicked();
            break;
        //Le pourcentage
        case Qt::Key_Percent:
            on_actionPercent_clicked();
            break;
    }
}




