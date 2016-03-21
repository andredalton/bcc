/* Processed by ecpg (4.8.0) */
/* These include files are added by the preprocessor */
#include <ecpglib.h>
#include <ecpgerrno.h>
#include <sqlca.h>
/* End of automatic include section */

#line 1 "ex_ecpg.pgc"
#include <stdio.h>
#include <stdlib.h>

#define NO_ERRORS !(strcmp(SQLSTATE,"00000"))
#define NO_MORE_TUPLES !(strcmp(SQLSTATE,"02000"))

/*  Inclui um novo estudio na tabela Estudio, 
   solicitando o seu nome e endereco ao 
   usuario.  */
void InsereEstudio() {
	/* exec sql begin declare section */
		  
	
#line 12 "ex_ecpg.pgc"
 char nomeEstudio [ 50 ] , enderecoEstudio [ 256 ] ;
/* exec sql end declare section */
#line 13 "ex_ecpg.pgc"


	/* exec sql whenever sqlerror  sqlprint ; */
#line 15 "ex_ecpg.pgc"
 //ou  EXEC SQL WHENEVER SQLERROR GOTO sql_error;
 
	printf("\n*** Insercao de um novo estudio ***\n");	
	printf("Digite o nome do estudio: ");
	scanf("%[^\n]", nomeEstudio);
	getchar(); /* elimina o enter armazenado no buffer de leitura */
	printf("Digite o endereco do estudio: ");
	scanf("%[^\n]", enderecoEstudio);
	getchar();

	{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "insert into Estudio ( nome , endereco ) values ( $1  , $2  )", 
	ECPGt_char,(nomeEstudio),(long)50,(long)1,(50)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,(enderecoEstudio),(long)256,(long)1,(256)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);
#line 25 "ex_ecpg.pgc"

if (sqlca.sqlcode < 0) sqlprint();}
#line 25 "ex_ecpg.pgc"


	{ ECPGtrans(__LINE__, NULL, "commit");
#line 27 "ex_ecpg.pgc"

if (sqlca.sqlcode < 0) sqlprint();}
#line 27 "ex_ecpg.pgc"

/*
	return;
sql_error:
	printf("ERRO: %s\n", SQLSTATE);
*/
}

/*  Solicita o nome de um estudio e exibe o
    patrimonio do presidente do estudio  */
void ExibePatrimonio() {
	/* exec sql begin declare section */
		  
		 
	
#line 39 "ex_ecpg.pgc"
 char nomeEstudio [ 50 ] ;
 
#line 40 "ex_ecpg.pgc"
 int patrimonioPresidente ;
/* exec sql end declare section */
#line 41 "ex_ecpg.pgc"


	printf("\n*** Mostra o patrimonio do presidente do estudio ***\n");	
	printf("Digite o nome do estudio: ");
	scanf("%[^\n]", nomeEstudio);
	getchar();

	{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "select patrimonio from Estudio , ExecutivoDeCinema where num_certificado_presidente = num_certificado and Estudio . nome = $1 ", 
	ECPGt_char,(nomeEstudio),(long)50,(long)1,(50)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, 
	ECPGt_int,&(patrimonioPresidente),(long)1,(long)1,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);
#line 50 "ex_ecpg.pgc"

if (sqlca.sqlcode < 0) sqlprint();}
#line 50 "ex_ecpg.pgc"


	if (NO_ERRORS) 
		printf("O patrimonio do presidente do estudio eh: %d \n", patrimonioPresidente);
	else
		printf("Nao foi possivel responder a consulta.\n");
}

/*
  Separa os executivos em faixas de patrimonio e  
  mostra quantos executivos estao em cada faixa.
  Dois executivos estao em uma mesma faixa se
  seus rendimentos possuem a mesma quantidade de
  digitos. 
*/
void FaixasPatrimonio() {
	int i, digitos, contadores[15];
	/* exec sql begin declare section */
		 
	
#line 68 "ex_ecpg.pgc"
 int patrimonio ;
/* exec sql end declare section */
#line 69 "ex_ecpg.pgc"


	/* declare cursorExecutivos cursor for select patrimonio from ExecutivoDeCinema */
#line 72 "ex_ecpg.pgc"

	
	{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "declare cursorExecutivos cursor for select patrimonio from ExecutivoDeCinema", ECPGt_EOIT, ECPGt_EORT);
#line 74 "ex_ecpg.pgc"

if (sqlca.sqlcode < 0) sqlprint();}
#line 74 "ex_ecpg.pgc"

	printf("\n*** Numero de executivos por faixa de patrimonio ***\n");	
	for(i = 0; i<15; i++)  contadores[i] = 0;
	while (1) {
		{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "fetch from cursorExecutivos", ECPGt_EOIT, 
	ECPGt_int,&(patrimonio),(long)1,(long)1,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);
#line 78 "ex_ecpg.pgc"

if (sqlca.sqlcode < 0) sqlprint();}
#line 78 "ex_ecpg.pgc"

		if (NO_MORE_TUPLES) break;
		digitos = 1;
		while ((patrimonio /= 10) > 0) digitos++;
			if (digitos <= 14) contadores[digitos]++;
	}
	{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "close cursorExecutivos", ECPGt_EOIT, ECPGt_EORT);
#line 84 "ex_ecpg.pgc"

if (sqlca.sqlcode < 0) sqlprint();}
#line 84 "ex_ecpg.pgc"

	for (i= 0; i<15; i++)
		printf("digitos = %d: numero de executivos = %d\n", i, contadores[i]) ;
}

/* Modifica o patrimonio dos executivos */
void MudaPatrimonio() {
	/* exec sql begin declare section */
		 
		  
		  
	
#line 92 "ex_ecpg.pgc"
 int numCert ;
 
#line 93 "ex_ecpg.pgc"
 long patrimonio ;
 
#line 94 "ex_ecpg.pgc"
 char nomeExec [ 31 ] , endExec [ 256 ] ;
/* exec sql end declare section */
#line 95 "ex_ecpg.pgc"
 

	/* exec sql whenever sqlerror  goto  sql_error ; */
#line 97 "ex_ecpg.pgc"


	/* declare cursorExecutivos2 cursor for select * from ExecutivoDeCinema */
#line 99 "ex_ecpg.pgc"

	{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "declare cursorExecutivos2 cursor for select * from ExecutivoDeCinema", ECPGt_EOIT, ECPGt_EORT);
#line 100 "ex_ecpg.pgc"

if (sqlca.sqlcode < 0) goto sql_error;}
#line 100 "ex_ecpg.pgc"


	printf("\n*** Modifica o patrimonio dos executivos ***\n");	
	while (1) {
		{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "fetch from cursorExecutivos2", ECPGt_EOIT, 
	ECPGt_int,&(numCert),(long)1,(long)1,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,(nomeExec),(long)31,(long)1,(31)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,(endExec),(long)256,(long)1,(256)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_long,&(patrimonio),(long)1,(long)1,sizeof(long), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);
#line 104 "ex_ecpg.pgc"

if (sqlca.sqlcode < 0) goto sql_error;}
#line 104 "ex_ecpg.pgc"

	if (NO_MORE_TUPLES) break;
	if (patrimonio < 1000)
		{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "delete from ExecutivoDeCinema where current of cursorExecutivos2", ECPGt_EOIT, ECPGt_EORT);
#line 108 "ex_ecpg.pgc"

if (sqlca.sqlcode < 0) goto sql_error;}
#line 108 "ex_ecpg.pgc"

	else
		{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "update ExecutivoDeCinema set patrimonio = 0.5 * patrimonio where current of cursorExecutivos2", ECPGt_EOIT, ECPGt_EORT);
#line 111 "ex_ecpg.pgc"

if (sqlca.sqlcode < 0) goto sql_error;}
#line 111 "ex_ecpg.pgc"

	}
	{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "close cursorExecutivos2", ECPGt_EOIT, ECPGt_EORT);
#line 113 "ex_ecpg.pgc"

if (sqlca.sqlcode < 0) goto sql_error;}
#line 113 "ex_ecpg.pgc"

	
	{ ECPGtrans(__LINE__, NULL, "commit");
#line 115 "ex_ecpg.pgc"

if (sqlca.sqlcode < 0) goto sql_error;}
#line 115 "ex_ecpg.pgc"

	return;

sql_error:
	printf("ERRO: %s - %s\n", SQLSTATE, sqlca.sqlerrm.sqlerrmc);
}

/* Executa um comando SQL qualquer digitado pelo usuario */
void LeComando(){
	/* exec sql begin declare section */
		 
	
#line 125 "ex_ecpg.pgc"
 char consulta [ 1024 ] ;
/* exec sql end declare section */
#line 126 "ex_ecpg.pgc"


	printf("\n*** Executa um comando dinamico ***\n");	
	printf("Digite um comando SQL de modificação do BD: ");
	scanf("%[^\n]", consulta);
	getchar();

	/* exec sql whenever sqlerror  goto  sql_error ; */
#line 133 "ex_ecpg.pgc"


	{ ECPGprepare(__LINE__, NULL, 0, "consultausuario", consulta);
#line 135 "ex_ecpg.pgc"

if (sqlca.sqlcode < 0) goto sql_error;}
#line 135 "ex_ecpg.pgc"

	{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_execute, "consultausuario", ECPGt_EOIT, ECPGt_EORT);
#line 136 "ex_ecpg.pgc"

if (sqlca.sqlcode < 0) goto sql_error;}
#line 136 "ex_ecpg.pgc"

	//ou EXEC SQL EXECUTE IMMEDIATE :consulta;

	return;

sql_error:
	printf("ERRO: %s - %s\n %s", SQLSTATE, sqlca.sqlerrm.sqlerrmc, consulta);
}

void ObtemData(){
	/* exec sql begin declare section */
		 
		 
		 
	
#line 147 "ex_ecpg.pgc"
 char data_hora [ 30 ] ;
 
#line 148 "ex_ecpg.pgc"
 char data [ 11 ] ;
 
#line 149 "ex_ecpg.pgc"
 char hora [ 19 ] ;
/* exec sql end declare section */
#line 150 "ex_ecpg.pgc"


	/* exec sql whenever sqlerror  sqlprint ; */
#line 152 "ex_ecpg.pgc"

	
	{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "select current_timestamp", ECPGt_EOIT, 
	ECPGt_char,(data_hora),(long)30,(long)1,(30)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);
#line 154 "ex_ecpg.pgc"

if (sqlca.sqlcode < 0) sqlprint();}
#line 154 "ex_ecpg.pgc"
 
	printf("\nData e hora: %s \n\n", data_hora);	
	
	{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "select current_date", ECPGt_EOIT, 
	ECPGt_char,(data),(long)11,(long)1,(11)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);
#line 157 "ex_ecpg.pgc"

if (sqlca.sqlcode < 0) sqlprint();}
#line 157 "ex_ecpg.pgc"

	printf("\nSomente data: %s \n\n", data);	

        { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "select current_time", ECPGt_EOIT, 
	ECPGt_char,(hora),(long)19,(long)1,(19)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);
#line 160 "ex_ecpg.pgc"

if (sqlca.sqlcode < 0) sqlprint();}
#line 160 "ex_ecpg.pgc"

	printf("\nSomente hora: %s \n\n", hora);	
}

int main(){
	/* exec sql begin declare section */
		/* Substitua os dados de conexao abaixo pelos seus dados */
		    
		    
		    
	
#line 167 "ex_ecpg.pgc"
 const char * bd_url = "avale@linux.ime.usp.br:5432" ;
 
#line 168 "ex_ecpg.pgc"
 const char * usuario = "avale" ;
 
#line 169 "ex_ecpg.pgc"
 const char * senha = "z5&wh$" ;
/* exec sql end declare section */
#line 170 "ex_ecpg.pgc"


	/* exec sql whenever sqlerror  goto  sql_error ; */
#line 172 "ex_ecpg.pgc"
 

        { ECPGconnect(__LINE__, 0, bd_url , usuario , senha , "conexaoBD", 0); 
#line 174 "ex_ecpg.pgc"

if (sqlca.sqlcode < 0) goto sql_error;}
#line 174 "ex_ecpg.pgc"


	{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "set search_path to 'bd_filmes'", ECPGt_EOIT, ECPGt_EORT);
#line 176 "ex_ecpg.pgc"

if (sqlca.sqlcode < 0) goto sql_error;}
#line 176 "ex_ecpg.pgc"


	/* Testa as funcoes criadas */
	InsereEstudio();
	if (NO_ERRORS) {
		ExibePatrimonio();
		if (NO_ERRORS) { 
			FaixasPatrimonio();
			if (NO_ERRORS) {
				MudaPatrimonio(); 
				if (NO_ERRORS) { 
					LeComando();
					if (NO_ERRORS)  
        					ObtemData(); 
				}
			}
		}
	}
        
        { ECPGdisconnect(__LINE__, "conexaoBD");
#line 195 "ex_ecpg.pgc"

if (sqlca.sqlcode < 0) goto sql_error;}
#line 195 "ex_ecpg.pgc"


	return 0;

sql_error:
	printf("ERRO: %s - %s\n", SQLSTATE, sqlca.sqlerrm.sqlerrmc);

	return 1;
}
