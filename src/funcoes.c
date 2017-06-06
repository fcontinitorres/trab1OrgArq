/*
GRUPO G5
Felipe Scrochio Custodio, 9442688
Felipe Contini Torres, 9253670
Júlia Diniz Ferreira, 9364865
Bruno Henrique Rasteiro, 9292910
*/

#include "../src/funcoes.h"

//*******************//
//* FUNÇÕES DO MAIN *//
//*******************//

/*	Descrição:
		Abre os arquivos de entrada e saída do programa
	Parâmetros:
		arquivoEntrada = arquivo de entrada
		arquivoSaida = arquivo de saída
	Retorno:
		Retorna um int que indica o sucesso da função. Os possíves valores são:
		1 -> Arquivos aberto com sucesso
		2 -> Falha ao abrir arquivos */
int validaArquivos(FILE **arquivoEntrada, FILE **arquivoSaida) {

	// abrindo arquivos
    *arquivoEntrada  = fopen(ARQUIVO_ENTRADA, "r");
    *arquivoSaida = fopen(ARQUIVO_SAIDA, "wb+");

    if (*arquivoEntrada == NULL) {
    	printf("Erro ao abrir: %s\n", ARQUIVO_ENTRADA);
    	return(0);
    }

    if (*arquivoSaida == NULL) {
    	printf("Erro ao abrir: %s\n", ARQUIVO_SAIDA);
    	return(0);
    }

    return(1);
}

/*	Descrição:
		Realiza as tarefas para executar a primeira opção do menu
	Parâmetros:
		arquivo = arquivo de saida (binário) */
void opcao1(FILE *arquivo) {
	// lista todos os dados
	listarBinario(arquivo);

	// reseta ponteiro do arquivo
	fseek(arquivo, 0, SEEK_SET);
}

/*	Descrição:
		Realiza as tarefas para executar a segunda opção do menu
	Parâmetros:
		arquivo = arquivo de saida (binário) */
void opcao2(FILE *arquivo) {
	int campo; // campo que a busca será realizado
	char *strBusca; // valor que a busca irá usar para comparar
	char *aux;
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
	scanf("%d", &campo);
	getchar();

	// valida campo de busca
	if (campo < 1 || campo > 8) {
		printf("\nOpcao invalida.\n");
		return;
	}

	// espera o valor da busca
	printf("Informe o valor a ser buscado (substitua ' ' por ';'): ");
	strBusca = NULL;
	scanf("%ms", &strBusca);
	// converte string informada trocando ';' por ' '
	aux = strBusca;
	strBusca = converterString(strBusca);
	printf("\n");
	free(aux);

	// busca
	reg = buscaCampo(arquivo, campo, strBusca);

	if (reg != NULL) {
		printRegistro(reg);
		free(reg->razaoSocial);
		free(reg->nomeFantasia);
		free(reg->motivoCancelamento);
		free(reg->nomeEmpresa);
	}
	else
		printf("Não foi possível localizar o registro.\n");

	// reseta ponteiro do arquivo
	fseek(arquivo, 0, SEEK_SET);
	free(reg);
	free(strBusca);
}

/*	Descrição:
		Realiza as tarefas para executar a terceira opção do menu
	Parâmetros:
		arquivo = arquivo de saida (binário) */
void opcao3(FILE *arquivo) {
	int regBusca; // localização do registro que será buscado
	Registro *reg; // resultado da busca

	// espera localização do registro
	printf("Informe o numero do registro desejado (1...n): ");
	scanf("%d", &regBusca);
	getchar();
	printf("\n");

	// valida a localização do registro
	if (regBusca < 1) {
		printf("\nOpcao invalida.\n");
		return;
	}

	// busca
	reg = buscaReg(arquivo, regBusca);

	if (reg != NULL) {
		printRegistro(reg);
		free(reg->razaoSocial);
		free(reg->nomeFantasia);
		free(reg->motivoCancelamento);
		free(reg->nomeEmpresa);
	}
	else
		printf("Não foi possível localizar o registro.\n");

	// reseta ponteiro do arquivo
	fseek(arquivo, 0, SEEK_SET);
	free(reg);
}

/*	Descrição:
		Realiza as tarefas para executar a quarta opção do menu
	Parâmetros:
		arquivo = arquivo de saida (binário) */
void opcao4(FILE *arquivo) {
	int regBusca; // localização do registro que será buscado
	int campoBusca; // indica qual campo sera buscado e retornado
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
	scanf("%d", &campoBusca);
	getchar();

	// valida campo de busca
	if (campoBusca < 1 || campoBusca > 8) {
		printf("\nOpcao invalida.\n");
		return;
	}

	// espera localização do registro
	printf("Informe o numero do registro desejado (1...n): ");
	scanf("%d", &regBusca);
	getchar();

	// valida a localização do registro
	if (regBusca < 1) {
		printf("\nOpcao invalida.\n");
		return;
	}

	// busca
	result = buscaRegCampo(arquivo, regBusca, campoBusca);

	if (result != NULL) {
		printf("\n%s\n", result);
		free(result);
	}else
		printf("\nRegistro não encontrado\n");

	// reseta ponteiro do arquivo
	fseek(arquivo, 0, SEEK_SET);
}

/*	Descrição:
		Dada uma string a função substitui os caracteres ';' por ' '
	Parâmetros:
		str = string que será convertida
	Retorno:
		Retorna a string convertida, caso a string dada seja NULL o retorno também será */
char* converterString(char *str) {
	int i; // indice que será percorrido na string
	char *retorno = NULL; // string de retorno

	if (str != NULL) {
		// aloca strlen(str)+1 posicoes para incluir o '\0'
		retorno = (char *)malloc(strlen(str)+1);
		// percorre até o '\0' fazendo a substituição quando necessária
		for(i=0; i < strlen(str)+1; i++) {
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
	Parâmetros:
		arquivoEntrada = Arquivo de entrada
		arquivoSaida = Arquivo de saída */
void gerarBinarioCSV(FILE *arquivoEntrada, FILE *arquivoSaida) {
    char c = 'c'; // caracter que irá percorrer o arquivo, deve ser inicializado com um valor != de EOF
    char string[18];
    int campo;  // indica qual campo do arquivo está sendo lido, seus valores vão de 0 a 7
    int iCampo; // indica o inidice o campo que está sendo lido, seus valores vão de 0 a n
    Registro reg; // armazena um registro lido

    // incializando variáveis
    // atribui null aos campos variáveis para fazer realloc deles
    nullCampos(&reg);
    campo = 0;
    iCampo = 0;

    // iterando sobre o arquivo
    while(c != EOF) {

        // le um caracter do arquivo
        c = fgetc(arquivoEntrada);

        // caracter de nova linha, deve ser ignorado na leitura
        if (c == '\r')
        	continue;

        // chegou em um novo registro
        if (c == '\n') {

        	//verifica se todos os campos fixos tem o tamanho correto
        	checarCamposFixos(&reg);

        	// grava o registro anterior
	        salvarRegistro(&reg, arquivoSaida);

	        // zera indicador do campo que será lido
            campo = 0;
            // zera indice do campo que será lido
            iCampo = 0;
            //Prepara os campo para serem realocados
			free(reg.razaoSocial);
			free(reg.nomeFantasia);
			free(reg.motivoCancelamento);
			free(reg.nomeEmpresa);
            nullCampos(&reg);
            continue;
        }

        // chegou em um novo campo
        if (c == ';') {
            // incrementa qual campo o arquivo está lendo
            campo++;
            // zera o indice do campo que será lido
            iCampo = 0;
            continue;
        }

        // adiciona o carácter lido no registro
        adicionaCharCampo(&reg, c, campo, iCampo++);
    }
}

/*	Descrição:
		Função que verifica se os campos fixos tem o tamanho correto
	Parâmetros:
		reg = Registro a verificar o tamanho dos campos fixos*/
void checarCamposFixos(Registro *reg) {
	int size,i;

	//verifica o tamanho do campo cnpj
	if( strlen(reg->cnpj) < 18 ) {
		size = strlen(reg->cnpj);
		for(i=size;i<18;i++) {
			reg->cnpj[i] = '0'; //completa o campo com 0s
		}
		reg->cnpj[i] = '\0';
	}

	//verifica o tamanho do campo data de registro
	if( strlen(reg->dataRegistro) < 8) {
		size = strlen(reg->dataRegistro);
		for(i=size;i<8;i++) {
			reg->dataRegistro[i] = '0'; //completa o campo com 0s
		}
		reg->dataRegistro[i] = '\0';
	}

	//verifica o tamanho do campo data de cancelamento
	if( strlen(reg->dataCancelamento) < 8) {
		size = strlen(reg->dataCancelamento);
		for(i=size;i<8;i++) {
			reg->dataCancelamento[i] = '0'; //completa o campo com 0s
		}
		reg->dataCancelamento[i] = '\0';
	}

	//verifica o tamanho do campo cnpj auditor
	if( strlen(reg->cnpjAuditor) < 18) {
		size = strlen(reg->cnpjAuditor);
		for(i=size;i<18;i++) {
			reg->cnpjAuditor[i] = '0'; //completa o campo com 0s
		}
		reg->cnpjAuditor[i] = '\0';
	}
}

/*	Descrição:
		Função que grava um registro no arquivo de acordo com a organização do mesmo
	Parâmetros:
		reg = Registro que será gravado
		arquivo = Arquivo cujo registro será gravado */
void salvarRegistro(Registro *reg, FILE *arquivo) {
	int tamanhoRegistro; // tamanho do registro que será gravado
	char delReg = DELIM_REGISTRO; // delimitador de registro

	//organização por indicador de tamanho
	if (org == 'I') {
		// capturando tamanho do registro
		tamanhoRegistro  = 4; // delimitadores de campo
		tamanhoRegistro += strlen(reg->cnpj);
		tamanhoRegistro += strlen(reg->razaoSocial);
		tamanhoRegistro += strlen(reg->nomeFantasia);
		tamanhoRegistro += strlen(reg->dataRegistro);
		tamanhoRegistro += strlen(reg->dataCancelamento);
		tamanhoRegistro += strlen(reg->motivoCancelamento);
		tamanhoRegistro += strlen(reg->nomeEmpresa);
		tamanhoRegistro += strlen(reg->cnpjAuditor);

		// grava o tamanho do registro
		fwrite(&tamanhoRegistro, sizeof(int), 1, arquivo);
		// grava todos os campos
		salvarCampo(reg, arquivo);
	}
	// organização por delimitador entre registros
	else if (org == 'D') {

		// grava todos os campos
		salvarCampo(reg, arquivo);
		// grava o delimitador de registros
		fwrite(&delReg, sizeof(char), 1, arquivo);
	}
	// organização por número fixo de campos
	else
		// grava todos os campos
		salvarCampo(reg, arquivo);
}

/*	Descrição:
		Dado um registro a função grava todos os seus campos no arquivo
	Parâmetros:
		reg = Registro que terá seus campos gravados
		arquivo = Arquivo cujos campos serão gravados */
void salvarCampo(Registro *reg, FILE *arquivo) {
	int sizeCampo; // tamanho da string que será salva
	char delCampo = DELIM_CAMPO; // delimitador de campo

	// CNPJ
	sizeCampo = strlen(reg->cnpj);				  	  // calcula tamanho da string
	fwrite(reg->cnpj, sizeof(char), sizeCampo, arquivo); // frwite string
	if(org != 'I') fwrite(&delCampo, sizeof(char), 1, arquivo);

	// Nome Social
	sizeCampo = strlen(reg->razaoSocial);
	fwrite(reg->razaoSocial, sizeof(char), sizeCampo, arquivo);
	fwrite(&delCampo, sizeof(char), 1, arquivo);

	// Nome Fantasia
	sizeCampo = strlen(reg->nomeFantasia);
	fwrite(reg->nomeFantasia, sizeof(char), sizeCampo, arquivo);
	fwrite(&delCampo, sizeof(char), 1, arquivo);

	// Data do Registro
	sizeCampo = strlen(reg->dataRegistro);
	fwrite(reg->dataRegistro, sizeof(char), sizeCampo, arquivo);
	if(org != 'I') fwrite(&delCampo, sizeof(char), 1, arquivo);

	// Data do Cancelamento
	sizeCampo = strlen(reg->dataCancelamento);
	fwrite(reg->dataCancelamento, sizeof(char), sizeCampo, arquivo);
	if(org != 'I') fwrite(&delCampo, sizeof(char), 1, arquivo);

	// Motivo do Cancelamento
	sizeCampo = strlen(reg->motivoCancelamento);
	fwrite(reg->motivoCancelamento, sizeof(char), sizeCampo, arquivo);
	fwrite(&delCampo, sizeof(char), 1, arquivo);

	// Nome da Empresa de Auditoria
	sizeCampo = strlen(reg->nomeEmpresa);
	fwrite(reg->nomeEmpresa, sizeof(char), sizeCampo, arquivo);
	fwrite(&delCampo, sizeof(char), 1, arquivo);

	// CNPJ da Empresa de Auditoria
	sizeCampo = strlen(reg->cnpjAuditor);
	fwrite(reg->cnpjAuditor, sizeof(char), sizeCampo, arquivo);
	if(org != 'I') fwrite(&delCampo, sizeof(char), 1, arquivo);
}
//-----------------------------------------------------//

//***************************************//
//* FUNÇÕES PARA LISTAGEM DOS REGISTROS *//
//***************************************//

/*	Descrição:
		Lista todos os registros do arquivo, independente da sua organização
	Parâmetros:
		arquivo = Arquivo que terá seus registros listados */
void listarBinario(FILE *arquivo) {

	if (org == 'I')
		listarBinario_I(arquivo);

	else if (org == 'D')
		listarBinario_D(arquivo);

	else {
		listarBinario_N(arquivo);
	}
}

/*	Descrição:
		Lista todos os registros de um arquivo, desde que os registros estejam organizados por indicadores de tamanho
	Parâmetros:
		arquivo = Arquivo que terá seus registros listados */
void listarBinario_I(FILE *arquivo) {
	Registro reg; // registro que será listado
	int tamanhoRegistro; // tamanho do registro
	int i; // itera sobre o registro
	int campo = 0; // campo que está sendo lido (0 a 7)
	int iCampo = 0; // indice do campo que está sendo  lido
	char c; // carácter que irá percorrer todo o arquivo
	char string[18]; //string que irá armazena os tamanhos fixos

	// iterenado sobre o arquivo, a cada repetição pega o tamanho de um campo
	while (fread(&tamanhoRegistro, sizeof(int), 1, arquivo) == 1) {

                char buffer;
		// Preparando variáveis para um novo registro
		campo = 0;
		iCampo = 0;
		nullCampos(&reg);

		// itera sobre o registro
		for(i=0; i < tamanhoRegistro; i++) {

			//Lendo campo fixo de CNPJ
			if(campo == 0 || campo == 7) {
				//lendo campo fixo CNPJ
				fread(&string,sizeof(char),18,arquivo);
                                // descartar delimitador de campo
                                fread(&buffer, sizeof(char), 1, arquivo);
				//adiciona a string do CNPJ lida na struct
				preencherCampoCNPJ(&reg, string,campo++);
				i += 18;
			}

			//Lendo campo fixo de Data
			if(campo == 3 || campo == 4) {
				//lendo campo fixo Datas
				fread(&string,sizeof(char),8,arquivo);
                                // descartar delimitador de campo
                                fread(&buffer, sizeof(char), 1, arquivo);
				//adiciona a string da data lida na struct
				preencherCampoData(&reg,string,campo++);
				i += 8;
			}

			//Lendo campo variaveis
			if(campo == 1 || campo == 2 || campo == 5 || campo == 6) {
				// le carácter
				fread(&c, sizeof(char), 1, arquivo);

				// campo novo
				if (c == ';') {
					campo++;
					iCampo = 0;
					continue;
				}

				// adiciona char ao registro
				adicionaCharCampo(&reg, c, campo, iCampo++);
			}
		}

		// exibe registro recuperado do arquivo
		printRegistro(&reg);
		free(reg.razaoSocial);
		free(reg.nomeFantasia);
		free(reg.motivoCancelamento);
		free(reg.nomeEmpresa);
	}
}

/*	Descrição:
		Lista todos os registros de um arquivo, desde que os registros estejam organizados por delimitadores
	Parâmetros:
		arquivo = Arquivo que terá seus registros listados */
void listarBinario_D(FILE *arquivo) {
	Registro reg; // registro que será listado
	int campo; // campo que está sendo lido (0 a 7)
	int iCampo;	// indice do campo que está sendo lido
	char c;	// carácter que irá percorrer todo o arquivo


	// inicializando variáveis
	campo = 0;
	iCampo = 0;
	nullCampos(&reg);

	while (fread(&c, sizeof(char), 1, arquivo) == 1) {

		// registro novo
		if (c == DELIM_REGISTRO) {
			campo = 0;
			iCampo = 0;

			printRegistro(&reg);
			free(reg.razaoSocial);
			free(reg.nomeFantasia);
			free(reg.motivoCancelamento);
			free(reg.nomeEmpresa);

			nullCampos(&reg);
			continue;
		}

		// campo novo
		if (c == DELIM_CAMPO) {
			campo++;
			iCampo = 0;
			continue;
		}

		// adiciona char ao campo
		adicionaCharCampo(&reg, c, campo, iCampo++);
	}
}

/*	Descrição:
		Lista todos os registros de um arquivo, desde que os registros estejam organizados por número fixo de campos
	Parâmetros:
		arquivo = Arquivo que terá seus registros listados */
void listarBinario_N(FILE *arquivo) {
	Registro reg; // registro que será listado
	int campo; // campo que está sendo lido (0 a 7)
	int iCampo;	// indice do campo que está sendo lido
	char c; // carácter que irá percorrer todo o arquivo

	// inicializando variáveis
	campo = 0;
	iCampo = 0;
	nullCampos(&reg);

	// percorre todo o arquivo
	while (fread(&c, sizeof(char), 1, arquivo) == 1) {

		if (c == DELIM_CAMPO) {

			// campo novo
			if (campo < 7) {
				campo++;
				iCampo = 0;
			}
			// registro novo
			else {
				campo = 0;
				iCampo = 0;

				printRegistro(&reg);
				// prepara reg para receber um novo registro
				free(reg.razaoSocial);
				free(reg.nomeFantasia);
				free(reg.motivoCancelamento);
				free(reg.nomeEmpresa);
				nullCampos(&reg);
			}
			continue;
		}

		// adiciona char ao campo
		adicionaCharCampo(&reg, c, campo, iCampo++);

	}
}
//---------------------------------------//

//********************//
//* FUNÇÕES DE BUSCA *//
//********************//

/*	Descrição:
		Faz uma busca no arquivo, independente da sua organização
		e de acordo com o campo e o valor informado
	Parâmetros:
		arquivo = Arquivo que a busca será realizada
		campoBusca = Campo que a busca irá comparar com o valor informado
		strBusca = Valor (string) de comparação da busca
	Retorno:
		Retorna o registro caso ele seja encontrado, caso contrário retorna NULL */
Registro* buscaCampo(FILE *arquivo, int campoBusca, char *strBusca) {
	if (org == 'I')
		return buscaCampo_I(arquivo, campoBusca, strBusca);
	else if (org == 'D')
		return buscaCampo_D(arquivo, campoBusca, strBusca);
	else
		return buscaCampo_N(arquivo, campoBusca, strBusca);
}

/*	Descrição:
		Faz uma busca no arquivo de acordo com o campo e valor informado,
		necessáriamente os registros do arquivo devem ser organizados por indicadores de tamanho
	Parâmetros:
		arquivo = Arquivo que a busca será realizada
		campoBusca = Campo que a busca irá comparar com o valor informado
		strBusca = Valor (string) de comparação da busca
	Retorno:
		Retorna o registro caso ele seja encontrado, caso contrário retorna NULL */
Registro* buscaCampo_I(FILE *arquivo, int campoBusca, char *strBusca) {
	int size; // tamanho do registro
	Registro *reg; // registro que será retornado

	// percorrendo cada registro do arquivo
	while (fread(&size, sizeof(int), 1, arquivo) == 1) {
		reg = getRegistro(arquivo);

		// encontrou
		if (compararCampos(reg, campoBusca, strBusca) == 0)
			return reg;

		// não encontrou
		free(reg->razaoSocial);
		free(reg->nomeFantasia);
		free(reg->motivoCancelamento);
		free(reg->nomeEmpresa);
		free(reg);
	}

	return NULL;
}

/*
		Faz uma busca no arquivo de acordo com o campo e valor informado,
		necessáriamente os registros do arquivo devem ser organizados por um delimitador entre eles
	Parâmetros:
		arquivo = Arquivo que a busca será realizada
		campoBusca = Campo que a busca irá comparar com o valor informado
		strBusca = Valor (string) de comparação da busca
	Retorno:
		Retorna o registro caso ele seja encontrado, caso contrário retorna NULL
*/
Registro* buscaCampo_D(FILE *arquivo, int campoBusca, char *strBusca) {
	char c; // carácter que irá percorrer todo o arquivo
	Registro *reg; // registro que será retornado
	int campo = 0; // campo que está sendo lido (0 a 7)
	int iCampo = 0; // indice do campo que está sendo lido

	// aloca e prepara o registro
	reg = (Registro *) malloc(sizeof(Registro));
	nullCampos(reg);

	// percorre arquivo char a char
	while(fread(&c, sizeof(char), 1, arquivo) == 1) {

		// troca de campo
		if (c == DELIM_CAMPO) {
			campo++;
			iCampo = 0;
			continue;
		}

		if (c == DELIM_REGISTRO) {
			// encontrou
			if (compararCampos(reg, campoBusca, strBusca) == 0)
				return reg;

			// troca de registro
			campo = 0;
			iCampo = 0;
			continue;
		}

		// adiciona o carácter lido no registro
		adicionaCharCampo(reg, c, campo, iCampo++);
	}

	// não encontrou
	free(reg->razaoSocial);
	free(reg->nomeFantasia);
	free(reg->motivoCancelamento);
	free(reg->nomeEmpresa);
	free(reg);
	return NULL;
}

/*	Descrição:
		Faz uma busca no arquivo de acordo com o campo e valor informado,
		necessáriamente os registros do arquivo devem ser organizados por número fixo de campos
	Parâmetros:
		arquivo = Arquivo que a busca será realizada
		campoBusca = Campo que a busca irá comparar com o valor informado
		strBusca = Valor (string) de comparação da busca
	Retorno:
		Retorna o registro caso ele seja encontrado, caso contrário retorna NULL */
Registro* buscaCampo_N(FILE *arquivo, int campoBusca, char *strBusca) {
	char c; // carácter que irá percorrer todo o arquivo
	Registro *reg; // registro que será retornado
	int campo = 0; // campo que está sendo lido (0 a 7)
	int iCampo = 0; // indice do campo que está sendo lido

	// aloca e prepara o registro
	reg = (Registro *) malloc(sizeof(Registro));
	nullCampos(reg);

	// percorre arquivo char a char
	while(fread(&c, sizeof(char), 1, arquivo) == 1) {

		if (c == DELIM_CAMPO) {

			// troca de registro
			if (campo == 7) {

				// encontrou
				if (compararCampos(reg, campoBusca, strBusca) == 0)
					return reg;

				campo = 0;
				iCampo = 0;
			}
			// troca de campo
			else {
				campo++;
				iCampo = 0;
			}
			continue;
		}
		// adiciona o carácter lido no registro
		adicionaCharCampo(reg, c, campo, iCampo++);
	}

	// não encontrou
	free(reg->razaoSocial);
	free(reg->nomeFantasia);
	free(reg->motivoCancelamento);
	free(reg->nomeEmpresa);
	free(reg);
	return NULL;
}

/*	Descrição:
		Faz uma busca no arquivo, independente da sua organização
		e de acordo com o campo e o valor informado
	Parâmetros:
		arquivo = Arquivo que a busca será realizada
		campoBusca = Campo que a busca irá comparar com o valor informado
		strBusca = Valor (string) de comparação da busca
	Retorno:
		Retorna o campo caso ele seja encontrado, caso contrário retorna NULL */
Registro* buscaReg(FILE *arquivo, int regBusca) {
	if (org == 'I')
		return buscaReg_I(arquivo, regBusca);
	else if (org == 'D')
		return buscaReg_D(arquivo, regBusca);
	else
		return buscaReg_N(arquivo, regBusca);
}

/*	Descrição:
		Faz uma busca no arquivo de acordo com o campo e valor informado,
		necessáriamente os registros do arquivo devem ser organizados por indicadores de tamanho
	Parâmetros:
		arquivo = Arquivo que a busca será realizada
		campoBusca = Campo que a busca irá comparar com o valor informado
		strBusca = Valor (string) de comparação da busca
	Retorno:
		Retorna o campo caso ele seja encontrado, caso contrário retorna NULL */
Registro* buscaReg_I(FILE *arquivo, int regBusca) {
	int size; // tamanho do registro
	int countReg = 1; //conta quantos registros foram lidos

	// percorre cada registro do arquivo
	while (fread(&size, sizeof(int), 1, arquivo) == 1)

		// pula para próximo registro
		if (countReg++ < regBusca)
			fseek(arquivo, size, SEEK_CUR);

		// registro encontrado
		else
			return getRegistro(arquivo);

	// não ecnotrou
	return NULL;
}

/*	Descrição:
		Faz uma busca no arquivo de acordo com o campo e valor informado,
		necessáriamente os registros do arquivo devem ser organizados por um delimitador entre eles
	Parâmetros:
		arquivo = Arquivo que a busca será realizada
		campoBusca = Campo que a busca irá comparar com o valor informado
		strBusca = Valor (string) de comparação da busca
	Retorno:
		Retorna o campo caso ele seja encontrado, caso contrário retorna NULL */
Registro* buscaReg_D(FILE *arquivo, int regBusca) {
	char c; // carácter que irá percorrer todo o arquivo
	Registro *reg; // registro que será retornado
	int campo = 0; // campo que está sendo lido (0 a 7)
	int iCampo = 0; // indice do campo que está sendo lido
	int countReg = 1; //conta quantos registros foram lidos

	// aloca e prepara o registro
	reg = (Registro *) malloc(sizeof(Registro));
	nullCampos(reg);

	// percorre o arquivo char a char
	while(fread(&c, sizeof(char), 1, arquivo) == 1) {

		// trocou de campo
		if (c == DELIM_CAMPO) {
			campo++;
			iCampo = 0;
			continue;
		}

		// trocou de registro
		if (c == DELIM_REGISTRO) {
			// encontrou
			if (countReg++ == regBusca)
				return reg;

			campo = 0;
			iCampo = 0;
			continue;
		}

		// adiciona char lido ao registro
		adicionaCharCampo(reg, c, campo, iCampo++);
	}

	// não encontrou
	free(reg->razaoSocial);
	free(reg->nomeFantasia);
	free(reg->motivoCancelamento);
	free(reg->nomeEmpresa);
	free(reg);
	return NULL;
}

/*	Descrição:
		Faz uma busca no arquivo de acordo com o campo e valor informado,
		necessáriamente os registros do arquivo devem ser organizados por número fixo de campos
	Parâmetros:
		arquivo = Arquivo que a busca será realizada
		campoBusca = Campo que a busca irá comparar com o valor informado
		strBusca = Valor (string) de comparação da busca
	Retorno:
		Retorna o campo caso ele seja encontrado, caso contrário retorna NULL */
Registro* buscaReg_N(FILE *arquivo, int regBusca) {
	char c; // carácter que irá percorrer todo o arquivo
	Registro *reg; // registro que será retornado
	int campo = 0; // campo que está sendo lido (0 a 7)
	int iCampo = 0; // indice do campo que está sendo lido
	int countReg = 1; //conta quantos registros foram lidos

	// aloca e prepara o registro
	reg = (Registro *) malloc(sizeof(Registro));
	nullCampos(reg);

	// percorre o arquivo char a char
	while(fread(&c, sizeof(char), 1, arquivo) == 1) {

		if (c == DELIM_CAMPO) {

			// troca de registro
			if (campo == 7) {
				// encontrou
				if (countReg++ == regBusca)
					return reg;

				campo = 0;
				iCampo = 0;
			}
			// troca de campo
			else {
				campo++;
				iCampo = 0;
			}

			continue;
		}

		// adiciona char lido ao registro
		adicionaCharCampo(reg, c, campo, iCampo++);
	}

	// não encontrou
	free(reg->razaoSocial);
	free(reg->nomeFantasia);
	free(reg->motivoCancelamento);
	free(reg->nomeEmpresa);
	free(reg);
	return NULL;
}
//--------------------//

//************************//
//* FUNÇÕES DE INDEXAÇÃO *//
//************************//

void gerarArquivoIndice();

//--------------------//

//**********************//
//* FUNÇÕES DE REMOÇÃO *//
//**********************//

void removerRegistro();

//--------------------//

//*******************************//
//* FUNÇÕES ÚTEIS (SUB FUNÇÕES) *//
//*******************************//

/*	Descrição:
		Função utilizada para preencher os campos variáveis, sua função é inserir um char em um campo,
		dado o registro, o campo e sua posição no campo
	Parâmetros:
		reg = Registro que terá o campo com o char adicionado
		c = Char que será adicionado
		campo = Indica qual campo terá o char adicionado (seus valores vão de 0 a 7)
		iCampo = Indice que indica em qual posição do campo o char será inserido
*/
void adicionaCharCampo(Registro *reg, char c, int campo, int iCampo) {
    switch(campo) {
        // CNPJ
        case 0:
            reg->cnpj[iCampo] = c;
            reg->cnpj[++iCampo] = '\0';
        break;

        // Nome Social
        case 1:
            reg->razaoSocial = (char*)realloc(reg->razaoSocial, sizeof(char) * iCampo+2);
            reg->razaoSocial[iCampo] = c;
            reg->razaoSocial[++iCampo] = '\0';
        break;

        // Nome Fantasia
        case 2:
            reg->nomeFantasia = (char*)realloc(reg->nomeFantasia, sizeof(char) * iCampo+2);
            reg->nomeFantasia[iCampo] = c;
            reg->nomeFantasia[++iCampo] = '\0';
        break;

        // Data do Registro
        case 3:
            reg->dataRegistro[iCampo++] = c;
            reg->dataRegistro[iCampo] = '\0';
        break;

        // Data do Registro
        case 4:
            reg->dataCancelamento[iCampo++] = c;
            reg->dataCancelamento[iCampo] = '\0';
        break;

        // Motivo do Cancelamento
        case 5:
            reg->motivoCancelamento = (char*)realloc(reg->motivoCancelamento, sizeof(char) * iCampo+2);
            reg->motivoCancelamento[iCampo] = c;
            reg->motivoCancelamento[++iCampo] = '\0';
        break;

        // Nome da Empresa de Auditoria
        case 6:
            reg->nomeEmpresa = (char*)realloc(reg->nomeEmpresa, sizeof(char) * iCampo+2);
            reg->nomeEmpresa[iCampo] = c;
            reg->nomeEmpresa[++iCampo] = '\0';
        break;

        // CNPJ da Empresa de Auditoria
        case 7:
            reg->cnpjAuditor[iCampo++] = c;
            reg->cnpjAuditor[iCampo] = '\0';
        break;
    }
}

/*	Descrição:
		Função utilizada para preencher os campos fixoes de CNPJ
	Parâmetros:
		reg = Registro que terá o campo com o char adicionado
		c = Char que será adicionado
		campo = Indica qual campo terá o char adicionado (seus valores vão de 0 a 7)
		iCampo = Indice que indica em qual posição do campo o char será inserido
*/

void preencherCampoCNPJ(Registro *reg, char c[18], int campo) {
	int i;
	switch(campo) {
		case 0:
			for(i=0;i<18;i++) {
				reg->cnpj[i] = c[i];
			}
			reg->cnpj[i] = '\0';
		break;

		case 7:
			for(i=0;i<18;i++) {
				reg->cnpjAuditor[i] = c[i];
			}
			reg->cnpjAuditor[i] = '\0';
		break;
	}
}

/*	Descrição:
		Função utilizada para preencher os campos fixoes referentes a data
	Parâmetros:
		reg = Registro que terá o campo com o char adicionado
		c = Char que será adicionado
		campo = Indica qual campo terá o char adicionado (seus valores vão de 0 a 7)
		iCampo = Indice que indica em qual posição do campo o char será inserido
*/
void preencherCampoData(Registro *reg, char c[18], int campo) {
	int i;
	switch(campo) {
		case 3:
			for(i=0;i<8;i++) {
				reg->dataRegistro[i] = c[i];
			}
			reg->dataRegistro[i] = '\0';
		break;

		case 4:
			for (i=0; i < 8; i++) {
				reg->dataCancelamento[i] = c[i];
			}
			reg->dataCancelamento[i] = '\0';
		break;
	}
}

/*	Descrição:
		Dado um registro esta função atribui NULL aos seus campos de tamanho variável.
		É utilizada quando um registro é recém criado e seus campos variáveis irão passar por um realloc
	Parâmetros:
		reg = Registro que irá receber null em seus campos variáveis
*/
void nullCampos(Registro *reg) {
    reg->razaoSocial  = NULL;
    reg->nomeFantasia = NULL;
    reg->motivoCancelamento  = NULL;
    reg->nomeEmpresa  = NULL;
}

/*	Descrição:
		Exibe um registro na saída padrão.
	Parâmetros:
		reg = Registro que será exibido na saída padrão */
void printRegistro(Registro *reg) {

	printf("CNPJ.........: ");
	if(reg->cnpj[0] == 'n') {
		printf("null\n");
	}
	else{
		printf("%s\n",reg->cnpj);
	}

	printf("Razão Social.: %s\n",   reg->razaoSocial);
	printf("Nome Fantasia: %s\n",   reg->nomeFantasia);

	printf("Data Registro: ");
	if(reg->dataRegistro[0] == 'n') {
		printf("null\n");
	}
	else{
		printf("%s\n", reg->dataRegistro);
	}

	printf("Data Canc....: ");
	if(reg->dataCancelamento[0] == 'n') {
		printf("null\n");
	}
	else{
		printf("%s\n", reg->dataCancelamento);
	}

	printf("Motivo Canc..: %s\n",   reg->motivoCancelamento);
	printf("Empresa Aud..: %s\n",   reg->nomeEmpresa);

	printf("CNPJ Emp Aud.: ");
	if(reg->cnpjAuditor[0] == 'n') {
		printf("null\n\n");
	}
	else{
		printf("%s\n\n", reg->cnpjAuditor);
	}
}

/*	Descrição:
		Função que compara o campo de um registro com um valor informado
	Parâmetros:
		reg = Registro cujo campo irá sofrer a comparação
		campo = Indica qual campo irá sofrer a comparação (seus valores vão de 1 a 8)
		strBusca = Valor (string) que será comparado com o campo do registro dado
	Retorno:
		Retorna o resultado da comparação, 0 para valores iguais e diferente de 0 caso contrário */
int compararCampos(Registro *reg, int campo, char *strBusca) {
	int cmp = -1; // armazena o resultado da comparação

	switch(campo) {
		case 1:
			if (reg->cnpj != NULL)
				cmp = strcmp(reg->cnpj, strBusca);
		break;

		case 2:
			if (reg->razaoSocial != NULL)
				cmp = strcmp(reg->razaoSocial, strBusca);
		break;

		case 3:
			if (reg->nomeFantasia != NULL)
				cmp = strcmp(reg->nomeFantasia, strBusca);
		break;

		case 4:
			if (reg->dataRegistro != NULL)
				cmp = strcmp(reg->dataRegistro, strBusca);
		break;

		case 5:
			if (reg->dataCancelamento != NULL)
				cmp = strcmp(reg->dataCancelamento, strBusca);
		break;

		case 6:
			if (reg->motivoCancelamento != NULL)
				cmp = strcmp(reg->motivoCancelamento, strBusca);
		break;

		case 7:
			if (reg->nomeEmpresa != NULL)
				cmp = strcmp(reg->nomeEmpresa, strBusca);
		break;

		case 8:
			if (reg->cnpjAuditor != NULL)
				cmp = strcmp(reg->cnpjAuditor, strBusca);
		break;
	}

	return(cmp);
}

/*	Descrição:
		Dado a posição de um registro no arquivo, a função procura o campo informado e retorna seu valor
	Parâmetros:
		arquivo = Arquivo cuja busca será realizada
		regBusca = Localização do registro no arquivo (seus valores vão de 1 a n, onde n é o número máximo
		de registros no arquivo)
		campoBusca = Indica qual campo será retornado (seus valores vão de 1 a 8)
	Retorno:
		Retorna o valor correspondente ao campo desejado, caso o registro não seja encontrado retorna NULL */
char* buscaRegCampo(FILE *arquivo, int regBusca, int campoBusca) {
	Registro *reg; // registro buscado
	char *retorno = NULL; // aramzena o valor do campo buscado

	// busca o registro
	reg = buscaReg(arquivo, regBusca);

	if (reg == NULL)
		return NULL;
	else
		switch(campoBusca) {
			case 1:
				if (reg->cnpj != NULL) {
					// aloca string que ira receber o campo
					retorno = (char *) malloc(sizeof(char) * strlen(reg->cnpj)+1);
					// copia campo para a sting de retorno
					memcpy(retorno, reg->cnpj, strlen(reg->cnpj)+1);
				}
			break;

			case 2:
				if (reg->razaoSocial != NULL) {
					retorno = (char *) malloc(sizeof(char) * strlen(reg->razaoSocial)+1);
					memcpy(retorno, reg->razaoSocial, strlen(reg->razaoSocial)+1);
				}
			break;

			case 3:
				if (reg->nomeFantasia != NULL) {
					retorno = (char *) malloc(sizeof(char) * strlen(reg->nomeFantasia)+1);
					memcpy(retorno, reg->nomeFantasia, strlen(reg->nomeFantasia)+1);
				}
			break;

			case 4:
				if (reg->dataRegistro != NULL) {
					retorno = (char *) malloc(sizeof(char) * strlen(reg->dataRegistro)+1);
					memcpy(retorno, reg->dataRegistro, strlen(reg->dataRegistro)+1);
				}
			break;

			case 5:
				if (reg->dataCancelamento != NULL) {
					retorno = (char *) malloc(sizeof(char) * strlen(reg->dataCancelamento)+1);
					memcpy(retorno, reg->dataCancelamento, strlen(reg->dataCancelamento)+1);
				}
			break;

			case 6:
				if (reg->motivoCancelamento != NULL) {
					retorno = (char *) malloc(sizeof(char) * strlen(reg->motivoCancelamento)+1);
					memcpy(retorno, reg->motivoCancelamento, strlen(reg->motivoCancelamento)+1);
				}
			break;

			case 7:
				if (reg->nomeEmpresa != NULL) {
					retorno = (char *) malloc(sizeof(char) * strlen(reg->nomeEmpresa)+1);
					memcpy(retorno, reg->nomeEmpresa, strlen(reg->nomeEmpresa)+1);
				}
			break;

			case 8:
				if (reg->cnpjAuditor != NULL) {
					retorno = (char *) malloc(sizeof(char) * strlen(reg->cnpjAuditor)+1);
					memcpy(retorno, reg->cnpjAuditor, strlen(reg->cnpjAuditor)+1);
				}
			break;
		}


	free(reg->razaoSocial);
	free(reg->nomeFantasia);
	free(reg->motivoCancelamento);
	free(reg->nomeEmpresa);
	free(reg);
	return retorno;
}

/*	Descrição:
		Dado um arquivo com seu ponteiro no início de um registro, o registro é retornado e o ponteiro do arquivo
		deslocado para uma posição após o registro
	Parâmetros:
		arquivo = Arquivo cujo registro será retornado
	Retorno:
		Retorna o registro capturado do arquivo */
Registro* getRegistro(FILE *arquivo) {
        char buffer; // descarte
	Registro *reg; // registro que será retornado
	char *campo; //campo genérico, usado para preencher o campo dos registros
	int i; // itera sobre os campos

	// alocando registro que será retornado
	reg = (Registro *) malloc(sizeof(Registro));

	// CNPJ
	fread(&reg->cnpj,sizeof(char),18,arquivo);

        // ler delimitador
        fread(&buffer, sizeof(char), 1, arquivo);

	reg->cnpj[18] = '\0';

	// Razao Social
	reg->razaoSocial = getCampo(arquivo);

	// Nome Fantasia
	reg->nomeFantasia = getCampo(arquivo);

	// Data do registro
	fread(&reg->dataRegistro,sizeof(char),8,arquivo);

        // ler delimitador
        fread(&buffer, sizeof(char), 1, arquivo);

	reg->dataRegistro[8] = '\0';

	// Data do Cancelamento
	fread(&reg->dataCancelamento,sizeof(char),8,arquivo);

        // ler delimitador
        fread(&buffer, sizeof(char), 1, arquivo);

	reg->dataCancelamento[8] = '\0';

	// Motivo do Cancelamento
	reg->motivoCancelamento = getCampo(arquivo);

	// Nome da Empresa de Auditoria
	reg->nomeEmpresa = getCampo(arquivo);

	// CNPJ da Empresa de Auditoria
	fread(&reg->cnpjAuditor,sizeof(char),18,arquivo);

        // ler delimitador
        fread(&buffer, sizeof(char), 1, arquivo);

	reg->cnpjAuditor[18] = '\0';

	return(reg);
}

/*	Descrição:
		Dado um arquivo com seu ponteiro no início de um campo, o campo é retornado e o ponteiro do arquivo
		deslocado para uma posição após o delimitador do campo, ou seja, no inicio do campo/registro sequente
	Parâmetros:
		arquivo = Arquivo cujo campo será retornado
	Retorno:
		Retorna o campo capturado do arquivo
*/
char* getCampo(FILE *arquivo) {
	char c; // carácter usado para percorrer o campo
	int i = 0; //usado para iterar sobre o campo
	char *campo = NULL; // campo que será retornado

	// le um caracter
	fread(&c, sizeof(char), 1, arquivo);

	// percorre até o delimitador de campo
	while (c != DELIM_CAMPO) {
		// adiciona o char ao campo
		campo = (char*)realloc(campo, sizeof(char) * (i+2));
        campo[i] = c;
        campo[++i] = '\0';
        // le um novo char
        fread(&c, sizeof(char), 1, arquivo);
	}

	return campo;
}
//----------------------------//
