void verifica_adversarios(char **jogadores, int totalJogadores, int adversarios[], char my_id[]);
void troca_adversarios(int adversarios[]);
char* escolhe_proximo_naipe(Carta cartasProxOponente[], int totalCartasProxOponente, Carta* cartas, int *totalCartas);
int escolhe_carta(Carta* cartas, int* totalCartas, int* totalCartasMesa, Carta cartaMesa, char secondComplement[], Carta* cartasAdversarios, int* totalAdversarios, int adversario);