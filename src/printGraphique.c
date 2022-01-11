/**
 * @file printGraphique.c
 * @author Sébastien SAUTIER/ Léo ATTIG
 * @brief Fichier gérant l'interface graphique du jeu
 * @version 1.0
 * @date 2022-01-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <MLV/MLV_all.h>
#include "header/structure.h"
#include "header/printGraphique.h"

#define MAX_LINE 6
#define MAX_COLUMN 20
#define TAILLE_CASE 75

/**
 * @brief Fonction donnant le prix des tourelles en fonction de leur type
 * 
 * @param type Entier corespondant à une tourelle
 * @return int Retourne le prix de la tourelle demandé ou -1 si il y a un problème
 */
int getTurretPrice(int type)
{
    switch (type)
    {
    case 1:
        return 20;
        break;
    case 2:
        return 50;
        break;
    case 3:
        return 50;
        break;
    case 4:
        return 100;
        break;
    case 5:
        return 200;
        break;
    }
    return -1;
}

/**
 * @brief Fonction donnant le nom des tourelles en fonction de leur type
 * 
 * @param type Entier corespondant à une tourelle
 * @return char* Retourne le nom de la tourelle demandé ou un vide si il y a un problème
 */
char* getTurretName(int type)
{
    switch (type)
    {
    case 1:
        return "Pion";
        break;
    case 2:
        return "Cavalier";
        break;
    case 3:
        return "Tour";
        break;
    case 4:
        return "Fou";
        break;
    case 5:
        return "Dame";
        break;
    }
    return "";
}

/**
 * @brief Fonction donnant le la vie maximal des tourelles en fonction de leur type
 * 
 * @param type Entier corespondant à une tourelle
 * @return int Retourne la vie maximal de la tourelle demandé ou 0 si il y a un problème
 */
int getTurretMaxLife(int type)
{
    switch (type)
    {
    case 1:
        return 5;
        break;
    case 2:
        return 10;
        break;
    case 3:
        return 30;
        break;
    case 4:
        return 1;
        break;
    case 5:
        return 25;
        break;
    }
    return 0;
}

/**
 * @brief Fonction donnant le la vie maximal des ennemies en fonction de leur type 
 * 
 * @param type Entier corespondant à un ennemies
 * @return int Retourne la vie maximal de l'ennemie demandé ou 0 si il y a un problème
 */
int getEnemyMaxLife(int type)
{
    switch (type)
    {
    case 1:
        return 10;
        break;
    case 2:
        return 20;
        break;
    case 3:
        return 30;
        break;
    case 4:
        return 10;
        break;
    case 5:
        return 50;
        break;
    }
    return 0;
}

/**
 * @brief Fonction dessinant un bouton
 * 
 * @param button Les caractéristiques du bouton
 */
void drawButton(Button button)
{
    if (!strcmp(button.image, ""))
    {
        MLV_draw_text_box(button.x, button.y, button.tailleX, button.tailleY, button.text,
                      0, MLV_COLOR_BLACK, MLV_COLOR_BLACK, MLV_COLOR_WHITE, MLV_TEXT_CENTER,
                      MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    } else {
        MLV_Image* image = MLV_load_image(button.image);
        MLV_draw_image(image, button.x, button.y);
        MLV_free_image(image);
    }
}

/**
 * @brief Fonction qui gére les clicks sur un bouton
 * 
 * @param list Liste des boutons
 * @param tailleL 
 * @param x coordonnées en x
 * @param y coordonnées en y
 * @return int Retourne l'action qui corespond au chiffre du bouton
 */
int clickButton(Button* list, int tailleL, int x, int y)
{
    for (int i = 0; i < tailleL; i++)
    {
        if (x > ((*(list + i))).x && x < ((*(list + i)).x + (*(list + i)).tailleX)
            && y > (*(list + i)).y && y < ((*(list + i)).y + (*(list + i)).tailleY))
        {
            return (*(list + i)).action;
        }
    }
    return 0;
}

/**
 * @brief Dessine le menu
 * 
 * @param lstButton Liste des boutons
 * @param taille La taile voulue
 */
void drawMenu(Button lstButton[10], int taille)
{
    for (int i = 0; i < taille; i++)
    {
        drawButton(lstButton[i]);
    }
    
}

/**
 * @brief Fonction dessinant les barres de vie 
 * 
 * @param enemy l'ennemie en question
 * @param turret le défenseur en question
 * @param x coordonnées en x
 * @param y coordonnées en y
 */
void drawHealthBar(Attacker* enemy, Defender* turret, int x, int y)
{
    if (turret != NULL)
    {
        int maxLife = getTurretMaxLife(turret->type);
        MLV_draw_filled_rectangle(x, y+70, 75, 5, MLV_COLOR_RED);
        int width = (int)(75 * ((float)turret->life / maxLife)); // Ration de "remplisage"
        MLV_draw_filled_rectangle(x, y+70, width, 5, MLV_COLOR_GREEN1);
    }else 
    {
        int maxLife = getEnemyMaxLife(enemy->type);
        MLV_draw_filled_rectangle(x, y+70, 75, 5, MLV_COLOR_RED);
        int width = (int)(75 * ((float)enemy->life / maxLife)); // Ration de "remplisage"
        MLV_draw_filled_rectangle(x, y+70, width, 5, MLV_COLOR_GREEN1);
    }
}

/**
 * @brief Fonction qui dessine les informations de jeu
 * 
 * @param turrets la tourelles qu'on veut
 * @param enemys l'ennenie qu'on veut 
 * @param game les parametres de la partie
 * @param lstButton la liste des boutons 
 */
void drawBoard (Defender* turrets[MAX_LINE+2][MAX_COLUMN+1], Attacker* enemys[MAX_LINE+2][MAX_COLUMN+1], Game game, Button lstButton[10])
{
    // info partie
    char integerMoney[20];
    char moneyStr[100] = "Argent: ";
    sprintf(integerMoney, "%d", game.money);
    strcat(moneyStr, integerMoney);
    MLV_draw_text(30, 100, moneyStr, MLV_COLOR_BLACK);
    char integerWave[10];
    char waveStr[100] = "Tour: ";
    sprintf(integerWave, "%d", game.turn);
    strcat(waveStr, integerWave);
    MLV_draw_text(30, 125, waveStr, MLV_COLOR_BLACK);

    // boite popUp
    MLV_draw_rectangle(150, 25, 300, 175, MLV_COLOR_BLACK);

    // dessine les bouttons
    drawMenu(lstButton, 6);
    for (int i = 0; i < 5; i++)
    {
        MLV_draw_text(lstButton[i].x+10, 110, getTurretName(lstButton[i].action), MLV_COLOR_BLACK);
        char price[5]; char priceStr[100] = "prix: ";
        sprintf(price, "%d", getTurretPrice(lstButton[i].action));
        strcat(priceStr, price);
        MLV_draw_text(lstButton[i].x+10, 135, priceStr, MLV_COLOR_BLACK);
    }
    

    // dessine le board de jeu
    MLV_Color color = MLV_rgba(225, 225, 225, 255);
    for (int x = 1; x <= MAX_COLUMN; x++)
    {
       for (int y = 1; y <= MAX_LINE; y++)
       {
            MLV_draw_filled_rectangle((25+(x-1)*TAILLE_CASE), (225+(y-1)*TAILLE_CASE), TAILLE_CASE,
                                      TAILLE_CASE, color);

            if (turrets[y][x] != NULL)
            {
                char typeStr[3]; char path[100] = "src/img/white/";
                sprintf(typeStr, "%d", turrets[y][x]->type); // On transform l'int en str
                strcat(path, typeStr);
                strcat(path, ".png");
                MLV_Image* image = MLV_load_image(path);
                MLV_draw_image(image, (25+(x-1)*TAILLE_CASE), (225+(y-1)*TAILLE_CASE));
                MLV_free_image(image);
                drawHealthBar(NULL, turrets[y][x], (25+(x-1)*TAILLE_CASE), (225+(y-1)*TAILLE_CASE));
            }if (enemys[y][x] != NULL)
            {
                char typeStr[3]; char path[100] = "src/img/black/";
                sprintf(typeStr, "%d", enemys[y][x]->type); // On transform l'int en str
                strcat(path, typeStr);
                strcat(path, ".png");
                MLV_Image* image = MLV_load_image(path);
                MLV_draw_image(image, (25+(x-1)*TAILLE_CASE), (225+(y-1)*TAILLE_CASE));
                MLV_free_image(image);
                drawHealthBar(enemys[y][x], NULL, (25+(x-1)*TAILLE_CASE), (225+(y-1)*TAILLE_CASE));
            }
            color = color == MLV_rgba(225, 225, 225, 255) ? MLV_rgba(0, 120, 180, 255) : MLV_rgba(225, 225, 225, 255);
       }
       color = color == MLV_rgba(225, 225, 225, 255) ? MLV_rgba(0, 120, 180, 255) : MLV_rgba(225, 225, 225, 255);
    }
    MLV_actualise_window();
}

/**
 * @brief Fonction qui gere l'affichage des info sur les tourelles
 * 
 * @param x coordonnées en x
 * @param y coordonnées en y
 * @param lstButton la liste des boutons
 */
void drawPopUp (int x, int y, Button lstButton[10])
{
    int cursor = clickButton(lstButton, 6, x, y);
    char desc[200] = ""; char life[20] = ""; char damage[20]= "";
    switch (cursor)
    {
    case 1:
        strcpy(life, "Vie: 5");
        strcpy(damage, "Degats: 1");
        strcpy(desc, "Attaque simplement devant lui");
        break;
    case 2:
        strcpy(life, "Vie: 10");
        strcpy(damage, "Degats: 1");
        strcpy(desc, "Attaque au-dessous et en-dessous");
        break;
    case 3:
        strcpy(life, "Vie: 30");
        strcpy(damage, "Degats: 0");
        strcpy(desc, "Elle n'attaque pas");
        break;
    case 4:
        strcpy(life, "Vie: 0");
        strcpy(damage, "Degats: 0");
        strcpy(desc, "Mine, elle explose avec l'ennemie");
        break;
    case 5:
        strcpy(life, "Vie: 25");
        strcpy(damage, "Degats: 5");
        strcpy(desc, "Attaque devant, dessus et dessous");
        break;
    }
    MLV_draw_text(160, 30, life, MLV_COLOR_BLACK);
    MLV_draw_text(160, 50, damage, MLV_COLOR_BLACK);
    MLV_draw_text(160, 70, desc, MLV_COLOR_BLACK);
    MLV_actualise_window();
}

void unlockLevel(Button lstButton[10], int lvCampagne)
{
    switch (lvCampagne)
    {
    case 5:
        lstButton[4].action = 5;
        strcpy(lstButton[4].image, "src/img/button/nv5.png");
    case 4:
        lstButton[3].action = 4;
        strcpy(lstButton[3].image, "src/img/button/nv4.png");
    case 3:
        lstButton[2].action = 3;
        strcpy(lstButton[2].image, "src/img/button/nv3.png");
    case 2:
        lstButton[1].action = 2;
        strcpy(lstButton[1].image, "src/img/button/nv2.png");
    }
    
}