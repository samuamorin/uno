#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h>;
#include "carta.h"

char** le_jogadores(char temp[], int* totalJogadores) {
  char **jogadores = NULL;
  int i = 0;

  char *jogador = strtok(temp, " \n");

  while (jogador != NULL) {
    if (i == 0) {
      jogadores = malloc(sizeof(char *));
    } else {
      jogadores = realloc(jogadores, sizeof(char *) * (i + 1));
    }

    jogadores[i] = malloc(sizeof(char) * (strlen(jogador) + 1));
    strcpy(jogadores[i], jogador);
    jogador = strtok(NULL, " \n");
    i++;
  }

  *totalJogadores = i;
  return jogadores;
}

void preenche_naipe(char *strCarta, int inicio, char strNaipe[4]) {
  
  int j = 0;
  char strNaipeAux[4];
  int tamanho = (int)strlen(strCarta);

  for (int i = inicio; i < tamanho; i++) {
    strNaipeAux[j] = strCarta[i];
    j++;
  }

  strcpy(strNaipe, strNaipeAux);

}

Carta criar_carta(char *strCarta, bool minha, bool mao, int indiceAdversario) {

  Carta carta;
  carta.mao = mao;
  carta.minha = minha;
  carta.indAdversario = -1;
  carta.mudaNaipe = NULL;

  if(indiceAdversario>=0){
    carta.indAdversario = indiceAdversario;
  }

  char strNaipe[4] = "    ";

  strcpy(carta.strCarta, strCarta);

  if(strCarta[0]=='D' || strCarta[0]=='V' || 
     strCarta[0]=='R' || strCarta[0]=='C' || 
     strCarta[0]=='A'){

     carta.especial=true;
     carta.efeito = strCarta[0];
     carta.numero = 0;
     preenche_naipe(strCarta, 1, carta.naipe);
  
  }else if (strCarta[0] == '1' && strCarta[1] == '0') {
    carta.especial = false;
    carta.numero = 10;
    carta.efeito = ' ';
    preenche_naipe(strCarta, 2, carta.naipe);
  } else {
    carta.especial = false;
    carta.numero = (int) strCarta[0] - 48;
    carta.efeito = ' ';
    preenche_naipe(strCarta, 1, carta.naipe);
  }
  return carta;
}

Carta* le_cartas_hand(char temp[], int* totalcartas, Carta* cartas) {

  int i = *totalcartas;
  char *strCarta = strtok(temp, " ]");

  while (strCarta != NULL) {
    if (i > 0) {
      cartas = realloc(cartas, sizeof(Carta) * (i + 1));
    }

    cartas[i] = criar_carta(strCarta, true, true, -1);

    strCarta = strtok(NULL, " ]");
    i++;
  }

  *totalcartas = i;
  return cartas;
}

Carta* le_carta_table(char temp[], int* totalcartas, Carta* cartas, bool minha, int indiceAdversario) {

  int i = *totalcartas;
  char *strCarta = temp;

  if (i > 0) {
    cartas = realloc(cartas, sizeof(Carta) * (i + 1));
  }

  cartas[i] = criar_carta(strCarta, minha, minha, indiceAdversario);

  i++;
  
  *totalcartas = i;
  return cartas;
}

Carta* compra_carta(int quantidade, char action[], int* totalCartas, Carta* cartas){
  Carta *cartas_compradas = cartas; 
  printf("BUY %d\n", quantidade);
  for(int i=0; i<quantidade; i++){
    scanf("%s ", action);
    cartas_compradas = le_carta_table(action, totalCartas, cartas_compradas, true, -1);
  }

  return cartas_compradas;
}