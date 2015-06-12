#import <stdio.h>
#import <objc/Object.h>
#import <Foundation/NSObject.h>

// interfaces
@interface SerVivo : NSObject
{
    double peso;
}
-(void) pesa: (double) p; // setter
-(double) pesa;           // getter
-(id) reproduzir: (id) Animal;
@end

@interface Gato: SerVivo
-(void) miau;
@end

@interface Cachorro: SerVivo
-(void) auau;
@end


// implementacoes
@implementation SerVivo
-(void) pesa: (double) p
{
    peso = p;
}

-(double) pesa
{
    return peso;
}

// a chamada será Animal reproduzir: Animal2, e devolve um novo objeto da mesma espécie
// que Animal e Animal2.
-(id) reproduzir: (id) Animal2
{
    id Filho=[[[self class] alloc] init];

    if ([self class] == [Animal2 class])
    {
    	printf("O filho nasceu sadio!\n");
    	[Filho pesa: (peso+[Animal2 pesa])/2];
    }
    else
    {
    	printf("O filho nasceu morto, pois os pais são de classes diferentes!\n");
    	[Filho pesa: 0];
    }
    return Filho;
}
@end

@implementation Gato
-(void) miau
{
    printf("Miaau!\n");
}
@end

@implementation Cachorro
-(void) auau
{
    printf("AuAu!\n");
}
@end


int main( int argc, char *argv[] )
{

    Gato *meuGato = [Gato new];
    Gato *meuGato2 = [[Gato alloc] init];
    Cachorro *meuCachorro = [[Cachorro alloc] init];
    Cachorro *meuCachorro2 = [[Cachorro alloc] init];
    Cachorro *filhote = [[Cachorro alloc] init];

    [meuGato pesa: 4.2];
    [meuGato2 pesa: 4.9];
    [meuGato miau];
    [meuGato2 miau];

    printf("meuGato pesa: %lf kg\n", [meuGato pesa]);
    printf("meuGato2 pesa: %lf kg\n", [meuGato2 pesa]);

    [meuCachorro pesa: 10.2];
    [meuCachorro2 pesa: 13.3];
    [meuCachorro auau];
    [meuCachorro2 auau];

    printf("meuCachorro pesa: %lf kg\n",[meuCachorro pesa]);
    printf("meuCachorro2 pesa: %lf kg\n",[meuCachorro2 pesa]);

    filhote = [meuCachorro reproduzir: meuCachorro2];
    printf("filhote pesa: %lf kg\n",[filhote pesa]);

    [meuCachorro dealloc];
    [meuCachorro2 dealloc];
    [filhote dealloc];
    [meuGato dealloc];
    [meuGato2 dealloc];

    return 0;
}
