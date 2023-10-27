#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include <windows.h>

#include <chrono>
#include <thread>

using namespace std;

const int LARGEUR = 25;
const int LONGUEUR = 30;
const int NMINE = 90;

int X = 5;
int Y = 10;

bool GameOver = false;
bool GameWin = false;

int countMine(int **t, int a, int b);
void showArray2D(int **t, int a, int b);
void showArray2DBool(bool **t, int a, int b);
bool **newArrayBool(int a, int b);
int **newArray(int a, int b);
int *newArray(int a);
void placeMine(int **t);
int countAllMine(int **t);

void printGame(int **t, bool **tshow, bool Curse);
void flag(int **t, bool **tshow);
void mine(int **t, bool **tshow);
void Win(int **t);
void move(int **t, bool **tshow);
void launch(int **t, bool **tshow);

void propagation(int **t, bool **tshow);
bool propAux(int **tp, bool **tshowp, int a, int b);

int main(int argc, char const *argv[])
{

    int **t = newArray(LARGEUR, LONGUEUR);
    bool **tshow = newArrayBool(LARGEUR, LONGUEUR);
    // showArray2DBool(tshow, LARGEUR, LONGUEUR);
    placeMine(t);
    // showArray2D(t, LARGEUR, LONGUEUR);
    // printf("numb total of mine in the game: %d\n", countAllMine(t));
    // int a = countMine(t, 2, 2);
    // printf("numb of mine in 1, 1 is: %d\n", a);
    int time = 0;
    launch(t, tshow);
    propagation(t, tshow);
    printGame(t, tshow, true);

    while (!GameWin && !GameOver)
    {
        time += 1;
        move(t, tshow);
        propagation(t, tshow);
        this_thread::sleep_for(200ms);
        Win(t);
    }
    system("cls");
    printf("Game: Mine remaining: %d\n", countAllMine(t));
    for (int i = 0; i < LARGEUR; i += 1)
    {
        for (int y = 0; y < LONGUEUR; y += 1)
        {
            if (t[i][y] == -1)
            {
                cout << "# ";
            }
            else if (t[i][y] == 0)
            {
                cout << "  ";
            }
            else if (t[i][y] == -2)
            {
                cout << "$ ";
            }
            else if (t[i][y] == -3)
            {
                cout << "& ";
            }
            else
            {
                cout << t[i][y] << " ";
            }
        }
        cout << endl;
    }
    cout << "end of program" << endl;
    return 0;
}

void launch(int **t, bool **tshow)
{
    bool findstart = false;
    srand(time(NULL));
    while (!findstart)
    {
        int x = rand() % LARGEUR;
        int y = rand() % LONGUEUR;
        if (t[x][y] == 0)
        {
            tshow[x][y] = true;
            findstart = !findstart;
        }
    }
}

void propagation(int **t, bool **tshow)
{
    bool update = false;
    for (int i = 0; i < LARGEUR; i += 1)
    {
        for (int y = 0; y < LONGUEUR; y += 1)
        {
            if (t[i][y] == 0 && tshow[i][y])
            {
                update = propAux(t, tshow, i, y);
            }
        }
    }
    if (update)
    {
        system("cls");
        printGame(t, tshow, true);
    }
}

bool propAux(int **t, bool **tshow, int a, int b)
{
    bool update = false;
    if (t[a][b] == 0)
    {
        // cout << "cellue null ";
        // printf("a: %d, b: %d", a, b);
        tshow[a][b] = true;
        // cout << " tshow is to true" << endl;
        if (a > 0 && a < LARGEUR - 1)
        {
            // printf("a?");
            if (b > 0 && b < LONGUEUR - 1)
            {
                update = true;
                // printf("b?");
                if (t[a + 1][b + 1] != -1)
                    tshow[a + 1][b + 1] = true;
                if (t[a + 1][b] != -1)
                    tshow[a + 1][b] = true;
                if (t[a + 1][b - 1] != -1)
                    tshow[a + 1][b - 1] = true;
                if (t[a][b + 1] != -1)
                    tshow[a][b + 1] = true;
                if (t[a][b - 1] != -1)
                    tshow[a][b - 1] = true;
                if (t[a - 1][b + 1] != -1)
                    tshow[a - 1][b + 1] = true;
                if (t[a - 1][b] != -1)
                    tshow[a - 1][b] = true;
                if (t[a - 1][b - 1] != -1)
                    tshow[a - 1][b - 1] = true;
            }
        }
        // cout << "a is too or not length" << endl;
    }
    return update;
}

void move(int **t, bool **tshow)
{
    bool moved = false;

    if (GetAsyncKeyState(0x5A) && X > 0) // z top
    {
        moved = true;
        X -= 1;
    }
    if (GetAsyncKeyState(0x51) && Y > 0) // q left
    {
        moved = true;
        Y -= 1;
    }
    if (GetAsyncKeyState(0x53) && X < LARGEUR-1) // s bottom
    {
        moved = true;
        X += 1;
    }
    if (GetAsyncKeyState(0x44) && Y < LONGUEUR-1) // d righ
    {
        moved = true;
        Y += 1;
    }
    if (GetAsyncKeyState(0x4F)) // o mine
    {
        moved = true;
        mine(t, tshow);
    }
    if (GetAsyncKeyState(0x50)) // p flag
    {
        moved = true;
        flag(t, tshow);
    }

    if (moved)
    {
        system("cls");
        printGame(t, tshow, true);
    }
}

void Win(int **t)
{
    int count = 0;
    for (int i = 0; i < LARGEUR; i += 1)
    {
        for (int y = 0; y < LONGUEUR; y += 1)
        {
            if (t[i][y] == -2)
            {
                count += 1;
            }
        }
    }
    if (count == NMINE)
    {
        GameWin = true;
    }
}

void mine(int **t, bool **tshow)
{
    tshow[X][Y] = true;
    if (t[X][Y] == -1)
    {
        GameOver = true;
    }
}

void flag(int **t, bool **tshow)
{
    /* -2 == Good Flag -3 == Bad Flag
     * Good Flag == Flag on mine
     */

    if (t[X][Y] == -1)
    {
        tshow[X][Y] = true;
        t[X][Y] = -2;
    }
    else
    {
        tshow[X][Y] = true;
        t[X][Y] = -3;
    }
}

void printGame(int **t, bool **tshow, bool Curse)
{
    printf("Game: Mine remaining: %d\n", countAllMine(t));
    for (int i = 0; i < LARGEUR; i += 1)
    {
        for (int y = 0; y < LONGUEUR; y += 1)
        {

            if (Curse)
            {
                if (i == X && y == Y)
                {
                    cout << "_ ";
                    continue;
                }
            }

            if (tshow[i][y])
            {
                if (t[i][y] == -1)
                {
                    cout << "# ";
                }
                else if (t[i][y] == -2)
                {
                    cout << "$ ";
                }
                else if (t[i][y] == -3)
                {
                    cout << "$ ";
                }
                else
                {
                    if (t[i][y] != 0)
                    {
                        cout << t[i][y] << " ";
                    }
                    else
                    {
                        cout << "  ";
                    }
                }
            }
            else
            {
                cout << "@ ";
            }
        }
        cout << endl;
    }
}

/**
 * Compte le nombre total de mine restante sur la matrice de jeu
 * @param t la matrice de jeu
 * @return le nombre total de mine sur la matrice de jeu
 */
int countAllMine(int **t)
{
    int count = 0;
    for (int i = 0; i < LARGEUR; i += 1)
    {
        for (int y = 0; y < LONGUEUR; y += 1)
        {
            if (t[i][y] == -1)
            {
                count += 1;
            }
        }
    }
    return count;
}

/**
 * Place les mine de façon aléatoire
 * @param t la matrice de jeu
 * @return void
 */
void placeMine(int **t)
{
    int decount = NMINE;
    srand(time(NULL));
    while (decount > 0)
    {
        int x = rand() % LARGEUR;
        int y = rand() % LONGUEUR;
        if (t[x][y] != -1)
        {
            t[x][y] = -1;
            decount -= 1;
            // printf("new mine at: %d, %d \n mine rest:%d", x, y, decount);
        }
    }

    for (int i = 0; i < LARGEUR; i += 1)
    {
        for (int y = 0; y < LONGUEUR; y += 1)
        {
            t[i][y] = countMine(t, i, y);
        }
    }
}

/**
 * donne le nombre de mine autour d'un block de la matrice donnée en paramètre
 * @param t la matrice
 * @param a la coordonnée en longueur
 * @param b la coordonnée en largeur
 */
int countMine(int **t, int a, int b)
{
    int count = 0;

    if (t[a][b] != 0)
    {
        return -1;
    }

    for (int i = a - 1; i < a + 2; i += 1)
    {
        for (int y = b - 1; y < b + 2; y += 1)
        {
            if (i >= 0 && i < LARGEUR)
            {
                if (y >= 0 && y < LONGUEUR)
                {
                    // printf("a: %d and b: %d\n", i, y);
                    if (t[i][y] == -1)
                    {
                        count += 1;
                    }
                }
            }
        }
    }
    return count;
}

/**
 * Crée un bool** de largeur a et de longueur b
 * @param a la largeur
 * @param b la longueur
 * @return un bool** de largeur a et de longueur b
 */
bool **newArrayBool(int a, int b)
{
    bool **low = (bool **)malloc(sizeof(int *) * a);
    for (int i = 0; i < a; i += 1)
    {
        low[i] = (bool *)malloc(sizeof(int *) * b);
        for (int y = 0; y < b; y += 1)
        {
            low[i][y] = false;
        }
    }
    return low;
}

/**
 * Crée un int** de largeur a et de longueur b
 * @param a la largeur
 * @param b la longueur
 * @return un int** de largeur a et de longueur b
 */
int **newArray(int a, int b)
{
    int **low = (int **)malloc(sizeof(int *) * a);
    for (int i = 0; i < a; i += 1)
    {
        low[i] = (int *)malloc(sizeof(int *) * b);
        for (int y = 0; y < b; y += 1)
        {
            low[i][y] = 0;
        }
    }
    return low;
}

/**
 * Crée un int* de longueur a
 * @param a la longueur
 * @return un int* de longueur a
 */
int *newArray(int a)
{
    int *low = (int *)malloc(sizeof(int *) * a);
    for (int i = 0; i < a; i += 1)
    {
        low[i] = 0;
    }
    return low;
}

/**
 * Affiche dans la console un int** de longueur b et de largeur a
 * @param t le int**
 * @param a la largeur
 * @param b la longueur
 * @return void
 */
void showArray2D(int **t, int a, int b)
{
    for (int i = 0; i < a; i += 1)
    {
        for (int y = 0; y < b; y += 1)
        {
            cout << t[i][y] << " ";
        }
        cout << endl;
    }
}

/**
 * Affiche dans la console un bool** de longueur b et de largeur a
 * @param t le bool**
 * @param a la largeur
 * @param b la longueur
 * @return void
 */
void showArray2DBool(bool **t, int a, int b)
{
    for (int i = 0; i < a; i += 1)
    {
        for (int y = 0; y < b; y += 1)
        {
            cout << t[i][y] << " ";
        }
        cout << endl;
    }
}
