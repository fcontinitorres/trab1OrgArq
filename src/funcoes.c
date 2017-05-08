#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/funcoes.h"
#include "../src/parametros.h"

//*******************//
//* FUNÇÕES DO MAIN *//
//*******************//

/*	Descrição:
		Abre os arquivos de entrada e saída do programa
	Parêmetros:
		fileIn = arquivo de entrada
		fileOut = arquivo de saída
	Retorno:
		Retorna um int que indica o sucesso da função. Os possíves valores são:
		1 -> Arquivos aberto com sucesso 
		2 -> Falha ao abrir arquivos */
int validaArquivos(FILE **fileIn, FILE **fileOut){
	
	// abrindo arquivos
    *fileIn  = fopen(FILE_IN, "r");
    *fileOut = fopen(FILE_OUT, "wb+");

    if (*fileIn == NULL){
    	printf("Erro ao abrir: %s\n", FILE_IN);
    	return(0);
    }

    if (*fileOut == NULL){
    	printf("Erro ao abrir: %s\n", FILE_OUT);
    	return(0);
    }

    return(1);
}

/*	Descrição:
		Realiza as tarefas para executar a primeira opção do menu
	Parêmetros:
		file = arquivo de saida (binário) */
void opcao1(FILE *file){
	// lista todos os dados
	listBin(file);
	
	// reseta ponteiro do arquivo
	fseek(file, 0, SEEK_SET);
}

/*	Descrição:
		Realiza as tarefas para executar a segunda opção do menu
	Parêmetros:
		file = arquivo de saida (binário) */
void opcao2(FILE *file){
	int field; // campo que a busca será realizado
	char *strBusca; // valor que a busca irá usar para comparar
	Registro *reg; // registro resultante da busca

	// espera campo de busca
	printf("1 - CNPJ\n");
	printf("2 - Razao Social\n");
	printf("3 - Nome Fantasia\n");
	printf("4 - Data do Registro\n");
	printf("5 - Data do Cancelamento\n");
	printf("6 - Motivo do Cancelamento\n");
	printf("7 - Nome da Empresa de Auditoria\n");
	printf("8 - CNPJ da Empresa de Auditoria\n");
	printf("Escolha um dos campos para fazer a busca: ");
	scanf("%d", &field);
	getchar();

	// valida campo de busca
	if (field < 1 || field > 8){
		printf("\nOpcao invalida.\n");
		return;
	}

	// espera o valor da busca
	printf("Informe o valor a ser buscado (substitua ' ' por ';'): ");
	strBusca = NULL;
	scanf("%ms", &strBusca);
	// converte string informada trocando ';' por ' '
	strBusca = convertStr(strBusca);
	printf("\n");

	// busca
	reg = buscaCampo(file, field, strBusca);

	if (reg != NULL){
		printReg(reg);
		free(reg->razSoc);
		free(reg->nomeFant);
		free(reg->motCanc);
		free(reg->nomeEmp);
		free(reg);
	}
	else
		printf("Não foi possível localizar o registro.\n");				

	// reseta ponteiro do arquivo
	fseek(file, 0, SEEK_SET);
}

/*	Descrição:
		Realiza as tarefas para executar a terceira opção do menu
	Parêmetros:
		file = arquivo de saida (binário) */
void opcao3(FILE *file){ 
	int regBusca; // localização do registro que será buscado
	Registro *reg; // resultado da busca

	// espera localização do registro
	printf("Informe o numero do registro desejado (1...n): ");
	scanf("%d", &regBusca);
	getchar();
	printf("\n");

	// valida a localização do registro
	if (regBusca < 1){
		printf("\nOpcao invalida.\n");
		return;
	}

	// busca
	reg = buscaReg(file, regBusca);

	if (reg != NULL)
		printReg(reg);
	else
		printf("Não foi possível localizar o registro.\n");				

	// reseta ponteiro do arquivo
	fseek(file, 0, SEEK_SET);
}

/*	Descrição:
		Realiza as tarefas para executar a quarta opção do menu
	Parêmetros:
		file = arquivo de saida (binário) */
void opcao4(FILE *file){
	int regBusca; // localização do registro que será buscado 
	int fieldBusca; // indica qual campo sera buscado e retornado
	char *result; // resultado da busca

	// espera campo de busca
	printf("1 - CNPJ\n");
	printf("2 - Razao Social\n");
	printf("3 - Nome Fantasia\n");
	printf("4 - Data do Registro\n");
	printf("5 - Data do Cancelamento\n");
	printf("6 - Motivo do Cancelamento\n");
	printf("7 - Nome da Empresa de Auditoria\n");
	printf("8 - CNPJ da Empresa de Auditoria\n");
	printf("Escolha um dos campos para fazer a busca: ");
	scanf("%d", &fieldBusca);
	getchar();

	// valida campo de busca
	if (fieldBusca < 1 || fieldBusca > 8){
		printf("\nOpcao invalida.\n");
		return;
	}

	// espera localização do registro
	printf("Informe o numero do registro desejado (1...n): ");
	scanf("%d", &regBusca);
	getchar();

	// valida a localização do registro
	if (regBusca < 1){
		printf("\nOpcao invalida.\n");
		return;
	}

	// busca
	result = buscaRegCampo(file, regBusca, fieldBusca);

	if (result != NULL){
		printf("\n%s\n", result);
		free(result);
	}else 
		printf("\nRegistro não encontrado\n");

	// reseta ponteiro do arquivo
	fseek(file, 0, SEEK_SET);
}

/*	Descrição:
		Dada uma string a função substitui os caracteres ';' por ' ' 
	Parêmetros:
		str = string que será convertida
	Retorno:
		Retorna a string convertida, caso a string dada seja NULL o retorno também será */
char* convertStr(char *str){
	int i; // indice que será percorrido na string
	char *retorno = NULL; // string de retorno

	if (str != NULL) {
		// aloca strlen(str)+1 posicoes para incluir o '\0'
		retorno = (char *)malloc(strlen(str)+1);
		// percorre até o '\0' fazendo a substituição quando necessária
		for(i=0; i < strlen(str)+1; i++){
			if (str[i] == ';')
				retorno[i] = ' ';
			else
				retorno[i] = str[i];
		}
	}
	return retorno;
}
//-------------------//



//*****************************************************//
//* FUNÇÕES PARA GERAR O ARQUIVO BINÁRIO DE REGISTROS *//
//*****************************************************//

/*	Descrição:
		Lê os dados em um arquivo de entrada e os salva de forma organizada em um arquivo de saída
	Parêmetros:
		fileIn = Arquivo de entrada
		fileOut = Arquvio de saída */
void csv2Bin(FILE *fileIn, FILE *fileOut){
    char c = 'c'; // caracter que irá percorrer o arquivo, deve ser inicializado com um valor != de EOF
    char string[18];
    int field;  // indica qual campo do arquivo está sendo lido, seus valores vão de 0 a 7
    int iField; // indica o inidice o campo que está sendo lido, seus valores vão de 0 a n
    Registro reg; // armazena um registro lido
    
    // incializando variáveis
    // atribui null aos campos variáveis para fazer realloc deles
    nullFields(&reg);
    field = 0;
    iField = 0;
    
    // iterando sobre o arquivo
    while(c != EOF){    	

        // le um caracter do arquivo
        c = fgetc(fileIn); 
        
        // caracter de nova linha, deve ser ignorado na leitura
        if (c == '\r')
        	continue;

        // chegou em um novo registro
        if (c == '\n'){

        	//verifica se todos os campos fixos tem o tamanho correto
        	checkSizeFixedFields(&reg);

        	// grava o registro anterior         
	        saveReg(&reg, fileOut);

	        // zera indicador do campo que será lido
            field = 0;
            // zera indice do campo que será lido
            iField = 0;
            //Prepara os campo para serem realocados
			free(reg.razSoc);
			free(reg.nomeFant);
			free(reg.motCanc);
			free(reg.nomeEmp);
            nullFields(&reg);
            continue;
        }

        // chegou em um novo campo
        if (c == ';'){
            // incrementa qual campo o arquivo está lendo
            field++;
            // zera o indice do campo que será lido
            iField = 0;
            continue;
        }

        // adiciona o carácter lido no registro
        addCharField(&reg, c, field, iField++);
    }
}

/*	Descrição:
		Função que verifica se os campos fixos tem o tamanho correto 
	Parêmetros:
		reg = Registro a verificar o tamanho dos campos fixos*/
void checkSizeFixedFields(Registro *reg){
	int size,i;

	//verifica o tamanho do campo cnpj
	if( strlen(reg->cnpj) < 18 ){
		size = strlen(reg->cnpj);
		for(i=size;i<18;i++){
			reg->cnpj[i] = '0'; //completa o campo com 0s
		}
		reg->cnpj[i] = '\0';
	}

	//verifica o tamanho do campo data de registro
	if( strlen(reg->dtReg) < 8){
		size = strlen(reg->dtReg);
		for(i=size;i<8;i++){
			reg->dtReg[i] = '0'; //completa o campo com 0s
		}
		reg->dtReg[i] = '\0';
	}

	//verifica o tamanho do campo data de cancelamento
	if( strlen(reg->dtCanc) < 8){
		size = strlen(reg->dtCanc);
		for(i=size;i<8;i++){
			reg->dtCanc[i] = '0'; //completa o campo com 0s
		}
		reg->dtCanc[i] = '\0';
	}

	//verifica o tamanho do campo cnpj auditor
	if( strlen(reg->cnpjAud) < 18){
		size = strlen(reg->cnpjAud);
		for(i=size;i<18;i++){
			reg->cnpjAud[i] = '0'; //completa o campo com 0s
		}
		reg->cnpjAud[i] = '\0';
	}
}	

/*	Descrição:
		Função que grava um registro no arquivo de acordo com a organização do mesmo 
	Parêmetros:
		reg = Registro que será gravado
		file = Arquvio cujo registro será gravado */
void saveReg(Registro *reg, FILE *file){
	int sizeReg; // tamanho do registro que será gravado
	char delReg = DEL_REG; // delimitador de registro

	//organização por indicador de tamnho
	if (org == 'I'){
		// capturando tamanho do registro
		sizeReg  = 4; // delimitadores de campo
		sizeReg += strlen(reg->cnpj);
		sizeReg += strlen(reg->razSoc);
		sizeReg += strlen(reg->nomeFant);
		sizeReg += strlen(reg->dtReg);
		sizeReg += strlen(reg->dtCanc);
		sizeReg += strlen(reg->motCanc);
		sizeReg += strlen(reg->nomeEmp);
		sizeReg += strlen(reg->cnpjAud);
		
		// grava o tamnho do registro
		fwrite(&sizeReg, sizeof(int), 1, file);
		// grava todos os campos
		saveField(reg, file);
	}
	// organização por delimitador entre registros
	else if (org == 'D'){
		// grava todos os campos
		saveField(reg, file);
		// grava o delimitador de registros 
		fwrite(&delReg, sizeof(char), 1, file);		
	}
	// organização por número fixo de campos
	else 
		// grava todos os campos
		saveField(reg, file);
}

/*	Descrição:
		Dado um registro a função grava todos os seus campos no arquivo
	Parêmetros:
		reg = Registro que terá seus campos gravados
		file = Arquvio cujos campos serão gravados */
void saveField(Registro *reg, FILE *file){
	int sizeField; // tamanho da string que será salva
	char delField = DEL_FIELD; // delimitador de campo

	// CNPJ
	sizeField = strlen(reg->cnpj);				  	  // calcula tamanho da string
	fwrite(reg->cnpj, sizeof(char), sizeField, file); // frwite string
	fwrite(&delField, sizeof(char), 1, file);

	// Nome Social
	sizeField = strlen(reg->razSoc);
	fwrite(reg->razSoc, sizeof(char), sizeField, file);
	fwrite(&delField, sizeof(char), 1, file);
	
	// Nome Fantasia
	sizeField = strlen(reg->nomeFant);
	fwrite(reg->nomeFant, sizeof(char), sizeField, file);
	fwrite(&delField, sizeof(char), 1, file);

	// Data do Registro
	sizeField = strlen(reg->dtReg);
	fwrite(reg->dtReg, sizeof(char), sizeField, file);
	fwrite(&delField, sizeof(char), 1, file);

	// Data do Cancelamento
	sizeField = strlen(reg->dtCanc);
	fwrite(reg->dtCanc, sizeof(char), sizeField, file);
	fwrite(&delField, sizeof(char), 1, file);

	// Motivo do Cancelamento
	sizeField = strlen(reg->motCanc);
	fwrite(reg->motCanc, sizeof(char), sizeField, file);
	fwrite(&delField, sizeof(char), 1, file);

	// Nome da Empresa de Auditoria
	sizeField = strlen(reg->nomeEmp);
	fwrite(reg->nomeEmp, sizeof(char), sizeField, file);
	fwrite(&delField, sizeof(char), 1, file);

	// CNPJ da Empresa de Auditoria 
	sizeField = strlen(reg->cnpjAud);
	fwrite(reg->cnpjAud, sizeof(char), sizeField, file);
	fwrite(&delField, sizeof(char), 1, file);
}
//-----------------------------------------------------//



//***************************************//
//* FUNÇÕES PARA LISTAGEM DOS REGISTROS *//
//***************************************//

/*	Descrição:
		Lista todos os registros do arquivo, independente da sua organização
	Parêmetros:
		file = Arquivo que terá seus registros listados */
void listBin(FILE *file){

	if (org == 'I')
		listBin_I(file);

	else if (org == 'D')
		listBin_D(file);

	else {
		listBin_N(file);
	}
}

/*	Descrição:
		Lista todos os registros de um arquivo, desde que os registros estejam organizados por indicadores de tamanho 
	Parêmetros:
		file = Arquivo que terá seus registros listados */
void listBin_I(FILE *file){
	Registro reg; // registro que será listado
	int sizeReg; // tamanho do registro
	int i; // itera sobre o registro
	int field = 0; // campo que está sendo lido (0 a 7)
	int iField = 0; // indice do campo que está sendo  lido
	char c; // carácter que irá percorrer todo o arquivo
	char string[18]; //string que irá armazena os tamanhos fixos

	// iterenado sobre o arquivo, a cada repetição pega o tamanho de um campo
	while (fread(&sizeReg, sizeof(int), 1, file) == 1) {

		// Preparando variáveis para um novo registro
		field = 0;
		iField = 0;
		nullFields(&reg);

		// itera sobre o registro
		for(i=0; i < sizeReg; i++){

			//Lendo campo fixo de CNPJ
			if(field == 0 || field == 7){
				//lendo campo fixo CNPJ
				fread(&string,sizeof(char),18,file);
				//adiciona a string do CNPJ lida na struct
				addStringFieldCNPJ(&reg, string,field++);
				i += 18;
			}

			//Lendo campo fixo de Data
			if(field == 3 || field == 4){
				//lendo campo fixo Datas
				fread(&string,sizeof(char),8,file);
				//adiciona a string da data lida na struct
				addStringFieldDate(&reg,string,field++);
				i += 8;
			}

			//Lendo campo variaveis
			if(field == 1 || field == 2 || field == 5 || field == 6){
				// le carácter
				fread(&c, sizeof(char), 1, file);

				// campo novo
				if (c == ';'){
					field++;
					iField = 0;
					continue;
				}

				// adiciona char ao registro
				addCharField(&reg, c, field, iField++);
			}
		}

		// exibe registro recuperado do arquivo
		printReg(&reg);
		free(reg.razSoc);
		free(reg.nomeFant);
		free(reg.motCanc);
		free(reg.nomeEmp);
	}
}

/*	Descrição:
		Lista todos os registros de um arquivo, desde que os registros estejam organizados por delimitadores 
	Parêmetros:
		file = Arquivo que terá seus registros listados */
void listBin_D(FILE *file){
	Registro reg; // registro que será listado
	int field; // campo que está sendo lido (0 a 7)
	int iField;	// indice do campo que está sendo lido
	char c;	// carácter que irá percorrer todo o arquivo


	// inicializando variáveis
	field = 0;
	iField = 0;
	nullFields(&reg);

	while (fread(&c, sizeof(char), 1, file) == 1){
		// registro novo
		if (c == DEL_REG){
			field = 0;
			iField = 0;

			printReg(&reg);
			free(reg.razSoc);
			free(reg.nomeFant);
			free(reg.motCanc);
			free(reg.nomeEmp);
			nullFields(&reg);
			continue;
		}

		// campo novo
		if (c == DEL_FIELD){
			field++;
			iField = 0;
			continue;
		}

		// adiciona char ao campo
		addCharField(&reg, c, field, iField++);
	}
}

/*	Descrição:
		Lista todos os registros de um arquivo, desde que os registros estejam organizados por número fixo de campos 
	Parêmetros:
		file = Arquivo que terá seus registros listados */
void listBin_N(FILE *file){
	Registro reg; // registro que será listado
	int field; // campo que está sendo lido (0 a 7)
	int iField;	// indice do campo que está sendo lido
	char c; // carácter que irá percorrer todo o arquivo

	// inicializando variáveis
	field = 0;
	iField = 0;
	nullFields(&reg);

	// percorre todo o arquivo
	while (fread(&c, sizeof(char), 1, file) == 1){	
		
		if (c == DEL_FIELD){

			// campo novo
			if (field < 7){
				field++;
				iField = 0;
			}
			// registro novo
			else {
				field = 0;
				iField = 0;

				printReg(&reg);
				// prepara reg para receber um novo registro
				free(reg.razSoc);
				free(reg.nomeFant);
				free(reg.motCanc);
				free(reg.nomeEmp);
				nullFields(&reg);
			}
			continue;
		}

		// adiciona char ao campo
		addCharField(&reg, c, field, iField++);
		
	}
}
//---------------------------------------//



//********************//
//* FUNÇÕES DE BUSCA *//
//********************//

/*	Descrição:
		Faz uma busca no arquivo, independente da sua organização 
		e de acordo com o campo e o valor informado 
	Parêmetros:
		file = Arquivo que a busca será realizada
		fieldBusca = Campo que a busca irá comparar com o valor informado
		strBusca = Valor (string) de comparação da busca 
	Retorno:
		Retorna o registro caso ele seja encontrado, caso contrário retorna NULL */
Registro* buscaCampo(FILE *file, int fieldBusca, char *strBusca){
	if (org == 'I')
		return buscaCampo_I(file, fieldBusca, strBusca);
	else if (org == 'D')
		return buscaCampo_D(file, fieldBusca, strBusca);
	else
		return buscaCampo_N(file, fieldBusca, strBusca);
}

/*	Descrição:
		Faz uma busca no arquivo de acordo com o campo e valor informado, 
		necessáriamente os registros do arquivo devem ser organizados por indicadores de tamanho 
	Parêmetros:
		file = Arquivo que a busca será realizada
		fieldBusca = Campo que a busca irá comparar com o valor informado
		strBusca = Valor (string) de comparação da busca 
	Retorno:
		Retorna o registro caso ele seja encontrado, caso contrário retorna NULL */
Registro* buscaCampo_I(FILE *file, int fieldBusca, char *strBusca){
	int size; // tamanho do registro
	Registro *reg; // registro que será retornado

	// percorrendo cada registro do arquivo
	while (fread(&size, sizeof(int), 1, file) == 1){
		reg = getReg(file);

		// encontrou
		if (compareField(reg, fieldBusca, strBusca) == 0)
			return reg;
	}

	// não ecnotrou
	free(reg->razSoc);
	free(reg->nomeFant);
	free(reg->motCanc);
	free(reg->nomeEmp);
	free(reg);
	return NULL;
}

/*	Descrição:
		Faz uma busca no arquivo de acordo com o campo e valor informado, 
		necessáriamente os registros do arquivo devem ser organizados por um delimitador entre eles 
	Parêmetros:
		file = Arquivo que a busca será realizada
		fieldBusca = Campo que a busca irá comparar com o valor informado
		strBusca = Valor (string) de comparação da busca 
	Retorno:
		Retorna o registro caso ele seja encontrado, caso contrário retorna NULL */
Registro* buscaCampo_D(FILE *file, int fieldBusca, char *strBusca){
	char c; // carácter que irá percorrer todo o arquivo
	Registro *reg; // registro que será retornado
	int field = 0; // campo que está sendo lido (0 a 7) 
	int iField = 0; // indice do campo que está sendo lido

	// aloca e prepara o registro
	reg = (Registro *) malloc(sizeof(Registro));
	nullFields(reg);

	// percorre arquivo char a char 
	while(fread(&c, sizeof(char), 1, file) == 1){
		
		// troca de campo
		if (c == DEL_FIELD){
			field++;
			iField = 0;
			continue;
		}
		
		if (c == DEL_REG){
			// encontrou 
			if (compareField(reg, fieldBusca, strBusca) == 0)
				return reg;

			// troca de registro
			field = 0;
			iField = 0;
			continue;
		}

		// adiciona o carácter lido no registro 
		addCharField(reg, c, field, iField++);
	}

	// não encontrou
	free(reg->razSoc);
	free(reg->nomeFant);
	free(reg->motCanc);
	free(reg->nomeEmp);
	free(reg);
	return NULL;
}

/*	Descrição:
		Faz uma busca no arquivo de acordo com o campo e valor informado, 
		necessáriamente os registros do arquivo devem ser organizados por número fixo de campos 
	Parêmetros:
		file = Arquivo que a busca será realizada
		fieldBusca = Campo que a busca irá comparar com o valor informado
		strBusca = Valor (string) de comparação da busca 
	Retorno:
		Retorna o registro caso ele seja encontrado, caso contrário retorna NULL */
Registro* buscaCampo_N(FILE *file, int fieldBusca, char *strBusca){
	char c; // carácter que irá percorrer todo o arquivo
	Registro *reg; // registro que será retornado
	int field = 0; // campo que está sendo lido (0 a 7) 
	int iField = 0; // indice do campo que está sendo lido

	// aloca e prepara o registro
	reg = (Registro *) malloc(sizeof(Registro));
	nullFields(reg);
	
	// percorre arquivo char a char 
	while(fread(&c, sizeof(char), 1, file) == 1){
		
		if (c == DEL_FIELD){

			// troca de registro
			if (field == 7){

				// encontrou
				if (compareField(reg, fieldBusca, strBusca) == 0)
					return reg;

				field = 0;
				iField = 0;
			}
			// troca de campo
			else {
				field++;
				iField = 0;
			}
			continue;
		}
		// adiciona o carácter lido no registro 
		addCharField(reg, c, field, iField++);
	}
	
	// não encontrou
	free(reg->razSoc);
	free(reg->nomeFant);
	free(reg->motCanc);
	free(reg->nomeEmp);
	free(reg);
	return NULL;
}

/*	Descrição:
		Faz uma busca no arquivo, independente da sua organização 
		e de acordo com o campo e o valor informado 
	Parêmetros:
		file = Arquivo que a busca será realizada
		fieldBusca = Campo que a busca irá comparar com o valor informado
		strBusca = Valor (string) de comparação da busca 
	Retorno:
		Retorna o campo caso ele seja encontrado, caso contrário retorna NULL */
Registro* buscaReg(FILE *file, int regBusca){
	if (org == 'I')
		return buscaReg_I(file, regBusca);
	else if (org == 'D')
		return buscaReg_D(file, regBusca);
	else
		return buscaReg_N(file, regBusca);
}

/*	Descrição:
		Faz uma busca no arquivo de acordo com o campo e valor informado, 
		necessáriamente os registros do arquivo devem ser organizados por indicadores de tamanho 
	Parêmetros:
		file = Arquivo que a busca será realizada
		fieldBusca = Campo que a busca irá comparar com o valor informado
		strBusca = Valor (string) de comparação da busca 
	Retorno:
		Retorna o campo caso ele seja encontrado, caso contrário retorna NULL */
Registro* buscaReg_I(FILE *file, int regBusca){
	int size; // tamanho do registro
	int countReg = 1; //conta quantos registros foram lidos

	// percorre cada registro do arquivo
	while (fread(&size, sizeof(int), 1, file) == 1)
	
		// pula para próximo registro
		if (countReg++ < regBusca)
			fseek(file, size, SEEK_CUR);

		// registro encontrado
		else
			return getReg(file); 

	// não ecnotrou
	return NULL;
}

/*	Descrição:
		Faz uma busca no arquivo de acordo com o campo e valor informado, 
		necessáriamente os registros do arquivo devem ser organizados por um delimitador entre eles 
	Parêmetros:
		file = Arquivo que a busca será realizada
		fieldBusca = Campo que a busca irá comparar com o valor informado
		strBusca = Valor (string) de comparação da busca 
	Retorno:
		Retorna o campo caso ele seja encontrado, caso contrário retorna NULL */
Registro* buscaReg_D(FILE *file, int regBusca){
	char c; // carácter que irá percorrer todo o arquivo
	Registro *reg; // registro que será retornado
	int field = 0; // campo que está sendo lido (0 a 7) 
	int iField = 0; // indice do campo que está sendo lido
	int countReg = 1; //conta quantos registros foram lidos

	// aloca e prepara o registro
	reg = (Registro *) malloc(sizeof(Registro));
	nullFields(reg);
	
	// percorre o arquivo char a char
	while(fread(&c, sizeof(char), 1, file) == 1){
		
		// trocou de campo
		if (c == DEL_FIELD){
			field++;
			iField = 0;
			continue;
		}

		// trocou de registro
		if (c == DEL_REG){
			// encontrou
			if (countReg++ == regBusca)
				return reg;

			field = 0;
			iField = 0;
			continue;
		}

		// adiciona char lido ao registro
		addCharField(reg, c, field, iField++);
	}

	// não encontrou
	free(reg->razSoc);
	free(reg->nomeFant);
	free(reg->motCanc);
	free(reg->nomeEmp);
	free(reg);
	return NULL;
}

/*	Descrição:
		Faz uma busca no arquivo de acordo com o campo e valor informado, 
		necessáriamente os registros do arquivo devem ser organizados por número fixo de campos 
	Parêmetros:
		file = Arquivo que a busca será realizada
		fieldBusca = Campo que a busca irá comparar com o valor informado
		strBusca = Valor (string) de comparação da busca 
	Retorno:
		Retorna o campo caso ele seja encontrado, caso contrário retorna NULL */
Registro* buscaReg_N(FILE *file, int regBusca){
	char c; // carácter que irá percorrer todo o arquivo
	Registro *reg; // registro que será retornado
	int field = 0; // campo que está sendo lido (0 a 7) 
	int iField = 0; // indice do campo que está sendo lido
	int countReg = 1; //conta quantos registros foram lidos

	// aloca e prepara o registro
	reg = (Registro *) malloc(sizeof(Registro));
	nullFields(reg);
	
	// percorre o arquivo char a char
	while(fread(&c, sizeof(char), 1, file) == 1){
		
		if (c == DEL_FIELD){
			
			// troca de registro
			if (field == 7){
				// encontrou
				if (countReg++ == regBusca)
					return reg;

				field = 0;
				iField = 0;
			}
			// troca de campo
			else{
				field++;
				iField = 0;
			}
			
			continue;
		}

		// adiciona char lido ao registro
		addCharField(reg, c, field, iField++);
	}

	// não encontrou
	free(reg->razSoc);
	free(reg->nomeFant);
	free(reg->motCanc);
	free(reg->nomeEmp);
	free(reg);
	return NULL;
}
//--------------------//



//*******************************// 
//* FUNÇÕES ÚTEIS (SUB FUNÇÕES) *//
//*******************************//

/*	Descrição:
		Função utilizada para preencher os campos variáveis, sua função é inserir um char em um campo, 
		dado o registro, o campo e sua posição no campo  
	Parêmetros:
		reg = Registro que terá o campo com o char adicionado
		c = Char que será adicionado
		field = Indica qual campo terá o char adicionado (seus valores vão de 0 a 7)
		iField = Indice que indica em qual posição do campo o char será inserido */
void addCharField(Registro *reg, char c, int field, int iField){
    switch(field){
        // CNPJ
        case 0:
            reg->cnpj[iField] = c;
            reg->cnpj[++iField] = '\0';
        break;

        // Nome Social
        case 1:
            reg->razSoc = (char*)realloc(reg->razSoc, sizeof(char) * iField+2);
            reg->razSoc[iField] = c;
            reg->razSoc[++iField] = '\0';
        break;

        // Nome Fantasia
        case 2:
            reg->nomeFant = (char*)realloc(reg->nomeFant, sizeof(char) * iField+2);
            reg->nomeFant[iField] = c;
            reg->nomeFant[++iField] = '\0';
        break;

        // Data do Registro
        case 3:
            reg->dtReg[iField++] = c;
            reg->dtReg[iField] = '\0';
        break;

        // Data do Registro
        case 4:
            reg->dtCanc[iField++] = c;
            reg->dtCanc[iField] = '\0';
        break;

        // Motivo do Cancelamento
        case 5:
            reg->motCanc = (char*)realloc(reg->motCanc, sizeof(char) * iField+2);
            reg->motCanc[iField] = c;
            reg->motCanc[++iField] = '\0';                    
        break;

        // Nome da Empresa de Auditoria
        case 6:
            reg->nomeEmp = (char*)realloc(reg->nomeEmp, sizeof(char) * iField+2);
            reg->nomeEmp[iField] = c;
            reg->nomeEmp[++iField] = '\0';                    
        break;

        // CNPJ da Empresa de Auditoria 
        case 7:
            reg->cnpjAud[iField++] = c;
            reg->cnpjAud[iField] = '\0';                 
        break;
    }
}

/*	Descrição:
		Função utilizada para preencher os campos fixoes de CNPJ  
	Parêmetros:
		reg = Registro que terá o campo com o char adicionado
		c = Char que será adicionado
		field = Indica qual campo terá o char adicionado (seus valores vão de 0 a 7)
		iField = Indice que indica em qual posição do campo o char será inserido */
void addStringFieldCNPJ(Registro *reg, char c[18], int field){
	int i;
	switch(field){
		case 0:
			for(i=0;i<18;i++){
				reg->cnpj[i] = c[i];
			}
			reg->cnpj[i] = '\0';
		break;

		case 7:
			for(i=0;i<18;i++){
				reg->cnpjAud[i] = c[i];
			}
			reg->cnpjAud[i] = '\0';
		break;
	}
}

/*	Descrição:
		Função utilizada para preencher os campos fixoes referentes a data  
	Parêmetros:
		reg = Registro que terá o campo com o char adicionado
		c = Char que será adicionado
		field = Indica qual campo terá o char adicionado (seus valores vão de 0 a 7)
		iField = Indice que indica em qual posição do campo o char será inserido */
void addStringFieldDate(Registro *reg, char c[18], int field){
	int i;
	switch(field){
		case 3:
			for(i=0;i<8;i++){
				reg->dtReg[i] = c[i];
			}
			reg->dtReg[i] = '\0';
		break;

		case 4:
			for(i=0;i<8;i++){
				reg->dtCanc[i] = c[i];
			}
			reg->dtCanc[i] = '\0';
		break;
	}
}

/*	Descrição:
		Dado um registro esta função atribui NULL aos seus campos de tamanho variável.
		É utilizada quando um registro é recém criado e seus campos variáveis irão passar por um realloc
	Parêmetros:
		reg = Registro que irá receber null em seus campos variáveis */
void nullFields(Registro *reg){
    reg->razSoc  = NULL;
    reg->nomeFant = NULL;
    reg->motCanc  = NULL;
    reg->nomeEmp  = NULL;
}

/*	Descrição:
		Exibe um registro na saída padrão. 
	Parêmetros:
		reg = Registro que será exibido na saída padrão */
void printReg(Registro *reg){

	printf("CNPJ.........: ");
	if(reg->cnpj[0] == 'n'){
		printf("null\n");
	}
	else{
		printf("%s\n",reg->cnpj);
	}

	printf("Razão Social.: %s\n",   reg->razSoc);
	printf("Nome Fantasia: %s\n",   reg->nomeFant);
	
	printf("Data Registro: ");
	if(reg->dtReg[0] == 'n'){
		printf("null\n");
	}
	else{
		printf("%s\n", reg->dtReg);
	}

	printf("Data Canc....: ");
	if(reg->dtCanc[0] == 'n'){
		printf("null\n");
	}
	else{
		printf("%s\n", reg->dtCanc);
	}
	
	printf("Motivo Canc..: %s\n",   reg->motCanc);
	printf("Empresa Aud..: %s\n",   reg->nomeEmp);

	printf("CNPJ Emp Aud.: ");
	if(reg->cnpjAud[0] == 'n'){
		printf("null\n\n");
	}
	else{
		printf("%s\n\n", reg->cnpjAud);
	}
} 

/*	Descrição:
		Função que compara o campo de um registro com um valor informado
	Parêmetros:
		reg = Registro cujo campo irá sofrer a comparação
		field = Indica qual campo irá sofrer a comparação (seus valores vão de 1 a 8)
		strBusca = Valor (string) que será comparado com o campo do registro dado
	Retorno:
		Retorna o resultado da comparação, 0 para valores iguais e diferente de 0 caso contrário */
int compareField(Registro *reg, int field, char *strBusca){
	int cmp = -1; // armazena o resultado da comparação

	switch(field) {
		case 1:
			if (reg->cnpj != NULL)
				cmp = strcmp(reg->cnpj, strBusca);
		break;

		case 2:
			if (reg->razSoc != NULL)
				cmp = strcmp(reg->razSoc, strBusca);
		break;			

		case 3:
			if (reg->nomeFant != NULL)
				cmp = strcmp(reg->nomeFant, strBusca);
		break;

		case 4:
			if (reg->dtReg != NULL)
				cmp = strcmp(reg->dtReg, strBusca);
		break;

		case 5:
			if (reg->dtCanc != NULL)
				cmp = strcmp(reg->dtCanc, strBusca);
		break;	

		case 6:
			if (reg->motCanc != NULL)
				cmp = strcmp(reg->motCanc, strBusca);
		break;

		case 7:
			if (reg->nomeEmp != NULL)
				cmp = strcmp(reg->nomeEmp, strBusca);
		break;			

		case 8:
			if (reg->cnpjAud != NULL)
				cmp = strcmp(reg->cnpjAud, strBusca);
		break;	
	}

	return(cmp);
}

/*	Descrição:
		Dado a posição de um registro no arquivo, a função procura o campo informado e retorna seu valor
	Parêmetros:
		file = Arquivo cuja busca será realizada
		regBusca = Localização do registro no arquivo (seus valores vão de 1 a n, onde n é o número máximo 
		de registros no arquivo)
		fieldBusca = Indica qual campo será retornado (seus valores vão de 1 a 8)
	Retorno:
		Retorna o valor correspondente ao campo desejado, caso o registro não seja encontrado retorna NULL */
char* buscaRegCampo(FILE *file, int regBusca, int fieldBusca){
	Registro *reg; // registro buscado
	char *retorno = NULL; // aramzena o valor do campo buscado

	// busca o registro
	reg = buscaReg(file, regBusca);

	if (reg == NULL)
		return NULL;
	else 
		switch(fieldBusca){
			case 1:
				if (reg->cnpj != NULL){
					// aloca string que ira receber o campo
					retorno = (char *) malloc(sizeof(char) * strlen(reg->cnpj)+1);
					// copia campo para a sting de retorno
					memcpy(retorno, reg->cnpj, strlen(reg->cnpj)+1);				
				}
			break;
			
			case 2:
				if (reg->razSoc != NULL){
					retorno = (char *) malloc(sizeof(char) * strlen(reg->razSoc)+1);
					memcpy(retorno, reg->razSoc, strlen(reg->razSoc)+1);
				}
			break;

			case 3:
				if (reg->nomeFant != NULL){
					retorno = (char *) malloc(sizeof(char) * strlen(reg->nomeFant)+1);
					memcpy(retorno, reg->nomeFant, strlen(reg->nomeFant)+1);
				}
			break;

			case 4:
				if (reg->dtReg != NULL){
					retorno = (char *) malloc(sizeof(char) * strlen(reg->dtReg)+1);
					memcpy(retorno, reg->dtReg, strlen(reg->dtReg)+1);
				}
			break;

			case 5:
				if (reg->dtCanc != NULL){
					retorno = (char *) malloc(sizeof(char) * strlen(reg->dtCanc)+1);
					memcpy(retorno, reg->dtCanc, strlen(reg->dtCanc)+1);
				}
			break;

			case 6:
				if (reg->motCanc != NULL){
					retorno = (char *) malloc(sizeof(char) * strlen(reg->motCanc)+1);
					memcpy(retorno, reg->motCanc, strlen(reg->motCanc)+1);
				}
			break;

			case 7:
				if (reg->nomeEmp != NULL){
					retorno = (char *) malloc(sizeof(char) * strlen(reg->nomeEmp)+1);
					memcpy(retorno, reg->nomeEmp, strlen(reg->nomeEmp)+1);
				}
			break;

			case 8:
				if (reg->cnpjAud != NULL){
					retorno = (char *) malloc(sizeof(char) * strlen(reg->cnpjAud)+1);
					memcpy(retorno, reg->cnpjAud, strlen(reg->cnpjAud)+1);
				}
			break;
		}


	free(reg->razSoc);
	free(reg->nomeFant);
	free(reg->motCanc);
	free(reg->nomeEmp);
	free(reg);
	return retorno;
}

/*	Descrição:
		Dado um arquivo com seu ponteiro no início de um registro, o registro é retornado e o ponteiro do arquivo 
		deslocado para uma posição após o registro
	Parêmetros:
		file = Arquivo cujo registro será retornado
	Retorno:
		Retorna o registro capturado do arquivo */
Registro* getReg(FILE *file){
	Registro *reg; // registro que será retornado
	char *field; //campo genérico, usado para preencher o campo dos registros
	int i; // itera sobre os campos

	// alocando registro que será retornado
	reg = (Registro *) malloc(sizeof(Registro));

	// CNPJ
	fread(&reg->cnpj,sizeof(char),18,file);
	reg->cnpj[18] = '\0';

	
	// Razao Social
	reg->razSoc = getField(file);
	
	// Nome Fantasia
	reg->nomeFant = getField(file);
	
	// Data do registro
	fread(&reg->dtReg,sizeof(char),8,file);
	reg->dtReg[8] = '\0';

	// Data do Cancelamento
	fread(&reg->dtCanc,sizeof(char),8,file);
	reg->dtCanc[8] = '\0';

	// Motivo do Cancelamento
	reg->motCanc = getField(file);
	
	// Nome da Empresa de Auditoria
	reg->nomeEmp = getField(file);

	// CNPJ da Empresa de Auditoria
	fread(&reg->cnpjAud,sizeof(char),18,file);
	reg->cnpjAud[18] = '\0';

	return(reg);
}

/*	Descrição:
		Dado um arquivo com seu ponteiro no início de um campo, o campo é retornado e o ponteiro do arquivo 
		deslocado para uma posição após o delimitador do campo, ou seja, no inicio do campo/registro sequente
	Parêmetros:
		file = Arquivo cujo campo será retornado
	Retorno:
		Retorna o campo capturado do arquivo */
char* getField(FILE *file){
	char c; // carácter usado para percorrer o campo
	int i = 0; //usado para iterar sobre o campo
	char *field = NULL; // campo que será retornado

	// le um caracter
	fread(&c, sizeof(char), 1, file);

	// percorre até o delimitador de campo
	while (c != DEL_FIELD){
		// adiciona o char ao campo
		field = (char*)realloc(field, sizeof(char) * (i+2));
        field[i] = c;
        field[++i] = '\0';
        // le um novo char
        fread(&c, sizeof(char), 1, file);
	}
	
	return field;
}
//----------------------------//