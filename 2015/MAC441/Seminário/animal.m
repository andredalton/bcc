#import <stdio.h>
#import <objc/Object.h>
#import <Foundation/NSObject.h>

// interfaces
@interface Animal : NSObject
{
    double peso;
    double pesoInicial;
}
-(id) init: (double) umPesoInicial;
-(void) dealloc;
-(void) pesa: (double) p; // setter
-(double) pesa;           // getter
-(void) come: (double) p;
-(id) reproduzir: (id) Animal;
@end

@interface Gato: Animal
-(void) come: (double) p;
@end

@interface Vaca: Animal
-(void) come: (double) p;
@end


// implementacoes
@implementation Animal
-(id) init: (double) umPesoInicial
{
    if (self = [super init])
    {
        peso = pesoInicial = umPesoInicial;
    }
    return self;
}

-(void) dealloc
{
    printf("Estou morrendo!\n"
           "Na minha vida, engordei %lf kg\n", peso - pesoInicial);
    [super dealloc];
}

-(void) pesa: (double) p
{
    peso = p;
}

-(double) pesa
{
    return peso;
}

-(void) come: (double) p
{
    peso += p;
}

// a chamada será Animal reproduzir: Animal2, e devolve um novo objeto da mesma espécie
// que Animal e Animal2.
-(id) reproduzir: (id) animal2
{
    if ([self class] == [animal2 class])
    {
        id filho=[[[self class] alloc] init];
    	printf("O filho nasceu sadio!\n");
    	[filho pesa: ([self pesa]+[animal2 pesa])/2];
    	return filho;
    }
    else
    {
    	printf("O filho nasceu morto, pois os pais são de classes diferentes!\n");
    	return nil;
//    	[Filho pesa: 0];
    }
}

@end

@implementation Gato
-(void) come: (double) p
{
    printf("Que peixe gostoso!\n");
    [super come: p/2];
}
@end

@implementation Vaca
-(void) come: (double) p
{
    printf("Muuuuu! ");
    [super come: p*2];
}
@end

int main( int argc, char *argv[] )
{

    Vaca *vaca = [[Vaca alloc] init: 100];
    Gato *gato = [[Gato alloc] init: 10];    
    Gato *filhote ;

    
    printf("Vaca pesa: %lf kg\n", [vaca pesa]);
    [vaca come: 20];
    printf("Vaca pesa: %lf kg\n", [vaca pesa]);
    

    printf("Gato pesa: %lf kg\n", [gato pesa]);
    [gato come: 5];
    printf("Gato pesa: %lf kg\n", [gato pesa]);

    filhote = [vaca reproduzir: gato];
    
    printf("Filhote pesa: %lf kg\n", [filhote pesa]);

    [vaca release];
    
    return 0;
}

/*
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
}*/
