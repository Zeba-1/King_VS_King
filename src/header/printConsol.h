#define MAX_LINE 6
#define MAX_COLUMN 20

// affiche le menu du jeu
void printMenuC();

// fait entrer au joueur un choix valide dans le menu du jeu
int choiceMenuC();

// affiche les règles du jeu
void printRulesC();

// affiche la vague a arriver
void printWaveC(Attacker* enemyList);

// gere le shop
int ShopC(Game* game);

//affiche le plateau de la partie en cours
void printBoardC(Defender* turrets[MAX_LINE+1][MAX_COLUMN+1], Attacker* enemys[MAX_LINE+1][MAX_COLUMN+1], Game game);