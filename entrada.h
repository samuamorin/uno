char** le_jogadores(char temp[], int* totalJogadores);
void preenche_naipe(char *strCarta, int inicio, char strNaipe[4]);
Carta criar_carta(char *strCarta, bool minha, bool mao, int indiceAdversario);
Carta* le_cartas_hand(char temp[], int* totalcartas, Carta* cartas);
Carta* le_carta_table(char temp[], int* totalcartas, Carta* cartas, bool minha, int indiceAdversario);
Carta* compra_carta(int quantidade, char action[], int* totalCartas, Carta* cartas);