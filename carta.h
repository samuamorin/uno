#ifndef DEF
#define DEF
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
#endif