// Initialise une tourelle
Defender* initTurret(int type, int line, int position);

// Ajoute une tourelle a la liste
void addTurret(Defender** list, Defender* turret);

// Supprime une tourelle de la liste
void supprTurret(Defender** list, Defender* turret);


// Initialise un ennemie
Attacker* initEnemy(int type, int line, int turn);

// Ajoute un enemy a la liste
void addEnemy(Attacker** list, Attacker* enemy);

// Supprime un enemy de la liste
void supprEnemy(Attacker** list, Attacker* enemy);

// Calcule l'ordre sur les ligne
void orderLineList(Attacker* list);
