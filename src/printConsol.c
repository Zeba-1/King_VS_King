/**
 * @file printConsol.c
 * @author Sébastien SAUTIER / Léo ATTIG
 * @brief Fichier gérant le menu de la console
 * @version 1.0
 * @date 2022-01-06
 * 
 * 
 */
#include <stdio.h>
#include <string.h>
#include "header/structure.h"
#include "header/printConsol.h"

#define MAX_LINE 6
#define MAX_COLUMN 20

/**
 * @brief Fonction gérant l'affichage du menu du jeu dans la console
 * 
 */
void printMenuC()
{
    printf("--- BIENVENUE DANS (inserer nom du jeu) ---\n\n");
    printf("1- Lancer une partie\n");
    printf("2- Regle du jeu\n");
    printf("3- QUITTER\n\n");
}

/**
 * @brief Fonction gérant les choix dans le menu du jeu via la console
 * 
 * @return Un entier en 1 et 3 qui désigne l'une des trois possiblité de la fonction printMenuC
 */
int choiceMenuC()
{
    int choice = 0;
    while ( choice > 3 || choice < 1)
    {
        printf("Votre choix: ");
        scanf(" %d", &choice);
    }
    
    return choice;
}

/**
 * @brief Fonction affichant les règles si le joueur les demandes
 * On utilise getchar vérifier qu'un retour chariot ne se cache pas et qui ne créer pas de bug
 */
void printRulesC()
{
    printf("- Vous devez placez des pieces blanche sur le plateau pour pour tuez les pieces noir qui arrivent par la droite");
    printf("\n\n");
    printf("- Des pieces noir peuvent apparaitre a tout les tours");
    printf("\n\n");
    printf("- Tuez une piece noir vous rapporte de l'argent pour acheter des pieces blanche");
    printf("\n\n");
    printf("- Si une piece noir arrivent au bout du plateau (a gauche) c'est perdu");
    printf("\n\n");
    printf("- Les pieces blanche attaque a distance alors que les pieces noirattaque au corps a corps");
    printf("\n\n");
    printf("\n (appuyer sur entrer pour quitter au menu)");

    while (getchar() != '\n' && getchar() != EOF); // On vide le buffer pour être sur qu'un retour chariot ne s'y cache pas déjà
    while (getchar() != '\n');
}

/**
 * @brief Fonction affichant les vagues d'ennemies
 * 
 * @param enemyList Les ennemies qui doivent apparaitre
 */
void printWaveC(Attacker* enemyList)
{
    // On creer un tableau 2D pour represanter la wave en approche
    Attacker* wave[MAX_LINE+1][MAX_COLUMN+1] = {{NULL}}; // +1 pour faire comme si la list començais a 1
    while (enemyList != NULL)
    {
        wave[enemyList->line][enemyList->turn] = enemyList;
        enemyList = enemyList->next;
    }

    // On affiche nôtre vague
    for (int x = 1; x <= MAX_LINE; x++)
    {
        printf("%d|", x);
        for (int y = 1; y <= MAX_COLUMN; y++)
        {
            Attacker* enemy = wave[x][y];
            if (enemy != NULL) {
                printf(" %d ", enemy->type);
            }
            else {
                printf("   ");
            }
        }
        printf("\n");
    }
}

/**
 * @brief Fonction qui affiche le menu pour acheter des tourelles dans le mode console
 * 
 * @param game Les parametres de la partie
 * @return int On retourne le choix que le joueur à fait qui est un entier qui correspond au type de la tourelle voulue
 */
int ShopC(Game* game)
{
    printf("\nArgent: %d\n\n", game->money);
    printf("1-pion (20)\n2-cavalier (50)\n3-tour (50)\n4-fou (100)\n5-dame (200)\n(0 pour quitter)\n\n");
    int choice = -1;
    while ( choice > 5 || choice < 0)
    {
        printf("Votre choix: ");
        scanf(" %d", &choice);
        switch (choice)
        {
        case 1: // Pion
            if (game->money >= 20) {
                game->money -= 20;
            }else {
                printf("PAS ASSEZ D'ARGENT !\n");
                choice = -1;
            }
            
            break;
        case 2: // Cavalier
            if (game->money >= 50) {
                game->money -= 50;
            }else {
                printf("PAS ASSEZ D'ARGENT !\n");
                choice = -1;
            }
            
            break;
        case 3: // Tour
            if (game->money >= 50) {
                game->money -= 50;
            }else {
                printf("PAS ASSEZ D'ARGENT !\n");
                choice = -1;
            }
            
            break;
        case 4: // Fou
            if (game->money >= 100) {
                game->money -= 100;
            }else {
                printf("PAS ASSEZ D'ARGENT !\n");
                choice = -1;
            }
            
            break;
        case 5: // Dame
            if (game->money >= 200) {
                game->money -= 200;
            }else {
                printf("PAS ASSEZ D'ARGENT !\n");
                choice = -1;
            }
            
            break;
        }
    }
    
    return choice;
}

/**
 * @brief On affiche les défenseur et les attaquant lors d'une partie
 * 
 * @param turrets Les tourelles alliées avec leur coordonnées
 * @param enemys Les ennemies avec leur coordonnées
 * @param turn Le tour on l'on se trouve en partie
 */
void printBoardC(Defender* turrets[MAX_LINE+1][MAX_COLUMN+1], Attacker* enemys[MAX_LINE+1][MAX_COLUMN+1], Game game)
{
    printf("Tour n°%d  |  Argent: %d\n", game.turn, game.money);
    for (int x = 1; x <= MAX_LINE; x++)
    {
        printf("%d|", x);
        for (int y = 1; y <= MAX_COLUMN; y++) 
        {
            if (turrets[x][y] != NULL) {
                printf(" %2d%d> ", turrets[x][y]->life, turrets[x][y]->type);
            }
            else if (enemys[x][y] != NULL) {
                printf(" <%2d%d ", enemys[x][y]->life, enemys[x][y]->type);
            }
            else {
                printf("   .  ");
            }
        }
        printf("\n");
    }
    printf("\n");
}