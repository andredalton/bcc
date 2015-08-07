// classes example
#include <iostream>
#include <string>
#include <cstdio>
using namespace std;

class Gato;
class Vaca;

class Animal
{
  public:
    Animal ( double );
    Animal ();
    virtual ~Animal ();
    double pesa ();
    void pesa (double);
    virtual void come (double);
    virtual Animal* reproduz (Animal*) = 0;
  
  protected:
    static Animal* criaFilho(Animal* mae, Animal* pai, Animal* filho);
    
  private:
    double pesoInicio;
    double peso;
};

Animal::Animal ( double p )
{
  pesoInicio = peso = p;
}

Animal::Animal ()
{
  pesoInicio = peso = 0;
}

Animal::~Animal ( void )
{
  cout << "Engordei " << (peso - pesoInicio) << " na minha vida!\n" << endl;
}

double Animal::pesa ( )
{
  return peso;
}

void Animal::pesa ( double p )
{
  pesoInicio = peso = p;
}

void Animal::come ( double p )
{
//  cout << "Nem todo ser vivo come!" << endl;
  peso += p;
}

Animal* Animal::criaFilho(Animal* mae, Animal* pai, Animal* filho)
{
  if (filho) {
    filho->pesa((mae->pesa() + pai->pesa()) / 2);
    printf("O filho nasceu sadio!\n");
    return filho;
  } else {
    printf("O filho nasceu morto, pois os pais são de classes diferentes!\n");
    return nullptr;
  }
}

class Gato : public Animal
{
public:
  Gato ();
  Gato (double);
  ~Gato ();
  // Se não existir na classe pai da erro de compilação
  virtual void come (double) override;
  virtual Animal* reproduz (Animal*) override;
};

Gato::Gato ( double p )
: Animal(p)
{
  printf("Miau! Sou um novo e belo gatinho!\n");
}

Gato::Gato ()
: Animal()
{
  printf("Miau! Sou um novo e belo gatinho!\n");
}

Gato::~Gato ( void )
{
  printf("Acabaram minhas 7 vidas!\n");
}

void Gato::come (double p)
{
  cout << "Que peixe gostoso!" << endl;
  Animal::come(p/2);
}

Animal* Gato::reproduz (Animal* pai)
{
  Animal* filho = nullptr;
  if (Gato* pai_g = dynamic_cast<Gato*>(pai)) {
    filho = new Gato;
  }
  return criaFilho(this, pai, filho);
}



class Vaca : public Animal
{
public:
  virtual void come (double) override;
  virtual Animal* reproduz (Animal*) override;
};

void Vaca::come (double p)
{
  cout << "Muuuuu!" << endl;
  Animal::come(p*2);
}

Animal* Vaca::reproduz (Animal* pai)
{
  Animal* filho = nullptr;
  if (Vaca* pai_g = dynamic_cast<Vaca*>(pai)) {
    filho = new Vaca;
  }
  return criaFilho(this, pai, filho);
}

int main () {
  Gato* g = new Gato(0);
  Gato* pai = new Gato(10);

  g->pesa(2);  
  g->come(0.5);

  Animal* f = g->reproduz(pai);
  f->come(10);
  
  Gato* neto = dynamic_cast<Gato*>(f->reproduz(pai));
  neto->come(5);

  printf("%lf\n", g->pesa());
  printf("%lf\n", pai->pesa());
  printf("%lf\n", f->pesa());
  printf("%lf\n", neto->pesa());
  
  delete g;
  delete f;
  delete neto;

  
  Vaca* v = new Vaca;
  v->pesa(500);
  
  Animal* quimera = v->reproduz(pai);
  
  
  printf("vaca com gato deu em: %p\n", quimera);
  
  delete pai;
  delete v;
  delete quimera;
 
  return 0;
}
