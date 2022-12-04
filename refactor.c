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
  int  indAdversario;
  char* mudaNaipe;
} Carta;

void debug(char *message) { fprintf(stderr, "%s\n", message); }

void imprime_carta(Carta carta){
   fprintf(stderr, "carta: %s mao: %d minha: %d especial: %d efeito: %c numero: %d naipe: %s indAdversario: %d mudaNaipe: %s\n", 
     carta.strCarta, carta.mao, carta.minha, carta.especial, carta.efeito, carta.numero, carta.naipe, carta.indAdversario, carta.mudaNaipe);
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

Carta* compra_carta(int quantidade, char action[], int* totalCartas, Carta* cartas){
  Carta *cartas_compradas = cartas; 
  printf("BUY %d\n", quantidade);
  for(int i=0; i<quantidade; i++){
    scanf("%s ", action);
    debug(action);
    cartas_compradas = le_carta_table(action, totalCartas, cartas_compradas, true, -1);
  }

  for(int i= *totalCartas-quantidade;  i<*totalCartas; i++){
    imprime_carta(cartas_compradas[i]);
  }
  return cartas_compradas;
}

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

  debug("------HAND------");
  fprintf(stderr,"total de cartas no HAND: %d\n", *totalCartas);
  for (int i = 0; i < *totalCartas; i++) {
    imprime_carta(cartas[i]);
  }

  scanf("TABLE %s\n", temp);
  cartasMesa = le_carta_table(temp, totalCartasMesa, cartasMesa, false, -1);

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
      
      //fprintf(stderr, "compare DISCARD: %d \n",strcmp(action,"DISCARD"));

      if(strcmp(action,"TURN")==0 && strcmp(complement,my_id)!=0){
          //debug("desliga pulouVez");
          strcpy(jogadorDaVez, complement);
          pulouVez = false;
          vouComprar = false;
      }

      if(strcmp(action,"BUY")==0 && strcmp(complement,my_id)!=0){
          //debug("desliga pulouVez");
          pulouVez = false;
          vouComprar = false;
      }

      if(strcmp(action,"BUY")==0 && complement[0] == '1'
          &&  (strcmp(jogadorDaVez, jogadores[adversarios[0]])== 0 || 
               strcmp(jogadorDaVez, jogadores[adversarios[1]])== 0)){
          //AQUI - implementar uma lista com as cartas que os oponentes não têm.

          int indiceAdversario = strcmp(jogadorDaVez, jogadores[adversarios[0]])== 0 ?adversarios[0]: adversarios[1];

          cartasAdversariosNaoPossuem = le_carta_table(cartasMesa[*totalCartasMesa-1].strCarta, totalAdversariosNaoPossuem, cartasAdversariosNaoPossuem, false, indiceAdversario);

          for(int i = 0; i< *totalAdversariosNaoPossuem; i++){
            imprime_carta(cartasAdversariosNaoPossuem[i]);
          }
          //debug("meus adversarios compraram 1");
      }

      if(strcmp(action,"DISCARD")==0){
        //debug(" Entrou pra ler discard");
        cartasMesa = le_carta_table(complement, totalCartasMesa, cartasMesa, false, -1);
        
        if(cartasMesa[*totalCartasMesa-1].efeito=='R'){
            //debug("liga pulouVez");
            pulouVez = true;
        }

        if(strcmp(jogadorDaVez, jogadores[adversarios[0]]) == 0
            && (cartasMesa[*totalCartasMesa-1].efeito=='V' || cartasMesa[*totalCartasMesa-1].efeito=='C')){
            //debug("é meu adversario , vou ter que comprar");
            vouComprar = true;
        }

        if(cartasMesa[*totalCartasMesa-1].efeito=='D'){
            //debug("Inverteu o jogo");
            troca_adversarios(adversarios);
        }

        /*debug("------TABLE DISCARD------");
        fprintf(stderr,"total de cartas no TABLE: %d\n", *totalCartasMesa);
        for (int i = 0; i < *totalCartasMesa; i++) {
            imprime_carta(cartasMesa[i]);
        }*/
      }
      debug("-----------------------------------------------------");

    } while (strcmp(action, "TURN") || strcmp(complement, my_id));

      // agora é a vez do seu bot jogar
      debug("----- MINHA VEZ -----");
      debug("------- MEU ADVERSARIO ------");
      debug(jogadores[adversarios[0]]);
      debug(jogadores[adversarios[1]]);

      debug(jogadorDaVez);

      if(cartasMesa[*totalCartasMesa -1].efeito=='R' && (strcmp(jogadorDaVez, jogadores[adversarios[0]])==0) && pulouVez){
        debug("pulou vez");
        pulouVez = false;
        printf(" \n");
      }else if(!cartasMesa[*totalCartasMesa -1].minha && vouComprar){
        
        if(cartasMesa[*totalCartasMesa -1].efeito=='V'){
            debug("----- VOU COMPRAR 2 -----");
            cartas = compra_carta(2,action,totalCartas,cartas);
            vouComprar = false;
            printf(" \n");
        }

        if(cartasMesa[*totalCartasMesa -1].efeito=='C'){
            debug("----- VOU COMPRAR 4 -----");
            cartas = compra_carta(4,action,totalCartas,cartas);
            vouComprar = false;
            printf(" \n");
        }

      }else{

          int indiceCarta = escolhe_carta(cartas, totalCartas, totalCartasMesa, cartasMesa[*totalCartasMesa-1], secondComplement, 
                                                               cartasAdversariosNaoPossuem, totalAdversariosNaoPossuem, adversarios[1]); 
          if(indiceCarta >= 0){
              
              debug("------CARTA NA MESA ------");
              imprime_carta(cartasMesa[*totalCartasMesa-1]);
              debug("------CARTA JOGADA ------");
              imprime_carta(cartas[indiceCarta]);
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
              debug("----- VOU COMPRAR 1 -----");
              cartas = compra_carta(1,action,totalCartas,cartas);
              printf(" \n");
          }
      }
  }

  return 0;
}