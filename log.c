#include<stdio.h>
#include<stdbool.h>
#include "carta.h"


void debug(char *message) { fprintf(stderr, "%s\n", message); }

void imprime_carta(Carta carta){
   fprintf(stderr, "carta: %s mao: %d minha: %d especial: %d efeito: %c numero: %d naipe: %s indAdversario: %d mudaNaipe: %s\n", 
     carta.strCarta, carta.mao, carta.minha, carta.especial, carta.efeito, carta.numero, carta.naipe, carta.indAdversario, carta.mudaNaipe);
}
