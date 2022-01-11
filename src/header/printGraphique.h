#define MAX_LINE 6
#define MAX_COLUMN 20

// dessine un bouton
void drawButton(Button button);

// regarde si le joueur click sur un bouton
int clickButton(Button list[10], int tailleL, int x, int y);

// dessine une list de bouton
void drawMenu(Button lstButton[10], int taille);

// dessine le plateau de jeu
void drawBoard (Defender* turrets[MAX_LINE+2][MAX_COLUMN+1], Attacker* enemys[MAX_LINE+2][MAX_COLUMN+1], Game game, Button lstButton[10]);

// donne le prix d'un tourelle selon son prix
int getTurretPrice(int type);

// dessine les iformations dans le cadre d'information
void drawPopUp (int x, int y, Button lstButton[10]);

// change l'image et l'action des bouton sile joueur a acces au niveau
void unlockLevel(Button lstButton[10], int lvCampagne);
