/**
 * @file linkedList.c
 * @author Sébastien SAUTIER / Léo ATTIG
 * @brief Fichier gérant les tourelles et les ennemies
 * @version 1.0
 * @date 2022-01-06
 * 
 * 
 */
// std include
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// personal include
#include "header/structure.h"
#include "header/linkedList.h"

#define TAILLE_PLATEAU_Y 6


// fonction pour les tourelles
/**
 * @brief Fonction permettant d'initiliser les tourelles des défenseurs
 * 
 * @param type Entier determinant le type de la tourelle par exemple 1 correspond à la tourelle la plus faible
 * @param line Entier determinant la ligne où se trouve la tourelle
 * @param position Entier déterminant la position où se trouve la tourelle
 * @return Defender* On renvoie la tourelle avec ces informations compléter dans grâce à la structure
 */
Defender* initTurret(int type, int line, int position)
{
    Defender* turret = malloc(sizeof(Defender)); // adresse de la tourelle
    if (turret == NULL) {
        return NULL; // l'allocation n'as pas fonctioner
    }

    turret->type = type; // type de la tourelle
    turret->line = line;
    turret->position = position;
    switch (turret->type) // vie de la tourelle
    {
    case 1: // Pion
        turret->life = 5;
        break;
    case 2: // Cavalier
        turret->life = 10;
        break;
    case 3: // Tour
        turret->life = 30;
        break;
    case 4: // Fou
        turret->life = 1;
        break;
    case 5: // Dame
        turret->life = 25;
        break;
    }
    turret->next = NULL;

    return turret;
}

/**
 * @brief Fonction permertant d'ajouter une tourelle des défenseurs à une liste
 * 
 * @param list La liste des tourelles
 * @param turret La tourelle qu'on veut ajouter
 */
void addTurret(Defender** list, Defender* turret)
{
    if (*list == NULL)
    {
        *list = turret;
        return;
    }
    
    while ((*list)->next != NULL) {
        list = &(*list)->next;
    }
    (*list)->next = turret;
}

/**
 * @brief Fonction permetant de supprimer une tourelle des défenseurs
 * 
 * @param list La liste des tourelles des défenseurs
 * @param turret La tourelles qu'on veut supprimer
 */
void supprTurret(Defender** list, Defender* turret)
{
    if (*list == NULL || turret == NULL) {
        return;
    }

    if (*list == turret) // si c'est le premier
    {
        Defender* temp = *list;
        *list = (*list)->next;
        free(temp);
    }else {
        while ((*list)->next != turret)
        {
            list = &(*list)->next;
        }
        Defender* temp = (*list)->next; // On stock celui qu'on veux suprr
        (*list)->next = (*list)->next->next; // On deconnecte de la list
        free(temp); // On libere l'espace
    }
}


// fonction pour les attaquant
/**
 * @brief Fonction itialisant les ennemies
 * 
 * @param type Entier determinant le type des ennemies par exemple 1 correspond à l'ennemies la plus faible
 * @param line Entier determinant la ligne où se trouve l'ennemie
 * @param turn Entier determinant le tour de l'ennemie
 * @return Attacker* 
 */
Attacker* initEnemy(int type, int line, int turn)
{
    Attacker* enemy = malloc(sizeof(Attacker)); // adresse de la tourelle
    if (enemy == NULL) {
        return NULL; // l'allocation n'as pas fonctioner
    }

    enemy->type = type; // type de la tourelle
    switch (enemy->type) 
    {
    case 1: // Pion
        enemy->life = 10;
        enemy->reward = 10;
        break;
    case 2: // Cavalier
        enemy->life = 20;
        enemy->reward = 20;
        break;
    case 3: // Tour
        enemy->life = 30;
        enemy->reward = 40;
        break;
    case 4: // Fou
        enemy->life = 10;
        enemy->reward = 40;
        break;
    case 5: // Dame
        enemy->life = 50;
        enemy->reward = 100;
        break;
    }
    enemy->line = line;
    enemy->turn = turn;
    enemy->position = 0; // Son avancement dans le monde (0, pas apparu)

    enemy->next = NULL;

    return enemy;
}

/**
 * @brief Fonction ajoutant un ennemie à une liste
 * 
 * @param list Liste où l'on souhaite ajouter l'ennemie
 * @param enemy L'ennemie en question
 */
void addEnemy(Attacker** list, Attacker* enemy)
{
    if (*list == NULL)
    {
        *list = enemy;
        return;
    }
    
    while ((*list)->next != NULL) {
        list = &(*list)->next;
    }
    (*list)->next = enemy;
}

/**
 * @brief Affiche la liste des ennemies
 * 
 * @param list La liste contenant les ennemies
 */
void printListe(Attacker** list)
{
    while (*list != NULL)
    {
        printf("%p->", *list);
        list = &(*list)->next;
    }
    printf("NULL\n");
}

/**
 * @brief Fonction permettant de supprimer les ennemies
 * 
 * @param list La liste où se trouve l'ennemie
 * @param enemy L'ennemie à supprimer
 */
void supprEnemy(Attacker** list, Attacker* enemy)
{
    if (*list == NULL || enemy == NULL) {
        return;
    }

    if (*list == enemy)
    {
        Attacker* temp = *list;
        *list = (*list)->next;
        free(temp);
    }else {
        while ((*list)->next != enemy)
        {
            list = &(*list)->next;
        }
        Attacker* temp = (*list)->next;
        (*list)->next = (*list)->next->next;
        free(temp);
    }
}