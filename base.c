#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>
#include "structure.h"
#include "predefined_info.h"


struct forest forest;
struct cell cell;
extern const int numPredefinedInfos;
extern struct predefinedInfo predefinedInfos[];
int iterationCounter = 0;




void initializeForestInteractive() {
    printf("Entrez le nombre de lignes : ");
    scanf("%d", &forest.rows);
    
    printf("Entrez le nombre de colonnes : ");
    scanf("%d", &forest.columns);
    
    initializeForest(&forest);
}

// Fonction permettant de vérifier le symbole entré par l'utilisateur
int isSymbolValid(char symbol) {
    if (symbol == '+' || symbol == '*'
     || symbol == ' ' || symbol == '#'
      || symbol == 'x' || symbol == '/'
       || symbol == '-' || symbol == '@' )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// Fonction permettant de vérifier le type entré par l'utilisateur.
int isTypeValid (char type[]) {
    if (strcmp(type, "sol") == 0 || strcmp(type, "arbre") == 0
     || strcmp(type, "feuille") == 0 || strcmp(type, "roche") == 0
      || strcmp(type, "herbe") == 0 || strcmp(type, "eau") == 0
       || strcmp(type, "cendres") == 0 || strcmp(type, "cendres eteintes") == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }

}

void fillForestManually() {
    for (int i = 0; i < forest.rows; i++) {
        for (int j = 0; j < forest.columns; j++) {
            int typeValid = 0;
            do {
                printf("Entrez le type pour la case [%d][%d] : ", i, j);
                forest.matrice[i][j].type = (char *)malloc(20 * sizeof(char));
                forest.matrice[i][j].decremented = 0; // Initialiser decremented à 0
                scanf("%19s", forest.matrice[i][j].type);

                if (isTypeValid(forest.matrice[i][j].type)) {
                    typeValid = 1;
                } else {
                    printf("Type non valide. Réessayez.\n");
                }

                scanf("%*[^\n]");
                scanf("%*c");

            } while (!typeValid);

            struct predefinedInfo info = getInfoForType(forest.matrice[i][j].type);
            forest.matrice[i][j].symbol = info.symbol;
            forest.matrice[i][j].degree = info.degree;
            forest.matrice[i][j].state = info.state;
            forest.matrice[i][j].isSelected = 0; // Initialiser isSelected à 0
        }
    }
}

void fillForestAutomatically() {
    for (int i = 0; i < forest.rows; i++) {
        for (int j = 0; j < forest.columns; j++) {
            int randomIndex = rand() % numPredefinedInfos;
            struct predefinedInfo info = predefinedInfos[randomIndex];
            forest.matrice[i][j].symbol = info.symbol;
            forest.matrice[i][j].degree = info.degree;
            forest.matrice[i][j].state = info.state;
            forest.matrice[i][j].type = (char *)malloc(strlen(info.type) + 1);
            forest.matrice[i][j].decremented = 0; // Initialiser decremented à 0
            forest.matrice[i][j].isSelected = 0; // Initialiser isSelected à 0
            strcpy(forest.matrice[i][j].type, info.type);
        }
    }
}




// Fonction pour afficher la forêt
void printForest() {
    printf("Grille de la forêt :\n");
    for (int i = 0; i < forest.rows; i++) {
        for (int j = 0; j < forest.columns; j++) {
            printf("%c", forest.matrice[i][j].symbol);
        }
        printf("\n");
    }
}


void updateNeighboringCells(struct forest *forest, int x, int y) {

    // Vérifier si la cellule sélectionnée est valide
    if (x < 0 || x >= forest->rows || y < 0 || y >= forest->columns) {
        return; // Sortir de la fonction si la cellule sélectionnée n'est pas valide
    }

    // Vérifier si la cellule a déjà été décrémentée
    if (forest->matrice[x][y].decremented == 1) {
        return; // Sortir si la cellule a déjà été décrémentée
    }

    // Mettre à jour le symbole de la cellule
    if (forest->matrice[x][y].degree == 1 ) {
        if (strcmp(forest->matrice[x][y].type, "sol") != 0 && strcmp(forest->matrice[x][y].type, "eau") != 0) {
            forest->matrice[x][y].symbol = '-';
        } else if (strcmp(forest->matrice[x][y].type, "sol") != 0 && strcmp(forest->matrice[x][y].type, "eau") != 0 || forest->matrice[x][y].degree == 0) 
        {
            forest->matrice[x][y].symbol = '@';
        }
         {
            forest->matrice[x][y].symbol = '@';
        }
    }

    // Décrémenter le degré de la cellule
    if (forest->matrice[x][y].degree != 0)
    {
        forest->matrice[x][y].degree--;
    }
    forest->matrice[x][y].decremented = 1; // Marquer la cellule comme décrémentée

    // Parcourir les 8 voisins de la case sélectionnée
    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            // Vérifier les limites de l'indice et éviter de mettre à jour la case d'origine à nouveau
            if (i >= 0 && i < forest->rows && j >= 0 && j < forest->columns && !(i == x && j == y)) {
                // Vérifier si le voisin n'a pas encore été décrémenté
                if (!forest->matrice[i][j].decremented) {
                    printf("Calling updateNeighboringCells for cell [%d][%d]\n", i, j);
                    // Appeler récursivement la fonction pour mettre à jour les voisins
                    updateNeighboringCells(forest, i, j);
                    iterationCounter++;
                    forest->matrice[x][y].decremented = 0;

                    break;
                }
            }
        }
    }
}
// Fonction pour gérer le déplacement dans la forêt
void navigateForest(struct forest *forest) {
    int cursorX = 0;
    int cursorY = 0;
    bool isCellSelected = false;

    int ch; // Variable pour stocker les entrées clavier
    int sel; // Variable pour stocker la cellule selectionée

   
    // Initialiser ncurses
    initscr();
     // Initialiser ncurses pour la gestion des couleurs
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK); // Pair de couleur pour le texte rouge sur fond noir
    keypad(stdscr, TRUE); // Activer la gestion des touches spéciales
    noecho(); // Désactiver l'affichage automatique des touches

    // Boucle pour la navigation
    do {
        // Effacer l'écran
        clear();

        // Afficher la forêt avec le curseur
    for (int i = 0; i < forest->rows; i++) {
        for (int j = 0; j < forest->columns; j++) {
            if (i == cursorX && j == cursorY) {
                attron(A_BOLD); // Activer l'attribut de texte en gras
                printw("O"); // Afficher le curseur
                attroff(A_BOLD); // Désactiver l'attribut de texte en gras
            } 
            else {
                printw("%c", forest->matrice[i][j].symbol);
            }
        }
        printw("\n");
    }

    

        // Demander à l'utilisateur de saisir une touche
        printw("Déplacez-vous (haut , bas , gauche , droite , selectionner/deselectionner ' ' quitter 'q'): ");
        ch = getch();

        if (ch == 'q') {
            break; // Quitter si l'utilisateur le souhaite
        }

        // Déplacer le curseur en fonction de la touche saisie
        switch (ch) {
    case KEY_UP:    // Flèche vers le haut
        if (cursorX > 0) {
            cursorX--;
        }
        break;
    case KEY_DOWN:  // Flèche vers le bas
        if (cursorX < forest->rows - 1) {
            cursorX++;
        }
        break;
    case KEY_LEFT:  // Flèche vers la gauche
        if (cursorY > 0) {
            cursorY--;
        }
        break;
    case KEY_RIGHT: // Flèche vers la droite
        if (cursorY < forest->columns - 1) {
            cursorY++;
        }
        break;
    case 32:
                // Appeler la fonction récursive pour mettre à jour la cellule actuelle et les cellules voisines
                updateNeighboringCells(forest, cursorX, cursorY);
                refresh(); // Mettre à jour l'écran
                usleep(500000);
                break;
}

    } while (1); // Boucle infinie pour la navigation

    // Fermer ncurses
    endwin();
}

int main() {

    int fillingMode;
    srand(time(NULL));
    
    initializeForestInteractive();

    printf(" Remplissage Manuel : 1 ou auto : 2 ? ");
    scanf("%d", &fillingMode);

    // Switch permettant de laisser le choix à l'utilisateur du mode de remplissage de la forêt.
    switch (fillingMode)
    {
    case 1:
        fillForestManually();
        break;

        case 2:
             fillForestAutomatically();
            break;
    
    default:
        break;
    }
    
    // fonction permettant d'afficher la forêt.
    printForest();

    navigateForest(&forest);
    
    // Fonction permettant de libérer la mémoire allouée à la forêt.
    freeForest(&forest);
    
}