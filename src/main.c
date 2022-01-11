/**
 * @file main.c
 * @author Sébastien SAUTIER / Léo ATTIG
 * @brief Fichier principale 
 * @version 1.0
 * @date 2022-01-06
 */
// std include
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <MLV/MLV_all.h>
// personal include
#include "header/structure.h"
#include "header/printConsol.h"
#include "header/linkedList.h"
#include "header/printGraphique.h"

#define CLEAR printf("\e[1;1H\e[2J")
#define FRAME_RATE 10 // le temps d'attente entre 2 frame en milliseconde (environ 60FPS)
// taille de la fenetre
#define TAILLE_X 1550
#define TAILLE_Y 700
// taille du plateau
#define TAILLE_PLATEAU_X 20 // c'est aussi le nombre maximum de vague
#define TAILLE_PLATEAU_Y 6

/**
 * @brief Fonction initialisant le jeu
 * 
 * @param game La structure avec les informations d'une partie
 * @param nv Le nom du fichier du niveau
 * @return Compte rendu d'erreur : 1 si tout vas bien, 0 si le nom du fichier est incorrect
 */
// initialisation d'une partie
int initGame(Game* game, char* nv)
{
    // On creer le chemin complet du niveau
    char path[100] =  "src/games/";
    strcat(path, nv);
    strcat(path, ".nv");
    
    FILE* file = fopen(path, "r");
    if (file != NULL) // Si le fichier a bien été trouver
    {
        game->attackers = NULL;

        fscanf(file, "%d", &game->money); // On recupère l'argent
        while (fgetc(file) != EOF) // Si on est pas a la fin alors on doit creer un nouveauc mechant
        {
            int type, line, turn;
            fscanf(file, "%d %d %d", &turn, &line, &type);
            Attacker* enemy = initEnemy(type, line, turn);
            if (enemy != NULL) {
                addEnemy(&game->attackers, enemy);
            }else {
                printf("ERREUR DE MEMOIRE");
                exit(-1); // On kill le problème après l'erreur d'allocation
            }
        }
        fclose(file);

        game->defenders = NULL;
        game->turn = 0;
    }else {
        printf("ERREUR DANS LE NOM DU FICHIER");
        return 0;
    }
    
    return 1;
}

/**
 * @brief Fonction qui regarde si les pieces qu'on veut placer sont bien sur le plateau
 * 
 * @param turrets Les tourelles de défenses avec leur coordonnées
 * @param enemys Les ennemies avec leur coordonées
 * @param y coordonnées de la ligne
 * @param x coordonnées de la position
 * @return int Compte rendu d'erreur : 1 si il y a un problème, 0 si tout vas bien
 */
// Regarde si il y a une collision au coordonnee x, y
int checkColision(Defender* turrets[TAILLE_PLATEAU_Y+1][TAILLE_PLATEAU_X+1], Attacker* enemys[TAILLE_PLATEAU_Y+1][TAILLE_PLATEAU_X+1], int y, int x)
{
    if (y < 1 || y > 6 || x < 1 || x > 20)
    {
        return 1;
    }
    
    if (turrets[y][x] != NULL)
    {
        return 1;
    }
    if (enemys[y][x] != NULL)
    {
        return 1;
    }
    return 0;
}

/**
 * @brief Fonction qui gere les prochains tour, qui regarde si l'ennemie peut avancer, s'il doit prendre ou faire des dégats ou si il est encore en vie
 * 
 * @param game Prends les paramètre de la partie
 * @param turrets Les tourelles de défenses avec leur coordonnées
 * @param enemys Les ennemies avec leur coordonées
 * @return int Compte rendu de fin de partie : 1 si la partie est perdu et 0 si elle est gagné
 */
int nextTurnConsol(Game* game, Defender* turrets[TAILLE_PLATEAU_Y+2][TAILLE_PLATEAU_X+1], Attacker* enemys[TAILLE_PLATEAU_Y+1][TAILLE_PLATEAU_X+1])
{
    // Infliger les degats aux enemy
    Defender* turret = game->defenders;
    while (turret != NULL)
    {
        switch (turret->type)
        {
        case 1: // Pion
            for (int i = turret->position; i <= TAILLE_PLATEAU_X; i++)
            {
                if (enemys[turret->line][i] != NULL)
                {
                    enemys[turret->line][i]->life -= 1;
                    if (enemys[turret->line][i]->life <= 0)
                    {
                        game->money += enemys[turret->line][i]->reward;
                        supprEnemy(&game->attackers, enemys[turret->line][i]);
                        enemys[turret->line][i] = NULL;
                    }
                    break;
                }
            }
            break;
        case 2: // Cavalier
            for (int l = -1; l < 2; l+=2) // Pour la ligne du dessus et du dessous
            {
                for (int i = turret->position; i <= TAILLE_PLATEAU_X; i++)
                {
                    if (enemys[turret->line+l][i] != NULL)
                    {
                        enemys[turret->line+l][i]->life -= 2;
                        if (enemys[turret->line+l][i]->life <= 0)
                        {
                            game->money += enemys[turret->line+l][i]->reward;
                            supprEnemy(&game->attackers, enemys[turret->line+l][i]);
                            enemys[turret->line+l][i] = NULL;
                        }
                        break;
                    }
                }
            }
            break;
        case 4: // Fou
            // Les Dame adverse ne sont pas affecter par les mines
            if (enemys[turret->line][turret->position+1] != NULL && enemys[turret->line][turret->position+1]->type != 5)
            {
                // On detruit l'enemy puis le fou
                game->money += enemys[turret->line][turret->position+1]->reward;
                supprEnemy(&game->attackers, enemys[turret->line][turret->position+1]);
                enemys[turret->line][turret->position+1] = NULL;
                int line = turret->line; int position = turret->position; // On sauvegarde la position
                turret = turret->next; // On affecte la prochaine tourelle avant de la detruire
                supprTurret(&game->defenders, turrets[line][position]);
                turrets[line][position] = NULL;
                continue; // On passe directe a la prochaine iteration
            }
            break;
        case 5: // Dame
            for (int l = -1; l < 2; l++) // Pour la ligne du dessus, devant et du dessous
            {
                for (int i = turret->position; i <= TAILLE_PLATEAU_X; i++)
                {
                    if (enemys[turret->line+l][i] != NULL)
                    {
                        enemys[turret->line+l][i]->life -= 5;
                        if (enemys[turret->line+l][i]->life <= 0)
                        {
                            game->money += enemys[turret->line+l][i]->reward;
                            supprEnemy(&game->attackers, enemys[turret->line+l][i]);
                            enemys[turret->line+l][i] = NULL;
                        }
                        break;
                    }
                }
            }
            break;
        }
        
        turret = turret->next;
    }
    
    // Avancer les enemy déjà présant
    for (int y = 1; y <= TAILLE_PLATEAU_Y; y++)
    {
        for (int x = 1; x <= TAILLE_PLATEAU_X; x++)
        {
            if (enemys[y][x] != NULL) // Pour chaque enemy
            {
                if (checkColision(turrets, enemys, y, x-1)) // Si il y a quelque chose devant
                {
                    if (turrets[y][x-1] != NULL) // Si il y a une tourelle devant
                    {
                        switch (enemys[y][x]->type)
                        {
                        case 1: // Pion
                            turrets[y][x-1]->life -= 1;
                            if (turrets[y][x-1]->life <= 0)
                            {
                                supprTurret(&game->defenders, turrets[y][x-1]);
                                turrets[y][x-1] = NULL;
                            }
                            break;
                        case 2: // Cavalier
                            for (int l = -1; l < 2; l++)
                            {
                                if (turrets[y+l][x-1] != NULL)
                                {
                                    turrets[y+l][x-1]->life -= 1;
                                    if (turrets[y+l][x-1]->life <= 0)
                                    {
                                        supprTurret(&game->defenders, turrets[y+l][x-1]);
                                        turrets[y+l][x-1] = NULL;
                                    }
                                }
                            }
                            break;
                        case 3: // Tour
                            turrets[y][x-1]->life -= 1;
                            if (turrets[y][x-1]->life <= 0)
                            {
                                supprTurret(&game->defenders, turrets[y][x-1]);
                                turrets[y][x-1] = NULL;
                            }
                            break;
                        case 4: // Fou
                            supprTurret(&game->defenders, turrets[y][x-1]);
                            turrets[y][x-1] = NULL;
                            break;
                        case 5: // Dame
                            turrets[y][x-1]->life -= 5;
                            if (turrets[y][x-1]->life <= 0)
                            {
                                supprTurret(&game->defenders, turrets[y][x-1]);
                                turrets[y][x-1] = NULL;
                            }
                            break;
                        }
                    }
                }else { // Si y'a rien devant On avance 
                    enemys[y][x-1] = enemys[y][x];
                    enemys[y][x] = NULL;
                    if (x-1 <= 1) {
                        return 1;
                    }
                }
            }
        }
    }

    // Aparaitre les nouveaux enemy
    Attacker* enemy = game->attackers;
    while (enemy != NULL)
    {
        if (enemy->turn == game->turn)
        {
            if (!checkColision(turrets, enemys, enemy->line, TAILLE_PLATEAU_X))
            {
                enemys[enemy->line][TAILLE_PLATEAU_X] = enemy;
            }else {
                enemy->turn++;
            }
        }
        enemy = enemy->next;
    }

    return 0;
}

/**
 * @brief Transforme les pixels en case de notre plateau
 * 
 * @param x la coordonnées en x
 * @param y la coordonnées en y
 */
void pixelToBoard(int* x, int* y)
{
    *x = ((*x - 25)/75)+1;
    *y = ((*y - 225)/75)+1;
}

int loadSave()
{
    FILE* file = fopen("src/save.sv", "r");
    if (file != NULL) // Si le fichier a bien été trouver
    {
        int lvSave = 0;
        fscanf(file, "%d", &lvSave); // On recupère l'argent
        fclose(file);

        return lvSave;
    }else {
        printf("ERREUR DANS LE CHARGEMENT DE LA SAUVEGARDE");
        return 0;
    }
}

void saveSave(int lvSave)
{
    FILE* file = fopen("src/save.sv", "w");
    if (file != NULL) // Si le fichier a bien été trouver
    {
        fprintf(file, "%d", lvSave); // On recupère l'argent
        fclose(file);
    }else {
        printf("ERREUR DANS LE CHARGEMENT DE LA SAUVEGARDE");
    }
}

/**
 * @brief Fonction permettant l'éxécution du jeu dans son ensemble.
 * @return Compte rendu d'erreur : 0 si tout vas bien 
 */
int main(int argc, char const *argv[])
{
    // Variable
    int graphMod = 1; // O = non, 1 = oui

    // le jeux doit il être en mode graphique
    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-c")) {
            graphMod = 0;
        }
        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            printf("-c : lance le jeu en mode console\n");
            return 0; // si le joueur demande de l'aide on ne lance pas le jeu
        }
    }
    
    if (!graphMod) // mode console
    {
        while (1) // boucle du jeu
        {
            int cursor;
            char nv[100] = "perso";
            int isRunnig = 1;
            while (isRunnig) // boucle de menue
            {
                CLEAR;
                printMenuC();
                cursor = choiceMenuC();
                switch (cursor)
                {
                case 1:
                    CLEAR;
                    isRunnig = 0;
                    break;
                case 2:
                    CLEAR;
                    printRulesC();
                    break;
                case 3:
                    CLEAR;
                    printf("merci d'avoir jouer\n");
                    return 0;
                }
            }
            isRunnig = 1;

            // Initialisation de la partie
            Game game;
            initGame(&game, nv);
            // +2 sur y pour eviter les erreur d'index avec les tourelle qui tire sur d'autre ligne
            Attacker* enemys[TAILLE_PLATEAU_Y+2][TAILLE_PLATEAU_X+1] = {{NULL}};
            Defender* turrets[TAILLE_PLATEAU_Y+2][TAILLE_PLATEAU_X+1] = {{NULL}};


            // debut de la partie
            printf("La vague a venir:\n");
            printWaveC(game.attackers);
            printf("\n(appuyer sur entrer pour lancer la partie)\n");
            while (getchar() != '\n' && getchar() != EOF); // On vide le buffer pour être sur qu'un retour chariot ne s'y cache pas déjà
            while (getchar() != '\n');

            while (isRunnig) // boucle de jeu
            {
                CLEAR;
                printBoardC(turrets, enemys, game);
                printf("1-Tour suivant | 2-Construire | 3-Quitter le jeu\n");
                cursor = choiceMenuC();
                switch (cursor)
                {
                case 1:
                    break;
                case 2:
                    while (1)
                    {
                        cursor = ShopC(&game);
                        if (cursor == 0) { // On sort du shop
                            break;
                        } else {
                            int line, position;
                            printf("Coordonnes de la tourelle: (ligne colone):\n");
                            scanf(" %d %d", &line, &position);
                            // On regarde si la place est libre
                            while (checkColision(turrets, enemys, line, position))
                            {
                                printf("COORDONNES INVALIDE !\n");
                                scanf(" %d %d", &line, &position);
                            }

                            Defender* turret = initTurret(cursor, line, position);
                            addTurret(&game.defenders, turret);
                            turrets[line][position] = turret;
                        }
                    }
                    
                    break;
                case 3:
                    return 0;
                }
                game.turn++;
                cursor = nextTurnConsol(&game, turrets, enemys);

                if (cursor) // si le joueur a perdu
                {
                    CLEAR;
                    printf("\nPERDU !!\n");

                    printf("\n (appuyer sur entrer pour quitter au menu)");
                    while (getchar() != '\n' && getchar() != EOF); // On vide le buffer pour être sur qu'un retour chariot ne s'y cache pas déjà
                    while (getchar() != '\n');
                    isRunnig = 0;
                }
                if (game.attackers == NULL) // si le joueur a gagner (eliminer tout les enemies)
                {
                    CLEAR;
                    printf("\nVICTOIRE !!\n");

                    printf("\n (appuyer sur entrer pour quitter au menu)");
                    while (getchar() != '\n' && getchar() != EOF); // On vide le buffer pour être sur qu'un retour chariot ne s'y cache pas déjà
                    while (getchar() != '\n');
                    isRunnig = 0;
                }
            }
            isRunnig = 1;
        }
        
    }else // Mode graphique
    {
        MLV_create_window("King vs King", "", TAILLE_X, TAILLE_Y);
        int cursor, cursorX, cursorY;
        int selctedLv = 0;
        char nv[100];
        int isRunning = 1;

        while (1)
        {
            int lvCampagne = loadSave();
            while(isRunning) // boucle de menu
            {
                // Creation du menu
                Button campagne = {525, 300, 500, 150, "JOUER", "src/img/button/campagne.png", 1};
                Button regles = {550, 475, 200, 100, "REGLES", "src/img/button/regles.png", 2};
                Button perso = {800, 475, 200, 100, "PERSO", "src/img/button/perso.png", 3};
                Button listButton[10];
                listButton[0] = campagne;
                listButton[1] = regles;
                listButton[2] = perso;
                MLV_clear_window(MLV_COLOR_WHITE);
                MLV_Image* image = MLV_load_image("src/img/screen/menu.png");
                MLV_draw_image(image, 0, 0);
                MLV_free_image(image);
                drawMenu(listButton, 3);
                MLV_actualise_window();

                // Click dans le menu
                cursor = 0;
                while (!cursor)
                {
                    MLV_wait_mouse(&cursorX, &cursorY);
                    cursor = clickButton(listButton, 3, cursorX, cursorY);
                }
                switch (cursor)
                {
                case 1: // Ecran de campagne
                    MLV_clear_window(MLV_COLOR_WHITE);
                    Button nv1 = {225, 250, 200, 100, "nv1", "src/img/button/nv1.png", 1};
                    Button nv2 = {450, 250, 200, 100, "nv2", "src/img/button/nv2D.png", 0};
                    Button nv3 = {675, 250, 200, 100, "nv3", "src/img/button/nv3D.png", 0};
                    Button nv4 = {900, 250, 200, 100, "nv4", "src/img/button/nv4D.png", 0};
                    Button nv5 = {1125, 250, 200, 100, "nv5", "src/img/button/nv5D.png", 0};
                    Button retour = {675, 575, 200, 100, "RETOUR", "src/img/button/retour.png", 6};
                    listButton[0] = nv1;
                    listButton[1] = nv2;
                    listButton[2] = nv3;
                    listButton[3] = nv4;
                    listButton[4] = nv5;
                    listButton[5] = retour;
                    unlockLevel(listButton, lvCampagne);
                    MLV_Image* campagneImg = MLV_load_image("src/img/screen/campagne.png");
                    MLV_draw_image(campagneImg, 0, 0);
                    MLV_free_image(campagneImg);
                    drawMenu(listButton, 6);
                    MLV_actualise_window();
                    
                    MLV_wait_mouse(&cursorX, &cursorY);
                    cursor = 0;
                    while (!cursor)
                    {
                        MLV_wait_mouse(&cursorX, &cursorY);
                        cursor = clickButton(listButton, 6, cursorX, cursorY);
                    }
                    switch (cursor)
                    {
                    case 1:
                        strcpy(nv, "campagne/nv1");
                        selctedLv = 2;
                        isRunning = 0;
                        break;
                    case 2:
                        strcpy(nv, "campagne/nv2");
                        selctedLv = 3;
                        isRunning = 0;
                        break;
                    case 3:
                        strcpy(nv, "campagne/nv3");
                        selctedLv = 4;
                        isRunning = 0;
                        break;
                    case 4:
                        strcpy(nv, "campagne/nv4");
                        selctedLv = 5;
                        isRunning = 0;
                        break;
                    case 5:
                        strcpy(nv, "campagne/nv5");
                        selctedLv = 5;
                        isRunning = 0;
                        break;
                    }
                    
                    break;
                case 2: // regles
                    // On dessine l'ecran des regles
                    MLV_clear_window(MLV_COLOR_WHITE);
                    MLV_Image* rulesImg = MLV_load_image("src/img/screen/regles.png");
                    MLV_draw_image(rulesImg, 0, 0);
                    MLV_free_image(rulesImg);
                    Button back = {675, 575, 200, 100, "RETOUR", "src/img/button/retour.png", 1};
                    listButton[0] = back;
                    drawMenu(listButton, 1);
                    MLV_actualise_window();

                    MLV_wait_mouse(&cursorX, &cursorY);
                    cursor = 0;
                    while (!cursor)
                    {
                        MLV_wait_mouse(&cursorX, &cursorY);
                        cursor = clickButton(listButton, 3, cursorX, cursorY);
                    }
                    break;
                case 3: // jouer
                    strcpy(nv, "perso");
                    isRunning = 0;
                    break;
                }
            }
            isRunning = 1;

            // inialisation de la partie
            Game game;
            initGame(&game, nv);
            // +2 sur y pour eviter les erreur d'index avec les tourelle qui tire sur d'autre ligne
            Attacker* enemys[TAILLE_PLATEAU_Y+2][TAILLE_PLATEAU_X+1] = {{NULL}};
            Defender* turrets[TAILLE_PLATEAU_Y+2][TAILLE_PLATEAU_X+1] = {{NULL}};
            // Pour MLV event
            MLV_Event event;
            MLV_Mouse_button mouseButton;
            MLV_Button_state buttonState;
            int selectedType = 1;
            // liste des buttons
            Button listButton[6];
            Button pionB = {500, 25, 100, 100, "Pion", "src/img/white/1.png", 1};
            Button cavalierB = {625, 25, 100, 100, "Cavalier", "src/img/white/2.png", 2};
            Button tourB = {750, 25, 100, 100, "Tour", "src/img/white/3.png", 3};
            Button fouB = {875, 25, 100, 100, "Fou", "src/img/white/4.png", 4};
            Button dameB = {1000, 25, 100, 100, "Dame", "src/img/white/5.png", 5};
            Button nextTurnB = {1225, 50, 200, 100, "Tour suivant", "src/img/button/nextTurn.png", 6};
            listButton[0] = pionB;
            listButton[1] = cavalierB;
            listButton[2] = tourB;
            listButton[3] = fouB;
            listButton[4] = dameB;
            listButton[5] = nextTurnB;

            while (isRunning) // Boucle de jeu
            {
                MLV_clear_window(MLV_COLOR_WHITE);
                drawBoard(turrets, enemys, game, listButton); // On dessine le plateau de jeu
                drawPopUp(cursorX, cursorY, listButton); // On affiche des infos contextuel 

                event = MLV_wait_event(NULL, NULL, NULL, NULL, NULL, &cursorX,
                                      &cursorY, &mouseButton, &buttonState);

                if(event == MLV_MOUSE_BUTTON) // si il y a un click
                {
                    if (buttonState == MLV_PRESSED)
                    {
                        cursor = clickButton(listButton, 6, cursorX, cursorY);
                        if (!cursor) // si on ne clique pas sur un bouton
                        {
                            if (cursorX > 25 && cursorX < 1450 && cursorY > 225 && cursorY < 675)
                            {
                                pixelToBoard(&cursorX, &cursorY); // On convertie les pixel en case du plateau
                                int price = getTurretPrice(selectedType);
                                if (!checkColision(turrets, enemys, cursorY, cursorX) && game.money >= price)
                                {
                                    Defender* turret = initTurret(selectedType, cursorY, cursorX);
                                    if (turret != NULL)
                                    {
                                        addTurret(&game.defenders, turret);
                                        turrets[cursorY][cursorX] = turret;
                                        game.money -= price;
                                    }
                                }
                            }
                        }else {
                            if (cursor == 6) {
                                game.turn++;
                                cursor = nextTurnConsol(&game, turrets, enemys);
                                if (cursor) {
                                    MLV_clear_window(MLV_COLOR_WHITE);
                                    MLV_Image* defImg = MLV_load_image("src/img/screen/defaite.png");
                                    MLV_draw_image(defImg, 0, 0);
                                    MLV_free_image(defImg);
                                    MLV_actualise_window();
                                    isRunning = 0;
                                    MLV_wait_mouse(NULL, NULL);
                                }else if (game.attackers == NULL) {
                                    MLV_clear_window(MLV_COLOR_WHITE);
                                    MLV_Image* vicImg = MLV_load_image("src/img/screen/victoire.png");
                                    MLV_draw_image(vicImg, 0, 0);
                                    MLV_free_image(vicImg);
                                    MLV_actualise_window();
                                    if (selctedLv > lvCampagne) {
                                        saveSave(selctedLv);
                                    }
                                    isRunning = 0;
                                    MLV_wait_mouse(NULL, NULL);
                                }
                            }else {
                                selectedType = cursor;
                            }
                        }
                    }
                }
            }
            isRunning = 1;
        }
    }
    
    return 0;
}