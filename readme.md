# Uno
Este repositório contém o projeto final desenvolvido pelos alunos SAMUEL JACINTHO DE AMORIN JUNIOR e LUCAS PINHEIRO CALDAS para a matéria de Introdução às Técnicas de Programação ofertada pelo curso de Bacharelado em Tecnologia da Informação - IMD/UFRN. Nesse projeto foi desenvolvido um bot que joga uma versão do jogo de cartas UNO criado pelos professores da disciplina.

## Introdução
Esta é uma versão do jogo de baralho Uno, com algumas diferenças:

A primeira é que este é um jogo baseado em turnos. Ou seja, um jogador só age quando for sua vez. Logo, não haverá a opção de gritar "UNO" quando um jogador tiver apenas uma carta. Essa regra foi deixada de fora.

A segunda é que ele é jogado com as cartas do baralho tradicional. Ou seja, ao invés de  cores, teremos naipes (copas, espadas, ouro e paus) e, ao invés de cartas especiais (como "Compre duas" ou "Passe a vez"), teremos cartas do baralho como Valete, Damas, Rei, Ás e Coringa. Estas cartas substituem as cartas especiais do Uno, seguindo a correspondência:
* "COMPRE 4"   : C = CORINGA
* "COMPRE 2"   : V = VALETE
* "VOLTA"      : D = DAMA
* "PULA A VEZ" : R = REI
* "MUDE A COR" : A = ÀS (muda o naipe)

A partida será jogada com um único baralho. Assim, teremos quatro cartas de um valor. Por exemplo, 7♥, 7♦, 7♣, 7♠, com exceção do coringa, que há apenas dois: um vermelho e um preto. Porém, para seguir o padrão das cartas, os coringas terão também um naipe, mas serão apenas dos naipes: ♥ (vermelho) e ♣ (preto).

O jogo é gerenciado por um "simulador" que irá controlar o jogo, dando a vez ao bot da rodada. O bot deve ler os dados da entrada-padrão (scanf) no formato especificado pelo simulador, caso contrário ele se perderá e será eliminado da partida. O bot também deve enviar suas ações para o simulador via saída-padrão (printf) no formato esperado pelo simulador, novamente sob pena de ser eliminado da partida.

As cartas do baralho sempre são no formato "ValorNaipe".
Nesse formato, Valor é um dos valores das cartas tradicionais, ou seja, A, 2, 3, 4, 5, 6, 7, 8, 9, 10, V, D, R e C, e Naipe é um dos seguintes naipes: ♥, ♦, ♣, ♠.

Os naipes são caracteres no formato ascii estendidos, que possuem uma representação maior que um byte (char). Isso significa que eles deve ser tratados como se fossem strings.

A lógica apresentada nesse template visa ilustrar a entrada e saída de dados de um bot. Cabe a você aprimorar a lógica das ações do seu bot.

Bom jogo!!!

## Executando o jogo

O gerenciador do jogo Uno se encontra no formato binário do Linux. Então, você precisará executá-lo sobre esse sistema operacional. Se seu computador for Windows, poderá usar o WSL.

Caso não tenha o WSL, nem queira instalá-lo, você poderá executar o jogo também via replit.

## Como rodar nosso bot (Kaspa)
Compilação:

cd modulo

make all

Para rodar o Kaspa (nosso executável):

cd ..
./uno -q bot_A bot_B kaspa

## Estrutura do bot
Neste projeto você encontrará 5 arquivos responsáveis pelo bot:


carta.h : local que guarda a estrutura das cartas.
entrada.c : local onde estão localizadas as funções que recebem os dados de entrada, responsável pela leitura das cartas da mão, mesa e  dos jogadores.
intelgencia.c : arquivo em que está contida as estratégia a ser seguida pelo bot ao longo da partida. 
log.c : responsável pela impressão das cartas.
main.c : arquivo principal do projeto.



