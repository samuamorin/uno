#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 100
#define MAX_ACTION 10
#define MAX_ID_SIZE 10

typedef struct carta {
  bool mao;
  bool especial;
  bool minha;
  char efeito;
  int  numero;
  char naipe[4];
  char strCarta[6];
} Carta;

void debug(char *message) { fprintf(stderr, "%s\n", message); }

void imprime_carta(Carta carta){
   fprintf(stderr, "carta: %s mao: %d minha: %d especial: %d efeito: %c numero: %d naipe: %s \n", 
     carta.strCarta, carta.mao, carta.minha, carta.especial, carta.efeito, carta.numero, carta.naipe);
}

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

Carta criar_carta(char *strCarta, bool minha) {

  Carta carta;
  carta.mao = true;
  carta.minha = minha;

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

    cartas[i] = criar_carta(strCarta, true);

    strCarta = strtok(NULL, " ]");
    i++;
  }

  *totalcartas = i;
  return cartas;
}

Carta* le_carta_table(char temp[], int* totalcartas, Carta* cartas, bool minha) {

  int i = *totalcartas;
  char *strCarta = temp;

  if (i > 0) {
    cartas = realloc(cartas, sizeof(Carta) * (i + 1));
  }

  cartas[i] = criar_carta(strCarta, minha);

  i++;
  
  *totalcartas = i;
  return cartas;
}

int escolhe_carta(Carta* cartas, int* totalCartas, Carta cartaMesa, char secondComplement[]){

    bool mudouNaipe = (cartaMesa.efeito == 'C' || cartaMesa.efeito == 'A') && *totalCartas>1;

    for(int i=0; i<*totalCartas-1; i++){
         
         if(cartas[i].mao==true){

            if(cartas[i].efeito=='C'){
              return i;
            }

            if(mudouNaipe && strcmp(secondComplement, cartas[i].naipe)==0){
                debug("Encontrou com mudouNaipe");
                debug(secondComplement);
                cartas[i].mao=false;
                return i;
            }

            if(!mudouNaipe && strcmp(cartaMesa.naipe, cartas[i].naipe)==0){
                debug("Encontrou Naipe");
                cartas[i].mao=false;
                return i;
            }

            if(!mudouNaipe && cartaMesa.especial && cartaMesa.efeito==cartas[i].efeito){
                debug("Encontrou efeito");
                cartas[i].mao=false;
                return i;
            }

            if(!mudouNaipe && cartaMesa.numero > 0 && cartaMesa.numero==cartas[i].numero){
                debug("Encontrou numero");
                cartas[i].mao=false;
                return i;
            }

         }  
    }

    return -1;

}

Carta* compra_carta(int quantidade, char action[], int* totalCartas, Carta* cartas){
  Carta *cartas_compradas = cartas; 
  printf("BUY %d\n", quantidade);
  for(int i=0; i<quantidade; i++){
    scanf("%s ", action);
    cartas_compradas = le_carta_table(action, totalCartas, cartas_compradas, true);
  }

  for(int i= *totalCartas-1;  i<*totalCartas; i++){
    imprime_carta(cartas[i]);
  }
  return cartas_compradas;
}

int main() {

  char temp[MAX_LINE];     // string para leitura temporária de dados
  char my_id[MAX_ID_SIZE]; // identificador do seu bot
  char **jogadores = NULL;
  int *totalJogadores = malloc(sizeof(int));

  Carta *cartas = malloc(sizeof(Carta));
  int *totalCartas = malloc(sizeof(int));
  *totalCartas = 0;

  Carta *cartasMesa = malloc(sizeof(Carta));;
  int *totalCartasMesa = malloc(sizeof(int));
  *totalCartasMesa = 0;

  setbuf(stdin, NULL);  // stdin, stdout e stderr não terão buffers
  setbuf(stdout, NULL); // assim, nada é "guardado temporariamente"
  setbuf(stderr, NULL);

  // === INÍCIO DA PARTIDA ===

  scanf("PLAYERS %[^\n]\n", temp);
  jogadores = le_jogadores(temp, totalJogadores);

  scanf("YOU %s\n", my_id);

  scanf("HAND [ %[^\n]\n", temp);
  cartas = le_cartas_hand(temp, totalCartas, cartas);

  debug("------HAND------");
  fprintf(stderr,"total de cartas no HAND: %d\n", *totalCartas);
  for (int i = 0; i < *totalCartas; i++) {
    imprime_carta(cartas[i]);
  }

  scanf("TABLE %s\n", temp);
  cartasMesa = le_carta_table(temp, totalCartasMesa, cartasMesa, false);

  debug("------TABLE------");
  fprintf(stderr,"total de cartas no TABLE: %d\n", *totalCartasMesa);
  for (int i = 0; i < *totalCartasMesa; i++) {
      imprime_carta(cartasMesa[i]);
  }

  // === PARTIDA ===

  char id[MAX_ID_SIZE];
  char action[MAX_ACTION];
  char complement[MAX_LINE];
  char secondComplement[MAX_LINE];
  bool pulouVez = false;

  while (1) {

   
    do {
      debug("-------------- JOGADA ADVERSARIO -------------------");
      scanf("%s %s", action, complement);
      debug(action);
      debug(complement);


      
      if(complement[0]=='C' || complement[0]=='A'){
          scanf(" %s", secondComplement);
      }
      
      fprintf(stderr, "compare DISCARD: %d \n",strcmp(action,"DISCARD"));

      if(strcmp(action,"TURN")==0 && strcmp(complement,my_id)!=0 && totalJogadores==2){
          debug("desliga pulouVez");
          pulouVez = false;
      }

      if(strcmp(action,"DISCARD")==0){
        debug(" Entrou pra ler discard");
        cartasMesa = le_carta_table(complement, totalCartasMesa, cartasMesa, false);
        
        if(cartasMesa[*totalCartasMesa-1].efeito=='R' && totalJogadores>2){
            debug("liga pulouVez");
            pulouVez = true;
        }

        debug("------TABLE DISCARD------");
        fprintf(stderr,"total de cartas no TABLE: %d\n", *totalCartasMesa);
        for (int i = 0; i < *totalCartasMesa; i++) {
            imprime_carta(cartasMesa[i]);
        }
      }
      debug("-----------------------------------------------------");

    } while (strcmp(action, "TURN") || strcmp(complement, my_id));

      // agora é a vez do seu bot jogar
      debug("----- MINHA VEZ -----");

      if((cartasMesa[*totalCartasMesa -1].efeito=='R') && !cartasMesa[*totalCartasMesa -1].minha && pulouVez){
        debug("pulou vez");
        printf(" \n");
      }else if((cartasMesa[*totalCartasMesa -1].efeito=='V' || cartasMesa[*totalCartasMesa -1].efeito=='C') && !cartasMesa[*totalCartasMesa -1].minha && !pulouVez){
        
        if(cartasMesa[*totalCartasMesa -1].efeito=='V'){
            debug("----- VOU COMPRAR 2 -----");
            cartas = compra_carta(2,action,totalCartas,cartas);
            printf(" \n");
        }

        if(cartasMesa[*totalCartasMesa -1].efeito=='C'){
            debug("----- VOU COMPRAR 4 -----");
            cartas = compra_carta(4,action,totalCartas,cartas);
            printf(" \n");
        }

      }else{

          int indiceCarta = escolhe_carta(cartas, totalCartas, cartasMesa[*totalCartasMesa-1], secondComplement); 
          if(indiceCarta >= 0){
              
              debug("------CARTA NA MESA ------");
              imprime_carta(cartasMesa[*totalCartasMesa-1]);
              debug("------CARTA JOGADA ------");
              imprime_carta(cartas[indiceCarta]);
              cartasMesa = le_carta_table(cartas[indiceCarta].strCarta, totalCartasMesa, cartasMesa, true);
              cartas[indiceCarta].mao=false;
        
              if(cartas[indiceCarta].efeito=='A' || cartas[indiceCarta].efeito=='C'){
                  strcpy(secondComplement, cartas[indiceCarta].naipe);
                  printf("DISCARD %s %s\n", cartas[indiceCarta].strCarta, cartas[indiceCarta].naipe);
              }else{
                  printf("DISCARD %s\n", cartas[indiceCarta].strCarta);
              }
          }else{
              debug("----- VOU COMPRAR 1 -----");
              cartas = compra_carta(1,action,totalCartas,cartas);
              printf(" \n");
          }
      }
  }

  return 0;
}