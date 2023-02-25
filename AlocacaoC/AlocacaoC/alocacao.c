#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>


#include "hash.h"

#define bufsize   6144

#define sizeinscricoes 700000   
#define bufinscricoes 1411   

#define sizeopcoes 1000000   
#define bufopcoes 50   

#define bufvagas 16
#define sizevagas 500000


#define pathLog "C:\\SSIS_SEEDUC\\log\\"
#define pathArquivos "C:\\SSIS_SEEDUC\\arquivos\\SAIDA\\"
#define pathArquivosProcessar "C:\\SSIS_SEEDUC\\arquivos\\PROCESSAR\\"

#define false 0
#define true  1

#define off_prioridadeMenorPublico		0			/* MENOR DA REDE PUBLICA */
#define off_prioridadeMenorPrivada		1			/* MENOR DA REDE PRIVADA */
#define off_prioridadeMaiorPublico		2			/* MAIOR DA REDE PUBLICA */
#define off_prioridadeMaiorPrivada		3			/* MAIOR DA REDE PRIVADA */

/* Offset de inscricoes */
#define off_ins_ID_INSCRICAO			0			/* ID_INSCRICAO(9)				 */
#define off_ins_DATA_NASC				69			/* DATA_NASCIMENTO(8)			 */
#define off_ins_ID_REDEDEORIGEM			79		    /* ID_REDEDEORIGEM(6)			 */
#define off_ins_UF_RESID				1062		/* UF_RESIDENCIA(2)				 */
#define off_ins_COD_MUN_EMA				1070		/* COD_MUN_EMA(3)				 */
#define off_ins_LIT_MUN_RESID			1073		/* LIT_MUN_RESID(70)			 */
#define off_ins_COD_BAI_EMA				1145		/* COD_BAI_EMA(3)				 */
#define off_ins_LIT_BAI_RESID			1148		/* LIT_BAI_RESID(60)			 */
#define off_ins_ID_DEFICIENCIA			1268		/* ID_DEFICIENCIA(6)			 */
#define off_ins_ID_PRIORIDADEALOCACAO   1410		/* ID_PRIORIDADEALOCACAO(1)		 */

/* Offset de inscricoes COM RESULTADO*/
#define off_ins_COD_CRITERIO       1411		/* COD_CRITERIO(2)				 */
#define off_ins_IND_ARR_INSCRICAO  1413		/* INDICE DE ARRAY INSCRICAO(10) */
#define off_ins_IND_ARR_OPCAO      1423     /* INDICE DE ARRAY OPCAO(10)	 */
#define off_ins_ID_INSCRICAO_OPCAO 1433     /* ID_INSCRICAO(9)				 */
#define off_ins_ID_OPCAO           1442		/* ID_OPCAO(9)   				 */
#define off_ins_COD_ESC_ALOC	   1451		/* COD_ESCOLA(8)				 */
#define off_ins_COD_CURSO		   1459		/* COD_CURSO(2)					 */
#define off_ins_COD_TURNO		   1461		/* COD_TURNO(1)				 	 */
#define off_ins_OPCAO_ALOC		   1462		/* NUM_OPCAO_ALOCACAO(2)		 */	

/* Offset de opcoes */
#define off_op_ID_INSCRICAO       0         /* ID_INSCRICAO(9)			*/
#define off_op_ID_OPCAOESCOLA	  9		    /* OPCAOESCOLA(9)			*/
#define off_op_CENSO			  18	    /* CENSO(8)					*/
#define off_op_ID_CURSO			  26		/* ID_CURSO(6)				*/
#define off_op_CONEXAO_CURSO	  32		/* CONEXAO_CURSO(7)			*/
#define off_op_CONEXAO_MODALIDADE 39		/* CONEXAO_MODALIDADE(6)	*/
#define off_op_CONEXAO_SERIE	  45		/* CONEXAO_SERIE(1)			*/
#define off_op_CONEXAO_TIPO		  46		/* CONEXAO_TIPO(1)			*/
#define off_op_TURNO			  47		/* TURNO(1)					*/
#define off_op_NUMOPCAO			  48		/* NUM_OPCAO(2)				*/

/* Offset de vagas */
#define off_vg_CENSO			  0			/* CENSO(8)					*/
#define off_vg_ID_CURSO			  8			/* ID_CURSO(2)				*/
#define off_vg_TURNO			  10		/* TURNO(1)					*/
#define off_vg_QTDVAGA			  11		/* QTD VAGA(4)				*/



char path_origem[100];               /* Caminho para layout no passado */
char Nomarq[50];

char Nada = 0;

long int lrecl;         /* Tamanho logico de arquivo    */
char *ptrreg;           /* Pointer para arquivo         */

char strret[255];       /* Variavel estatica para retorno de subrotinas */

char crlf[]={13, 10, 0};

long int totinscricoes;
long int totopcoes;
long int totvagas;
long int totresultado;

hashtable_t *hashtableAlocacao;

/* Variaveis registro de INSCRICOES */
char ins_ID_INSCRICAO[9+2];
char ins_ID_REDEDEORIGEM[6+2];
char ins_ID_INSCRICAO_OPCAO[9+2];
char ins_ID_DEFICIENCIA[6+2];
char ins_ID_PRIORIDADEALOCACAO[1+2];

char ins_COD_CRITERIO[2+2];
char ins_ID_OPCAO[9+2];
char ins_IND_ARR_INSCRICAO[10+2];
char ins_IND_ARR_OPCAO[10+2];

/* Variaveis registro de OPCOES */
char op_ID_INSCRICAO[9+2];
char op_ID_OPCAOESCOLA[9+2];
char op_ID_CURSO[6+2];
char op_CENSO[8+2];
char op_TURNO[1+2];

/* Variaveis registro de VAGAS */
char vg_CENSO[8+2];
char vg_ID_CURSO[2+2];
char vg_TURNO[1+2];
char vg_QTDVAGA[4+2];	


/* Arrays INSCRICAO */
char inscricoes[sizeinscricoes][bufinscricoes+200];

/* Arrays de OPCOES */
char opcoes[sizeopcoes][bufopcoes+10];

/* Arrays de Vagas */
char vagas[sizevagas][bufvagas+10];


char *ZeroEsq(long int valor,long int zeros)
{
	char tmp1[255];
	static char tmp2[255];
	long int a;

	a = zeros;
	sprintf(tmp1,"%%%ld.%ldi",zeros, a);
	sprintf(tmp2,tmp1,valor);
	return(tmp2);
}

char *String(char *letra,int qtd)
{
	int i;
	strret[0]=0;

	if (qtd>254) qtd=254;
	for (i=0;i<qtd;i++)
		strret[i]=letra[0];
	strret[i]=0;
	return(strret);
}

char *Hora()
{
	static char Volta[30];
	struct tm *now;
	time_t t;
	t=time(NULL);
	now=localtime(&t);
	sprintf(Volta,"%0.2d:%0.2d:%0.2d",now->tm_hour,now->tm_min,now->tm_sec);
	return(Volta);
}

char *Left(char campo[],int tam)
{
	int i,j;
	static char tmp[512];

	j=0;
	if (tam==0) tam=strlen(campo);
	tam--;
	for (i=0;i<=tam;i++) {
		tmp[j]=campo[i];
		j++;
	}
	tmp[j]=Nada;
	return(tmp);
}

char *Right(char campo[],int tam)
{
	int i,j,de;
	static char tmp[512];

	j=0;
	if (tam==0) de=0;
	else de=strlen(campo)-tam;
	tam=strlen(campo)-1;
	for (i=de;i<=tam;i++) {
		tmp[j]=campo[i];
		j++;
	}
	tmp[j]=Nada;
	return(tmp);
}

void log_aloca(char mensagem[])
{
	FILE *fp;
	char clinha[50];
	char buflog[20000];

	strcpy(clinha,pathLog);
	strcat(clinha,"logSaida");
	strcat(clinha,".log");

	if ((fp=fopen(clinha,"a+")) == NULL) {
		perror("Erro na gravacao do log\n");
		return;
	}

	strcpy(buflog,Hora());
	strcat(buflog," ");
	strcat(buflog,mensagem);
	strcat(buflog,crlf);

	fputs(buflog,fp);

	/*
	for (i=1;i <= lnhlog;i++)
	{
	strcpy(linha,buferlog[i]);
	fputs(linha,fp);
	}
	*/
	fclose(fp);

}

void Erro(char cod[], char mensagem[])
{
	FILE *fp;
	char tmp[100];
	char clinha[50];


	strcpy(clinha,pathLog);
	strcat(clinha,"logSaida");
	strcat(clinha,".err");

	if ((fp=fopen(clinha,"a+")) == NULL) {
		perror("Erro na gravacao do log de erro\n");
		//exit(1);
		return;
	}
	strcpy(tmp,Hora());
	strcat(tmp," ");
	strcat(tmp,cod);
	strcat(tmp," - ");
	strcat(tmp,mensagem);
	strcat(tmp,crlf);

	fputs(tmp,fp);
	fclose(fp);
}

char *Trim(char *campo)
{
	int i,j,tam;

	strcpy(strret,campo);
	tam=strlen(strret);
	for (i=0;i<tam;i++)
		if (strret[i]!=' ')
			break;
	for (j=tam-1;j>=i;j--)
		if (strret[j]!=' ')
		{
			j++;
			strret[j]=0;
			break;
		}
		return(strret+i);
}

char *Substr(char *campo,int de, int tam)
{
	int i,j;
	strret[0]=0;

	j=0;
	if (tam==0)
		tam=strlen(campo)-de;

	tam=de+tam;
	tam--;
	for (i=de;i<=tam;i++,j++)
		strret[j]=campo[i];
	strret[j]=0;
	return(strret);
}

char *ChangeSubstr(char *campo,char valor[],int pos)
{
	int i,j,tam;

	if (strcmp(valor,"")==0)
	{
		campo[pos]='\0';
	}else{
		j=0;
		strcpy(strret,valor);
		tam=strlen(strret);
		for (i=0;i<tam;i++){
			campo[pos]=valor[i];
			pos++;
		}
	}
	return(campo);
}

long int lerIncricoes()
{
	printf("Lendo Arquivo de Inscricoes...\n");
	FILE *fnum_arq;
	char nomearq[255];
	char tempc[255];
	int ret;
	char inscricao[bufinscricoes+10];

	strcpy(Nomarq,"INSCRITOS_1FASE_MAINFRAME");

	strcpy(nomearq,pathArquivos);
	strcat(nomearq,Nomarq);
	strcat(nomearq,".txt");

	if ((fnum_arq=fopen(nomearq,"rt")) == NULL) {
		strcpy(tempc,"Erro na abertura do arquivo: ");
		strcat(tempc,nomearq);
		strcat(tempc,"\n");
		Erro("15",tempc);
		return 0;

	}

	/* Verifica o tamanho logico do registro  */
	lrecl=bufinscricoes;

	totinscricoes=0;
	ptrreg = fgets(inscricao+1,lrecl+2,fnum_arq); /* Faz deslocamento = Layout */
	while(!feof(fnum_arq))
	{
		strcpy(inscricoes[totinscricoes],inscricao+1);
		ChangeSubstr(inscricoes[totinscricoes], "",bufinscricoes);

		totinscricoes ++;
		ptrreg = fgets(inscricao+1,lrecl+2,fnum_arq); /* Faz deslocamento = Layout */
	}


	fclose(fnum_arq);
	
	//printf("Total de Inscricoes : %s\n", ZeroEsq(totinscricoes, 10));
	
	return totinscricoes;

}

long int lerOpcoes()
{
	printf("Lendo Arquivo de Opcoes dos Alunos...\n");
	FILE *fnum_arq;
	char nomearq[255];
	char tempc[255];
	int ret;
	char opcao[bufopcoes+10];


	strcpy(Nomarq,"OPCOES_MAINFRAME");

	strcpy(nomearq,pathArquivos);
	strcat(nomearq,Nomarq);
	strcat(nomearq,".txt");

	if ((fnum_arq=fopen(nomearq,"rt")) == NULL) {
		strcpy(tempc,"Erro na abertura do arquivo: ");
		strcat(tempc,nomearq);
		strcat(tempc,"\n");
		Erro("15",tempc);
		return 0;
	}

	/* Verifica o tamanho logico do registro  */
	lrecl=bufopcoes;

	totopcoes=0;
	ptrreg = fgets(opcao+1,lrecl+2,fnum_arq); /* Faz deslocamento = Layout */
	while(!feof(fnum_arq))
	{
		strcpy(opcoes[totopcoes],opcao+1);
		totopcoes ++;

		ptrreg = fgets(opcao+1,lrecl+2,fnum_arq); /* Faz deslocamento = Layout */
	}


	fclose(fnum_arq);

	//printf("Total de Opções dos Alunos : %s\n", totopcoes);

	return totopcoes;

}

long int lerVagas()
{
	printf("Lendo Arquivo de Vagas...\n");
	FILE *fnum_arq;
	char nomearq[255];
	char tempc[255];
	char vaga[bufvagas+10];

	int ret;

	strcpy(Nomarq,"VAGAS_1FASE_MAINFRAME");

	strcpy(nomearq,pathArquivos);
	strcat(nomearq,Nomarq);
	strcat(nomearq,".txt");

	if ((fnum_arq=fopen(nomearq,"rt")) == NULL) {
		strcpy(tempc,"Erro na abertura do arquivo: ");
		strcat(tempc,nomearq);
		strcat(tempc,"\n");
		Erro("15",tempc);
		return 0;
	}

	/* Verifica o tamanho logico do registro  */
	lrecl=bufvagas;

	totvagas=0;
	ptrreg = fgets(vaga+1,lrecl+2,fnum_arq); /* Faz deslocamento = Layout */
	while(!feof(fnum_arq))
	{
		strcpy(vagas[totvagas],vaga+1);
		totvagas ++;

		ptrreg = fgets(vaga+1,lrecl+2,fnum_arq); /* Faz deslocamento = Layout */
	}

	fclose(fnum_arq);

	//printf("Total de Vagas : %s\n", totvagas);


	return totvagas;

}

void montarHashTable(){
	long int iCount;

	char KEY[30];
	char VALUE[30];

	printf("Montando o Hash nas Tabelas em Memoria...\n");

	hashtableAlocacao = ht_create( totopcoes + totvagas + 100 );
	for (iCount=0;iCount<=totopcoes;iCount++){

		strcpy(KEY,"OP_");
		strcat(KEY,Substr(opcoes[iCount], off_op_ID_INSCRICAO,9));
		strcat(KEY,Substr(opcoes[iCount], off_op_NUMOPCAO,2));

		strcpy(VALUE,ZeroEsq(iCount,10));
		ht_set( hashtableAlocacao, KEY, VALUE);

	}

	for (iCount=0;iCount<=totvagas;iCount++){

		strcpy(KEY,"VG_");
		strcat(KEY,Substr(vagas[iCount], off_vg_CENSO,8));
		strcat(KEY,Substr(vagas[iCount], off_vg_ID_CURSO,2));
		strcat(KEY,Substr(vagas[iCount], off_vg_TURNO,1));

		strcpy(VALUE,Substr(vagas[iCount], off_vg_QTDVAGA,4));
		ht_set( hashtableAlocacao, KEY, VALUE);

	}

}

long int realizarAlocacao(int processarDeficiente,int processarMenorPublico){

	long int iInscricao;
	long int iOpcao;
	char KEY[30];
	char VALUE[30];
	char cTemp[500];
	long int nTemp;
	long int iArrayOpcao;
	char COD_CRITERIO[4];

	int podeExecutar;

	totresultado=0;
	
	printf("Iniciando Alocacao...\n");

//VAMOS VARRER TODO O ARQUIVO DE INSCRIÇÃO	
	for (iInscricao=0;iInscricao<totinscricoes;iInscricao++){
		
		strcpy(ins_ID_INSCRICAO,Substr(inscricoes[iInscricao], off_ins_ID_INSCRICAO,9));
		strcpy(ins_ID_DEFICIENCIA,Substr(inscricoes[iInscricao], off_ins_ID_DEFICIENCIA,6));
		strcpy(ins_COD_CRITERIO,Substr(inscricoes[iInscricao], off_ins_COD_CRITERIO,2));
		strcpy(ins_ID_REDEDEORIGEM,Substr(inscricoes[iInscricao], off_ins_ID_REDEDEORIGEM,6));
		strcpy(ins_ID_PRIORIDADEALOCACAO,Substr(inscricoes[iInscricao], off_ins_ID_PRIORIDADEALOCACAO,1));
		
		podeExecutar = false;

		printf("Alocando Inscricao...%s\n", ins_ID_INSCRICAO);

		// SE NAO TEM ALOCACAO VAMOS VER SE ATENDE AOS REQUISITOS
		if (strcmp(ins_COD_CRITERIO,"")==0){
		
			// Verificar se aluno é deficiente
			if (atoi(ins_ID_DEFICIENCIA)>0){
				strcpy(COD_CRITERIO,"01");
				if (processarDeficiente) podeExecutar = true;

			
			}else{
				podeExecutar = true;
				
				nTemp = atoi(ins_ID_PRIORIDADEALOCACAO);
				switch (nTemp)
				{
				case off_prioridadeMenorPublico:
					strcpy(COD_CRITERIO,"03");
					break;
				
				case off_prioridadeMenorPrivada:
					strcpy(COD_CRITERIO,"05");
					break;
				
				case off_prioridadeMaiorPublico:
					strcpy(COD_CRITERIO,"07");
					break;
				
				case off_prioridadeMaiorPrivada:
					strcpy(COD_CRITERIO,"09");
					break;

				default:
					podeExecutar = false;
					break;
				}
			}

		}
		
		
		// SE ATENDE AOS REQUISITOS DA EXECUCAO
		if (podeExecutar){

			// MONTA CHAVE DE OPCAO E PROCURA NAS OCORRENCIAS DE ESCOLHA, RETORNA A POSICAO DO ARRAY DE OPCOES
			// VAMOS RODAR AS OPÇÕES JÁ ORDENADAS VINDAS DO CONEXÃO EDUCAÇÃO
			for (iOpcao=1;iOpcao<=20;iOpcao++){		
				strcpy(KEY,"OP_");
				strcat(KEY,ins_ID_INSCRICAO);
				strcat(KEY,ZeroEsq(iOpcao,2));

				strcpy(VALUE, ht_get( hashtableAlocacao, KEY));

				// ACHOU O REGISTRO?
				if (strlen(Trim(VALUE))>0){

					// TRATA RETORNO E CAPTURA POSICAO DO ARRAY
					nTemp = atoi(VALUE);
					iArrayOpcao = nTemp;

					strcpy(op_ID_INSCRICAO,		Substr(opcoes[nTemp], off_op_ID_INSCRICAO,9));
					strcpy(op_ID_OPCAOESCOLA,	Substr(opcoes[nTemp], off_op_ID_OPCAOESCOLA,9));
					strcpy(op_CENSO,			Trim(Substr(opcoes[nTemp], off_op_CENSO,8)));
					strcpy(op_ID_CURSO,			Trim(Substr(opcoes[nTemp], off_op_ID_CURSO,6)));
					strcpy(op_TURNO,			Substr(opcoes[nTemp], off_op_TURNO,1));

					if (strcmp(ins_ID_INSCRICAO,op_ID_INSCRICAO)==0){

						strcpy(KEY,"VG_");
						strcat(KEY,op_CENSO);
						strcat(KEY,Right(op_ID_CURSO,2));
						strcat(KEY,op_TURNO);

						if (strlen(KEY)<14)
						{
							strcpy(cTemp,"		ERRO ");
							strcat(cTemp,KEY);
							strcat(cTemp,"		INVALIDA ");
							log_aloca(cTemp);
						}
						// Localiza a chave na memória 
						strcpy(VALUE,"0");
						strcat(VALUE, ht_get( hashtableAlocacao, KEY));
						nTemp=atoi(VALUE);
						// se for > 0 tem vaga
						// SE TEM VAGA?
						if (nTemp>0){
							
							nTemp--;

							ht_set( hashtableAlocacao, KEY, ZeroEsq(nTemp,4));

							strcpy(cTemp,COD_CRITERIO);					// COD_CRITERIO
							strcat(cTemp,ZeroEsq(iInscricao,10));		// INDICE DO ARRAY DE INSCRICAO
							strcat(cTemp,ZeroEsq(iArrayOpcao,10));		// INDICE ARRAY OPCAO
							strcat(cTemp,op_ID_INSCRICAO);				// ID_INSCRICAO
							strcat(cTemp,op_ID_OPCAOESCOLA);			// ID_OPCAO
							
							strcat(cTemp,op_CENSO);						// COD_ESC_ALOC
							strcat(cTemp,Right(op_ID_CURSO,2));			// COD_CURSO
							strcat(cTemp,op_TURNO);						// COD_TURNO
							strcat(cTemp,ZeroEsq(iOpcao,2));			// OPCAO_ALOC

							strcat(inscricoes[iInscricao],cTemp);
							totresultado++;

							// JA ENCONTREI VAGA FORÇAR SAIDA DO FOR DE OPÇÕES 
							break;

						}
					}
				}			
				else{
					if (atoi(Right(KEY,2))<3){
						strcpy(cTemp,"ERRO ");
						strcat(cTemp,KEY);
						strcat(cTemp," NAO ENCONTRADO ");
						strcat(cTemp," NA INSCRICAO ");
						strcat(cTemp,ins_ID_INSCRICAO);
						Erro("404", cTemp);
					}
				}
			}
		}
	}

	//printf("Total de Alocados : %s\n", totresultado);
	return totresultado;
}

void gravarResultado(){

	long int iResultado;
	
	char bufInscricao[bufinscricoes+3000];

	printf("Gerando o Arquivo de Alocados...\n");

	FILE *fp;
	char clinha[50];

	strcpy(clinha,pathArquivosProcessar);
	strcat(clinha,"alocados");
	strcat(clinha,".csv");

	if ((fp=fopen(clinha,"w+")) == NULL) {
		perror("Erro na gravacao do arquivo\n");
		return;
	}

	for (iResultado=0;iResultado<totinscricoes;iResultado++){	
		
		// SE TEM ALOCACAO VAMOS LISTAR
		if (atoi(Substr(inscricoes[iResultado],off_ins_COD_CRITERIO,2))>0){
			
			strcpy(bufInscricao,Substr(inscricoes[iResultado],off_ins_ID_INSCRICAO,9));
			strcat(bufInscricao,";");
		
			strcat(bufInscricao,Substr(inscricoes[iResultado],off_ins_COD_ESC_ALOC,8));
			strcat(bufInscricao,";");
		
			strcat(bufInscricao,Substr(inscricoes[iResultado],off_ins_COD_CURSO,2));
			strcat(bufInscricao,";");
		
			strcat(bufInscricao,Substr(inscricoes[iResultado],off_ins_COD_TURNO,1));
			strcat(bufInscricao,";");
	
			strcat(bufInscricao,Substr(inscricoes[iResultado],off_ins_COD_CRITERIO,2));
			strcat(bufInscricao,";");
		
			strcat(bufInscricao,Substr(inscricoes[iResultado],off_ins_OPCAO_ALOC,2));
			strcat(bufInscricao,";");
		
			strcat(bufInscricao,Substr(inscricoes[iResultado],off_ins_UF_RESID,2));
			strcat(bufInscricao,";");
		
			strcat(bufInscricao,Substr(inscricoes[iResultado],off_ins_COD_MUN_EMA,3));
			strcat(bufInscricao,";");

			strcat(bufInscricao,Substr(inscricoes[iResultado],off_ins_LIT_MUN_RESID,30));
			strcat(bufInscricao,";");
		
			strcat(bufInscricao,Substr(inscricoes[iResultado],off_ins_COD_BAI_EMA,3));
			strcat(bufInscricao,";");

			strcat(bufInscricao,Substr(inscricoes[iResultado],off_ins_LIT_BAI_RESID,30));
			strcat(bufInscricao,";");
		
			strcat(bufInscricao,Substr(inscricoes[iResultado],off_ins_DATA_NASC,8));
			strcat(bufInscricao,";");

			strcat(bufInscricao,crlf);

			fputs(bufInscricao,fp);
		}
	}
	
	fclose(fp);

}

int main(int totparms,char *parm[])
{
	long int nTemp;
	char cTemp[500];
	long int iVagas;
	char KEY[30];


	// APAGANDO LOG E LOG DE ERRO **************************************
	strcpy(cTemp,pathLog);
	strcat(cTemp,"logSaida");
	strcat(cTemp,".log");

	remove(cTemp);

	strcpy(cTemp,pathLog);
	strcat(cTemp,"logSaida");
	strcat(cTemp,".err");

	remove(cTemp);
	// ****************************************************************

	log_aloca(" ********** INICIO EXECUCAO *******************");

	log_aloca(" LER INSCRICOES");

	nTemp = lerIncricoes();
	sprintf(cTemp, "	%d Registros", nTemp);
	log_aloca(cTemp);

	log_aloca(" LER OPCOES");
	nTemp = lerOpcoes();
	sprintf(cTemp, "	%d Registros", nTemp);
	log_aloca(cTemp);

	log_aloca(" LER VAGAS");
	nTemp = lerVagas();
	sprintf(cTemp, "	%d Registros", nTemp);
	log_aloca(cTemp);

	log_aloca(" MONTAR HASHTABLE");
	montarHashTable();

	log_aloca(" REALIZAR ALOCACAO");
	nTemp = realizarAlocacao(true,true);
	sprintf(cTemp, "	%d Registros", nTemp );
	log_aloca(cTemp);
	
	log_aloca(" GRAVAR RESULTADO DA ALOCACAO");
	gravarResultado();
	
	log_aloca(" ********** FIM EXECUCAO *******************");
	//getch();
	return 0;
}