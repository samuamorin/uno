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
}



char* escolhe_proximo_naipe(Carta cartasProxOponente[], int totalCartasProxOponente, Carta* cartas, int *totalCartas){

      char paus[] = "♣";
      char ouros[] = "♦";
      char copas[] = "♥";
      char espadas[] = "♦";

      int totalPausAdversario= 0;
      int totalOurosAdversario = 0;
      int totalCopasAdversario = 0;
      int totalEspadasAdversario = 0;

      int totalPausHand= 0;
      int totalOurosHand = 0;
      int totalCopasHand = 0;
      int totalEspadasHand = 0;

      bool possuiCartasOponente = totalCartasProxOponente>0;

      if(possuiCartasOponente){
          for(int i = 0; i< totalCartasProxOponente; i++){
               if(strcmp(cartasProxOponente[i].naipe, paus)==0)
                   totalPausAdversario ++;

               if(strcmp(cartasProxOponente[i].naipe, ouros)==0)
                   totalOurosAdversario++;

               if(strcmp(cartasProxOponente[i].naipe, copas)==0)
                   totalCopasAdversario++;

               if(strcmp(cartasProxOponente[i].naipe, espadas)==0)
                   totalEspadasAdversario++;
          } 
      }

      for(int i = 0; i< totalCartasProxOponente; i++){
           if(strcmp(cartasProxOponente[i].naipe, paus)==0)
               totalPausHand ++;

           if(strcmp(cartasProxOponente[i].naipe, ouros)==0)
               totalOurosHand++;

           if(strcmp(cartasProxOponente[i].naipe, copas)==0)
               totalCopasHand++;

           if(strcmp(cartasProxOponente[i].naipe, espadas)==0)
               totalEspadasHand++;
      }
      

      if(possuiCartasOponente && totalPausAdversario > totalOurosAdversario && totalPausAdversario> totalCopasAdversario && totalPausAdversario > totalEspadasAdversario){
          if(totalPausHand > 0){
            return paus;
          }  
      }
      

      if(possuiCartasOponente && totalOurosAdversario > totalPausAdversario && totalOurosAdversario > totalCopasAdversario && totalOurosAdversario > totalEspadasAdversario){
          if(totalOurosHand > 0){
            return ouros;
          }  
      }
      

      if(possuiCartasOponente && totalCopasAdversario > totalPausAdversario && totalCopasAdversario> totalOurosAdversario && totalCopasAdversario > totalEspadasAdversario){
          if(totalCopasHand > 0){
            return copas;
          }  
      }
      

      if(possuiCartasOponente && totalEspadasAdversario > totalPausAdversario && totalEspadasAdversario> totalOurosAdversario && totalEspadasAdversario > totalCopasAdversario){
          if(totalEspadasHand > 0){
            return espadas;
          }  
      }


      if(totalPausHand > totalOurosHand && totalPausHand> totalCopasHand && totalPausHand > totalEspadasHand)
          return paus;
      

      if(totalOurosHand > totalPausHand && totalOurosHand> totalCopasHand && totalOurosHand > totalEspadasHand)
          return ouros;
      

      if(totalCopasHand > totalPausHand && totalCopasHand> totalOurosHand && totalCopasHand > totalEspadasHand)
          return copas;
      

      if(totalEspadasHand > totalPausHand && totalEspadasHand> totalOurosHand && totalEspadasHand > totalCopasHand)
          return espadas;
      
      return NULL;

}

char* mudaNaipe(Carta cartasProxOponente[], int totalCartasProxOponente, Carta* cartas, int *totalCartas, char naipeAtual[]){

  char* mudaNaipe = malloc(sizeof(char)*4);

  char* proximoNaipe = escolhe_proximo_naipe(cartasProxOponente, totalCartasProxOponente, cartas, totalCartas);

  if(proximoNaipe!=NULL){
    strcpy(mudaNaipe,proximoNaipe);
  }else{
    strcpy(mudaNaipe,naipeAtual);
  }

  return mudaNaipe;
  
}

int escolhe_carta(Carta* cartas, int* totalCartas, int* totalCartasMesa, Carta cartaMesa, char secondComplement[], Carta* cartasAdversarios, int* totalAdversarios, int adversario){

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
    bool possuiAs = false;

    bool mudouNaipe = (cartaMesa.efeito == 'C' || cartaMesa.efeito == 'A') && !primeiraJogada;

    if(primeiraJogada){
      strcpy(secondComplement, cartaMesa.naipe);
    }

    for(int i=0; i<*totalCartas; i++){
         
         if(cartas[i].mao==true){
            
           if(cartas[i].efeito=='C'){
              cartasJogaveis[totalCargasJogaveis]=i;
              totalCargasJogaveis++;
              possuiCoringa = true;
            }

            if(cartas[i].efeito=='A'){
              cartasJogaveis[totalCargasJogaveis]=i;
              totalCargasJogaveis++;
              possuiAs = true;
            }

            if((mudouNaipe || primeiraJogada) && strcmp(secondComplement, cartas[i].naipe)==0){
                cartasJogaveis[totalCargasJogaveis]=i;
                totalCargasJogaveis++;
            }

            if(!mudouNaipe && strcmp(cartaMesa.naipe, cartas[i].naipe)==0){
                cartasJogaveis[totalCargasJogaveis]=i;
                naipe++;
                totalCargasJogaveis++;
            }

            if(!mudouNaipe && cartaMesa.especial && cartaMesa.efeito==cartas[i].efeito){
                if(cartas[i].efeito=='V'){
                  possuiCompra = true;
                }
                cartasJogaveis[totalCargasJogaveis]=i;
                numero++;
                totalCargasJogaveis++;
            }

            if(!mudouNaipe && cartaMesa.numero > 0 && cartaMesa.numero==cartas[i].numero){
                cartasJogaveis[totalCargasJogaveis]=i;
                numero++;
                totalCargasJogaveis++;
            }

         }  
    }

    if(totalCargasJogaveis<1){
      return -1;
    }

    if(possuiCoringa){
      for(int i=0; i<totalCargasJogaveis; i++){
          if(cartas[cartasJogaveis[i]].efeito =='C'){
            cartas[cartasJogaveis[i]].mudaNaipe = mudaNaipe(cartasProxOponente,totalProxOponente,cartas,totalCartas, cartas[cartasJogaveis[i]].naipe);
            return cartasJogaveis[i];
          }
      }
    }

    if(possuiCompra){
      for(int i=0; i<totalCargasJogaveis; i++){
          if(cartas[cartasJogaveis[i]].efeito =='V'){
            return cartasJogaveis[i];
          }
      }
    }

    if(possuiAs){
      for(int i=0; i<totalCargasJogaveis; i++){
          if(cartas[cartasJogaveis[i]].efeito =='A'){
              cartas[cartasJogaveis[i]].mudaNaipe = mudaNaipe(cartasProxOponente,totalProxOponente,cartas,totalCartas, cartas[cartasJogaveis[i]].naipe);
              return cartasJogaveis[i];
          }
      }
    }

    if(numero > naipe){
        for(int i=0; i<totalCargasJogaveis; i++){
          if(cartas[cartasJogaveis[i]].efeito==cartaMesa.efeito || cartas[cartasJogaveis[i]].numero==cartaMesa.numero){
              return cartasJogaveis[i];
          }
        }
    }
    
    for(int i=0; i<totalCargasJogaveis; i++){
      if(strcmp(cartas[cartasJogaveis[i]].naipe, mudouNaipe? secondComplement: cartaMesa.naipe)==0){
          return cartasJogaveis[i];
      }
   }

}