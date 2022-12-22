#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "carta.h"
#include "entrada.h"
#include "log.h"
#include "inteligencia.h"

#define MAX_LINE 100
#define MAX_ACTION 10
#define MAX_ID_SIZE 10


int main() {

  char temp[MAX_LINE];     // string para leitura temporária de dados
  char my_id[MAX_ID_SIZE]; // identificador do seu bot
  char **jogadores = NULL;
  int *totalJogadores = malloc(sizeof(int));
  int adversarios[2];

  Carta *cartas = malloc(sizeof(Carta));
  int *totalCartas = malloc(sizeof(int));
  *totalCartas = 0;

  Carta *cartasMesa = malloc(sizeof(Carta));;
  int *totalCartasMesa = malloc(sizeof(int));
  *totalCartasMesa = 0;

  Carta *cartasAdversariosNaoPossuem = malloc(sizeof(Carta));
  int *totalAdversariosNaoPossuem = malloc(sizeof(int));


  setbuf(stdin, NULL);  // stdin, stdout e stderr não terão buffers
  setbuf(stdout, NULL); // assim, nada é "guardado temporariamente"
  setbuf(stderr, NULL);

  // === INÍCIO DA PARTIDA ===

  scanf("PLAYERS %[^\n]\n", temp);
  jogadores = le_jogadores(temp, totalJogadores);

  scanf("YOU %s\n", my_id);
  verifica_adversarios(jogadores, *totalJogadores, adversarios, my_id);


  scanf("HAND [ %[^\n]\n", temp);
  cartas = le_cartas_hand(temp, totalCartas, cartas);

  for (int i = 0; i < *totalCartas; i++) {
    imprime_carta(cartas[i]);
  }

  scanf("TABLE %s\n", temp);
  cartasMesa = le_carta_table(temp, totalCartasMesa, cartasMesa, false, -1);


  // === PARTIDA ===

  char id[MAX_ID_SIZE];
  char action[MAX_ACTION];
  char complement[MAX_LINE];
  char secondComplement[MAX_LINE];
  bool pulouVez = false;
  bool vouComprar = false;
  char jogadorDaVez[MAX_LINE];

  while (1) {

   
    do {
      debug("-------------- JOGADA ADVERSARIO -------------------");
      scanf("%s %s", action, complement);
      debug(action);
      debug(complement);


      
      if(complement[0]=='C' || complement[0]=='A'){
          scanf(" %s", secondComplement);
      }
      

      if(strcmp(action,"TURN")==0 && strcmp(complement,my_id)!=0){
          strcpy(jogadorDaVez, complement);
          pulouVez = false;
          vouComprar = false;
      }

      if(strcmp(action,"BUY")==0 && strcmp(complement,my_id)!=0){
          pulouVez = false;
          vouComprar = false;
      }

      if(strcmp(action,"BUY")==0 && complement[0] == '1'
          &&  (strcmp(jogadorDaVez, jogadores[adversarios[0]])== 0 || 
               strcmp(jogadorDaVez, jogadores[adversarios[1]])== 0)){

          int indiceAdversario = strcmp(jogadorDaVez, jogadores[adversarios[0]])== 0 ?adversarios[0]: adversarios[1];

          cartasAdversariosNaoPossuem = le_carta_table(cartasMesa[*totalCartasMesa-1].strCarta, totalAdversariosNaoPossuem, cartasAdversariosNaoPossuem, false, indiceAdversario);

 
      }

      if(strcmp(action,"DISCARD")==0){
        cartasMesa = le_carta_table(complement, totalCartasMesa, cartasMesa, false, -1);
        
        if(cartasMesa[*totalCartasMesa-1].efeito=='R'){
            pulouVez = true;
        }

        if(strcmp(jogadorDaVez, jogadores[adversarios[0]]) == 0
            && (cartasMesa[*totalCartasMesa-1].efeito=='V' || cartasMesa[*totalCartasMesa-1].efeito=='C')){
            vouComprar = true;
        }

        if(cartasMesa[*totalCartasMesa-1].efeito=='D'){
            troca_adversarios(adversarios);
        }

      }
      debug("-----------------------------------------------------");

    } while (strcmp(action, "TURN") || strcmp(complement, my_id));

      debug("----- MINHA VEZ -----");

      if(cartasMesa[*totalCartasMesa -1].efeito=='R' && (strcmp(jogadorDaVez, jogadores[adversarios[0]])==0) && pulouVez){
        pulouVez = false;
        printf(" \n");
      }else if(!cartasMesa[*totalCartasMesa -1].minha && vouComprar){
        
        if(cartasMesa[*totalCartasMesa -1].efeito=='V'){
            cartas = compra_carta(2,action,totalCartas,cartas);
            vouComprar = false;
            printf(" \n");
        }

        if(cartasMesa[*totalCartasMesa -1].efeito=='C'){
            cartas = compra_carta(4,action,totalCartas,cartas);
            vouComprar = false;
            printf(" \n");
        }

      }else{

          int indiceCarta = escolhe_carta(cartas, totalCartas, totalCartasMesa, cartasMesa[*totalCartasMesa-1], secondComplement, 
                                                               cartasAdversariosNaoPossuem, totalAdversariosNaoPossuem, adversarios[1]); 
          if(indiceCarta >= 0){

              cartasMesa = le_carta_table(cartas[indiceCarta].strCarta, totalCartasMesa, cartasMesa, true, -1);
              cartas[indiceCarta].mao=false;
              if(cartas[indiceCarta].efeito=='D'){
                 troca_adversarios(adversarios);
              }
        
              if(cartas[indiceCarta].efeito=='A' || cartas[indiceCarta].efeito=='C'){
                  strcpy(secondComplement, cartas[indiceCarta].mudaNaipe);
                  printf("DISCARD %s %s\n", cartas[indiceCarta].strCarta, secondComplement);
              }else{
                  printf("DISCARD %s\n", cartas[indiceCarta].strCarta);
              }
          }else{
              cartas = compra_carta(1,action,totalCartas,cartas);
              printf(" \n");
          }
      }
  }

  return 0;
}