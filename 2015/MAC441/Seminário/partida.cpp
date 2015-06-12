// classes example
#include <iostream>
#include <string>
#include <stdio.h>
using namespace std;

class Partida {
  public:
    void adicionaJogo (string, string, int, int);
    void imprimeJogo ();
  private:
    string timeCasa;
    int golsTimeCasa;
    string timeVisitante;
    int golsTimeVisitante;
};

void Partida::adicionaJogo ( string time1,
                             string time2,
                             int golsCasa,
                             int golsVisitante ) {
  timeCasa = time1;
  timeVisitante = time2;
  golsTimeCasa = golsCasa;
  golsTimeVisitante = golsVisitante;
}

void Partida::imprimeJogo () {
  printf( "O jogo do %s contra o %s foi %d a %d.\n",
          timeCasa.c_str(), timeVisitante.c_str(),
          golsTimeCasa, golsTimeVisitante);
}


int main () {
  Partida partida;
  
  partida.adicionaJogo("Alemanha", "Brasil", 7, 1);
  
  partida.imprimeJogo();
  
  return 0;
}
