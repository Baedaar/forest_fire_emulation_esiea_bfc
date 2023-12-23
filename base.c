#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>
#include "structure.h"
#include "predefined_info.h"

struct forest forest;
struct cell cell;
extern const int numPredefinedInfos;
extern struct predefinedInfo predefinedInfos[];
int iterationMax = 0;
struct cell ***temporaryForests;

void interactiveFill()
{
    printf("Entrez le nombre de lignes : ");
    scanf("%d", &forest.lignes);

    printf("Entrez le nombre de colonnes : ");
    scanf("%d", &forest.colonnes);

    startForest(&forest);
    initNextChanges(&forest);
}

int isTypeValid(char type[])
{
    if (strcmp(type, "sol") == 0 || strcmp(type, "arbre") == 0 || strcmp(type, "feuille") == 0 || strcmp(type, "roche") == 0 || strcmp(type, "herbe") == 0 || strcmp(type, "eau") == 0 || strcmp(type, "cendres") == 0 || strcmp(type, "cendres eteintes") == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void manuallyFill()
{
    for (int i = 0; i < forest.lignes; i++)
    {
        for (int j = 0; j < forest.colonnes; j++)
        {
            int typeValid = 0;
            do
            {
                printf("Entrez le type pour la case [%d][%d] : ", i, j);
                forest.matrice[i][j].type = (char *)malloc(20 * sizeof(char));
                forest.matrice[i][j].decremented = 0;
                scanf("%19s", forest.matrice[i][j].type);

                if (isTypeValid(forest.matrice[i][j].type))
                {
                    typeValid = 1;
                }
                else
                {
                    printf("Type non valide. Réessayez.\n");
                }

                scanf("%*[^\n]");
                scanf("%*c");

            } while (!typeValid);

            struct predefinedInfo info = getInfoForType(forest.matrice[i][j].type);
            forest.matrice[i][j].symbole = info.symbole;
            forest.matrice[i][j].degree = info.degree;
            forest.matrice[i][j].etat = info.etat;
            forest.matrice[i][j].isSelected = 0;
        }
    }
}

void fillForestAutomatically()
{
    for (int i = 0; i < forest.lignes; i++)
    {
        for (int j = 0; j < forest.colonnes; j++)
        {
            int randomIndex;
            struct predefinedInfo info;
            do
            {
                randomIndex = rand() % numPredefinedInfos;
                info = predefinedInfos[randomIndex];
            } while (strcmp(info.type, "cendres") == 0 || strcmp(info.type, "cendres eteintes") == 0);

            forest.matrice[i][j].symbole = info.symbole;
            forest.matrice[i][j].degree = info.degree;
            forest.matrice[i][j].etat = info.etat;
            forest.matrice[i][j].type = (char *)malloc(strlen(info.type) + 1);
            forest.matrice[i][j].decremented = 0; // Initialiser decremented à 0
            strcpy(forest.matrice[i][j].type, info.type);
        }
    }
}

void askForIteration()
{
    printf("Entrez le nombre d'itérations pour la propagation du feu : ");
    scanf("%d", &iterationMax);
}

void firePropagation(struct forest *forest)
{
    for (int x = 0; x < forest->lignes; x++)
    {
        for (int y = 0; y < forest->colonnes; y++)
        {
            struct cell *currentCell = &forest->matrice[x][y];
            if (forest->nextChanges[x][y] == 1 && strcmp(currentCell->type, "sol") != 0 && strcmp(currentCell->type, "eau") != 0)
            {
                currentCell->etat = 1;
            }
            forest->nextChanges[x][y] = 0;
        }
    }

    for (int x = 0; x < forest->lignes; x++)
    {
        for (int y = 0; y < forest->colonnes; y++)
        {
            struct cell *currentCell = &forest->matrice[x][y];
            if (currentCell->etat == 1 && strcmp(currentCell->type, "sol") != 0 && strcmp(currentCell->type, "eau") != 0)
            {
                for (int i = x - 1; i <= x + 1; i++)
                {
                    for (int j = y - 1; j <= y + 1; j++)
                    {
                        if (i >= 0 && i < forest->lignes && j >= 0 && j < forest->colonnes && !(i == x && j == y))
                        {
                            struct cell *neighboringCell = &forest->matrice[i][j];
                            if (strcmp(neighboringCell->type, "sol") != 0 &&
                                strcmp(neighboringCell->type, "eau") != 0 &&
                                strcmp(neighboringCell->type, "cendres eteintes") != 0 &&
                                neighboringCell->etat != 1)
                            {
                                forest->nextChanges[i][j] = 1;
                            }
                        }
                    }
                }
            }
        }
    }

    for (int x = 0; x < forest->lignes; x++)
    {
        for (int y = 0; y < forest->colonnes; y++)
        {
            struct cell *currentCell = &forest->matrice[x][y];
            if (currentCell->etat == 1 && strcmp(currentCell->type, "sol") != 0 && strcmp(currentCell->type, "eau") != 0)
            {
                currentCell->degree--;

                if (currentCell->degree == 0 && strcmp(currentCell->type, "sol") != 0 && strcmp(currentCell->type, "eau") != 0)
                {
                    struct predefinedInfo newInfo = getInfoForType("cendres eteintes");
                    strcpy(currentCell->type, newInfo.type);
                    currentCell->symbole = newInfo.symbole;
                    currentCell->etat = 0;
                }
            }
        }
    }
}

void saveCurrentForest(int iteration)
{
    for (int i = 0; i < forest.lignes; i++)
    {
        for (int j = 0; j < forest.colonnes; j++)
        {
            temporaryForests[iteration][i][j] = forest.matrice[i][j];
        }
    }
}

void printSavedForest(int iteration)
{
    printf("État de la forêt à l'itération %d :\n", iteration + 1);
    for (int i = 0; i < forest.lignes; i++)
    {
        for (int j = 0; j < forest.colonnes; j++)
        {
            printf("%c", temporaryForests[iteration][i][j].symbole);
        }
        printf("\n");
    }
}

int isFireOver(struct forest *forest)
{
    for (int x = 0; x < forest->lignes; x++)
    {
        for (int y = 0; y < forest->colonnes; y++)
        {
            struct cell currentCell = forest->matrice[x][y];
            if (currentCell.etat == 1)
            {
                return 0;
            }
            if (strcmp(currentCell.type, "cendres") == 0 && strcmp(currentCell.type, "cendres eteintes") != 0)
            {
                return 0;
            }
        }
    }
    return 1;
}

void freeTemporaryForest()
{
    for (int i = 0; i < iterationMax; i++)
    {
        for (int j = 0; j < forest.lignes; j++)
        {
            free(temporaryForests[i][j]);
        }
        free(temporaryForests[i]);
    }
    free(temporaryForests);
}

void navigateForest(struct forest *forest)
{
    int cursorX = 0, cursorY = 0;
    int ch;
    int totalIterations = 0;
    temporaryForests = malloc(iterationMax * sizeof(struct cell **));
    for (int i = 0; i < iterationMax; i++)
    {
        temporaryForests[i] = malloc(forest->lignes * sizeof(struct cell *));
        for (int j = 0; j < forest->lignes; j++)
        {
            temporaryForests[i][j] = malloc(forest->colonnes * sizeof(struct cell));
        }
    }

    initscr();
    keypad(stdscr, TRUE);
    noecho();

    do
    {
        clear();
        for (int i = 0; i < forest->lignes; i++)
        {
            for (int j = 0; j < forest->colonnes; j++)
            {
                if (i == cursorX && j == cursorY)
                {
                    attron(A_BOLD);
                    printw("O");
                    attroff(A_BOLD);
                }
                else
                {
                    printw("%c", forest->matrice[i][j].symbole);
                }
            }
            printw("\n");
        }

        printw("Déplacez-vous (haut, bas, gauche, droite),\ndémarrez le feu (' '),\n'r' pour retourner sur une itération precedente à la fin de la simulation \nquittez ('q'): ");
        ch = getch();

        switch (ch)
        {
        case KEY_UP:
            cursorX = (cursorX > 0) ? cursorX - 1 : cursorX;
            break;
        case KEY_DOWN:
            cursorX = (cursorX < forest->lignes - 1) ? cursorX + 1 : cursorX;
            break;
        case KEY_LEFT:
            cursorY = (cursorY > 0) ? cursorY - 1 : cursorY;
            break;
        case KEY_RIGHT:
            cursorY = (cursorY < forest->colonnes - 1) ? cursorY + 1 : cursorY;
            break;
        case ' ':
            forest->matrice[cursorX][cursorY].etat = 1;
            int currentIteration = 0;
            while (!isFireOver(forest) && currentIteration < iterationMax)
            {
                firePropagation(forest);
                clear();
                saveCurrentForest(currentIteration);
                for (int i = 0; i < forest->lignes; i++)
                {
                    for (int j = 0; j < forest->colonnes; j++)
                    {
                        printw("%c", forest->matrice[i][j].symbole);
                    }
                    printw("\n");
                }
                printw("Itération n° %d\n", totalIterations + 1);
                refresh();
                sleep(1);
                currentIteration++;
                totalIterations++;
            }
            totalIterations = 0;
            break;
        case 'r':
            echo();
            printw("\nEntrez un numéro d'itération pour revoir son état ou -1 pour revenir: ");
            refresh();

            int reviewIteration;
            scanw("%d", &reviewIteration);
            while (reviewIteration != -1)
            {
                if (reviewIteration >= 0 && reviewIteration < iterationMax)
                {
                    for (int i = 0; i < forest->lignes; i++)
                    {
                        for (int j = 0; j < forest->colonnes; j++)
                        {
                            printw("%c", temporaryForests[reviewIteration][i][j].symbole);
                        }
                        printw("\n");
                    }
                    printw("\nItération n° %d. Appuyez sur n'importe quelle touche pour continuer...", reviewIteration + 1);
                    refresh();
                    getch();
                }
                else
                {
                    printw("\nNuméro d'itération non valide. Appuyez sur n'importe quelle touche pour continuer...");
                    refresh();
                    getch();
                }
                clear();
                printw("Entrez un autre numéro d'itération ou -1 pour revenir: ");
                refresh();
                scanw("%d", &reviewIteration);
            }
            noecho();
            clear();
            break;
        }

        refresh();
    } while (ch != 'q');
    endwin();
    freeTemporaryForest();
}

int main()
{

    int fillingMode;
    srand(time(NULL));
    interactiveFill();
    printf(" Remplissage Manuel : 1 ou auto : 2 ? ");
    scanf("%d", &fillingMode);
    switch (fillingMode)
    {
    case 1:
        manuallyFill();
        break;

    case 2:
        fillForestAutomatically();
        break;

    default:
        break;
    }

    askForIteration();
    navigateForest(&forest);

    freeNextChanges(&forest);
    freeForest(&forest);
}