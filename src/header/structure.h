/**
 * @brief Structure gérant les tourelles de défense
 * 
 */
// structure des defenseur
typedef struct defender {
    int type;
    int life;
    int line;
    int position;
    // int price; // Pas besoin du prix ici
    struct defender* next;
} Defender;

/**
 * @brief Structure gérant les ennemies
 * 
 */
// structure des attaquants
typedef struct attacker {
    int type;
    int life;
    int line;
    int position;
    int reward;
    int turn;
    struct attacker* next;
    // struct attacker* next_line;
    // struct attacker* prev_line;
} Attacker;

/**
 * @brief Structure gérant la partie
 * 
 */
// structure d'une partie
typedef struct {
    Attacker* attackers;
    Defender* defenders;
    int turn;
    int money;
} Game;

/**
 * @brief Structure gérant les boutons
 * 
 */
// structure de bouton
typedef struct button
{
    int x;
    int y;
    int tailleX;
    int tailleY;
    char text[50];
    char image[100];
    int action;
} Button;

