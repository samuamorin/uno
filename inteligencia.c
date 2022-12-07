#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>
#include "carta.h"

// indice 0 - joga antes de mim; indice 1 - joga depois
void verifica_adversarios(char **jogadores, int totalJogadores, int adversarios[], char my_id[]){
    int meuIndice = 0;
    
    for(int i = 0; i< totalJogadores; i++){
      if(strcmp(jogadores[i], my_id) == 0){
          meuIndice = i;
          break;
      }
    }

    if(meuIndice==0){
        adversarios[0]=totalJogadores-1;
        adversarios[1]=meuIndice+1;
        return;
    }

    if(meuIndice==totalJogadores-1){
      adversarios[0]=meuIndice-1;
      adversarios[1]=0;
      return;
    }


    adversarios[0] = meuIndice-1;
    adversarios[1] = meuIndice+1;

}

void troca_adversarios(int adversarios[]){
    int aux = adversarios[0];
    adversarios[0] = adversarios[1];
    adversarios[1] = aux;
    debug("trocou");
}



char* escolhe_proximo_naipe(Carta cartasProxOponente[], int totalCartasProxOponente, Carta* cartas, int *totalCartas){

      if(totalCartasProxOponente>0){
          debug("escolheu outro naipe");
          debug(cartasProxOponente[totalCartasProxOponente-1].naipe);
          return cartasProxOponente[totalCartasProxOponente-1].naipe;  
      }

      //♣ ♠ ♦ ♥
      debug("vai com mesmo naipe");
      return NULL;

}

int escolhe_carta(Carta* cartas, int* totalCartas, int* totalCartasMesa, Carta cartaMesa, char secondComplement[], Carta* cartasAdversarios, int* totalAdversarios, int adversario){
    debug("------ Carta na Mesa ----------");
    imprime_carta(cartaMesa);

    int cartasJogaveis[*totalCartas];
    int totalCargasJogaveis = 0;

    for(int i=0; i < *totalCartas; i++){
      cartasJogaveis[0] = -1;
    }


    Carta cartasProxOponente[*totalAdversarios];
    int totalProxOponente = 0;

    for(int i = 0; i< *totalAdversarios; i++){
        if(cartasAdversarios[i].indAdversario==adversario){
          cartasProxOponente[totalProxOponente]=cartasAdversarios[i];
          totalProxOponente++;
        }
    }

  
    bool primeiraJogada = *totalCartasMesa<2;

    int naipe = 0;
    int numero = 0;
    bool possuiCoringa = false;
    bool possuiCompra = false;

    bool mudouNaipe = (cartaMesa.efeito == 'C' || cartaMesa.efeito == 'A') && !primeiraJogada;
    if(mudouNaipe){
      debug("mudouNaipe");
    }

    if(primeiraJogada){
      debug("primeiraJogada");
      strcpy(secondComplement, cartaMesa.naipe);
    }

    debug("------ Escolhendo todas as cartas jogaveis ------");
    for(int i=0; i<*totalCartas; i++){
         
         if(cartas[i].mao==true){
            //imprime_carta(cartas[i]);
            if(cartas[i].efeito=='C'){
              cartasJogaveis[totalCargasJogaveis]=i;
              totalCargasJogaveis++;
              possuiCoringa = true;
            }

            if((mudouNaipe || primeiraJogada) && strcmp(secondComplement, cartas[i].naipe)==0){
                debug("Encontrou com mudouNaipe");
                debug(secondComplement);
                cartasJogaveis[totalCargasJogaveis]=i;
                totalCargasJogaveis++;
            }

            if(!mudouNaipe && strcmp(cartaMesa.naipe, cartas[i].naipe)==0){
                debug("Encontrou Naipe");
                cartasJogaveis[totalCargasJogaveis]=i;
                naipe++;
                totalCargasJogaveis++;
            }

            if(!mudouNaipe && cartaMesa.especial && cartaMesa.efeito==cartas[i].efeito){
                debug("Encontrou efeito");
                if(cartas[i].efeito=='V'){
                  possuiCompra = true;
                }
                cartasJogaveis[totalCargasJogaveis]=i;
                numero++;
                totalCargasJogaveis++;
            }

            if(!mudouNaipe && cartaMesa.numero > 0 && cartaMesa.numero==cartas[i].numero){
                debug("Encontrou numero");
                cartasJogaveis[totalCargasJogaveis]=i;
                numero++;
                totalCargasJogaveis++;
            }

         }  
    }

    debug("-----CARTAS JOGAVEIS-----");
    for(int i = 0; i<totalCargasJogaveis; i++){
        imprime_carta(cartas[cartasJogaveis[i]]);
    }

    if(totalCargasJogaveis<1){
      return -1;
    }

    if(possuiCoringa){
      debug("possuiCoringa");
      for(int i=0; i<totalCargasJogaveis; i++){
          if(cartas[cartasJogaveis[i]].efeito =='C'){
            cartas[cartasJogaveis[i]].mudaNaipe = malloc(sizeof(char)*4);
            strcpy(cartas[cartasJogaveis[i]].mudaNaipe,
                           escolhe_proximo_naipe(cartasProxOponente, totalProxOponente, cartas, totalCartas)!=NULL? 
                           escolhe_proximo_naipe(cartasProxOponente, totalProxOponente, cartas, totalCartas):cartas[cartasJogaveis[i]].naipe);
            debug(cartas[cartasJogaveis[i]].naipe);
            debug(cartas[cartasJogaveis[i]].mudaNaipe);
            return cartasJogaveis[i];
          }
      }
    }

    if(possuiCompra && !possuiCoringa){
      for(int i=0; i<totalCargasJogaveis; i++){
          if(cartas[cartasJogaveis[i]].efeito =='V'){
            cartas[cartasJogaveis[i]].mudaNaipe = malloc(sizeof(char)*4);
            strcpy(cartas[cartasJogaveis[i]].mudaNaipe,
                           escolhe_proximo_naipe(cartasProxOponente, totalProxOponente, cartas, totalCartas)!=NULL? 
                           escolhe_proximo_naipe(cartasProxOponente, totalProxOponente, cartas, totalCartas):cartas[cartasJogaveis[i]].naipe);
            debug(cartas[cartasJogaveis[i]].naipe);
            debug(cartas[cartasJogaveis[i]].mudaNaipe);
            return cartasJogaveis[i];
          }
      }
    }

    if(numero > naipe){
        debug("numero maior");
        for(int i=0; i<totalCargasJogaveis; i++){
          if(cartas[cartasJogaveis[i]].efeito==cartaMesa.efeito || cartas[cartasJogaveis[i]].numero==cartaMesa.numero){
              if(cartas[cartasJogaveis[i]].efeito=='A'){
                 debug("escolheu A");
                 cartas[cartasJogaveis[i]].mudaNaipe = malloc(sizeof(char)*4);
                 strcpy(cartas[cartasJogaveis[i]].mudaNaipe,
                           escolhe_proximo_naipe(cartasProxOponente, totalProxOponente, cartas, totalCartas)!=NULL? 
                           escolhe_proximo_naipe(cartasProxOponente, totalProxOponente, cartas, totalCartas):cartas[cartasJogaveis[i]].naipe);
                 debug(cartas[cartasJogaveis[i]].naipe);
                 debug(cartas[cartasJogaveis[i]].mudaNaipe);
              }
              debug("deu numero");
              return cartasJogaveis[i];
          }
        }
    }
    
    debug("naipe maior");
    for(int i=0; i<totalCargasJogaveis; i++){
      if(strcmp(cartas[cartasJogaveis[i]].naipe, mudouNaipe? secondComplement: cartaMesa.naipe)==0){
          debug("deu naipe");
          if(cartas[cartasJogaveis[i]].efeito=='A'){
             cartas[cartasJogaveis[i]].mudaNaipe = malloc(sizeof(char)*4);
             strcpy(cartas[cartasJogaveis[i]].mudaNaipe,
                           escolhe_proximo_naipe(cartasProxOponente, totalProxOponente, cartas, totalCartas)!=NULL? 
                           escolhe_proximo_naipe(cartasProxOponente, totalProxOponente, cartas, totalCartas):cartas[cartasJogaveis[i]].naipe);
          }
          return cartasJogaveis[i];
      }
   }

}