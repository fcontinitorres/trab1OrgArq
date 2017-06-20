/*
GRUPO G5
Felipe Scrochio Custodio, 9442688
Felipe Contini Torres, 9253670
Júlia Diniz Ferreira, 9364865
Bruno Henrique Rasteiro, 9292910
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parametros.h"

//*************************************************//
//* ESTRUTURA DE DADOS QUE REPRESENTA UM REGISTRO *//
//*************************************************//
typedef struct {
        char cnpj[19];
        char *razaoSocial;
        char *nomeFantasia;
        char dataRegistro[9];
        char dataCancelamento[9];
        char *motivoCancelamento;
        char *nomeEmpresa;
        char cnpjAuditor[19];
} Registro;

//*******************//
//* FUNÇÕES DO MAIN *//
//*******************//
int validaArquivos(FILE **arquivoEntrada, FILE **arquivoSaida1, FILE **arquivoSaida2, FILE **arquivoSaida3);

void opcao1(FILE *arquivo);

void opcao2(FILE *arquivo);

void opcao3(FILE *arquivo);

void opcao4(FILE *arquivo);

char* converterString(char *str);
//-------------------//


//*****************************************************//
//* FUNÇÕES PARA GERAR O ARQUIVO BINÁRIO DE REGISTROS *//
//*****************************************************//
void checarCamposFixos(Registro *reg);

void gerarBinarioCSV(FILE *arquivoEntrada, FILE *arquivo);

void salvarCampo(Registro *reg, FILE *arquivo);

void salvarRegistro(Registro *reg, FILE *arquivo);
//-----------------------------------------------------//


//***************************************//
//* FUNÇÕES PARA LISTAGEM DOS REGISTROS *//
//***************************************//
void listarBinario(FILE *arquivo);

void listarBinario_I(FILE *arquivo);

void listarBinario_D(FILE *arquivo);

void listarBinario_N(FILE *arquivo);
//---------------------------------------//

//********************//
//* FUNÇÕES DE BUSCA *//
//********************//
Registro* buscaCampo(FILE *arquivo, int campoBusca, char *strBusca);

Registro* buscaCampo_I(FILE *arquivo, int campoBusca, char *strBusca);

Registro* buscaCampo_D(FILE *arquivo, int campoBusca, char *strBusca);

Registro* buscaCampo_N(FILE *arquivo, int campoBusca, char *strBusca);

Registro* buscaReg(FILE *arquivo, int regBusca);

Registro* buscaReg_I(FILE *arquivo, int regBusca);

Registro* buscaReg_D(FILE *arquivo, int regBusca);

Registro* buscaReg_N(FILE *arquivo, int regBusca);
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

//****************************//
//* SUB FUNÇÕES - UTILIDADES *//
//****************************//
void adicionaCharCampo(Registro *reg, char c, int campo, int iCampo);

void preencherCampoCNPJ(Registro *reg, char c[18], int campo);

void preencherCampoData(Registro *reg,char c[18],int campo);

void nullCampos(Registro *reg);

void printRegistro(Registro *reg);

int compararCampos(Registro *reg, int campo, char *strBusca);

char* buscaRegCampo(FILE *arquivo, int regBusca, int campoBusca);

Registro* getRegistro(FILE *arquivo);

char* getCampo(FILE *arquivo);
//----------------------------//
