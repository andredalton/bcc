#import <stdio.h>
#import <objc/Object.h>
#import <Foundation/NSString.h>
#import <Foundation/NSObject.h>
#import <Foundation/NSDictionary.h>


@interface Partida: NSObject
{
  NSString* timeCasa;
  int golsTimeCasa;
  NSString* timeVisitante;
  int golsTimeVisitante;
}
-(void) adicionaJogoDo: (NSString*) timeCasa
        contraO:        (NSString*) timeVisitante
        placar:         (int)   golsCasa
        a:              (int)   golsVisitante;
-(void) imprimeJogo;
@end


@implementation Partida
-(void) adicionaJogoDo: (NSString*) time1
        contraO:        (NSString*) time2
        placar:         (int)   golsCasa
        a:              (int)   golsVisitante;
{
  timeCasa = time1;
  timeVisitante = time2;
  golsTimeCasa = golsCasa;
  golsTimeVisitante = golsVisitante;
}
-(void) imprimeJogo;
{
  printf( "O jogo do %s contra o %s foi %d a %d\n",
          timeCasa, timeVisitante,
          golsTimeCasa, golsTimeVisitante);
}
@end


int main( int argc, const char *argv[] )
{
  Partida *partida = [[Partida alloc] init];
        
  [partida adicionaJogoDo: @"Alemanha"
           contraO: @"Brasil"
           placar: 7 a: 1];
 
       
  [partida imprimeJogo];
        
  [partida release];
        
  return 0;
}
