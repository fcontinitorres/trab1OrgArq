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
        char *razSoc;
        char *nomeFant;
        char dtReg[9];
        char dtCanc[9];
        char *motCanc;
        char *nomeEmp;
        char cnpjAud[19];
} Registro;

//*******************//
//* FUNÇÕES DO MAIN *//
//*******************//
int validaArquivos(FILE **fileIn, FILE **fileOut);

void opcao1(FILE *file);

void opcao2(FILE *file);

void opcao3(FILE *file);

void opcao4(FILE *file);

char* convertStr(char *str);
//-------------------//


//*****************************************************//
//* FUNÇÕES PARA GERAR O ARQUIVO BINÁRIO DE REGISTROS *//
//*****************************************************//
void checkSizeFixedFields(Registro *reg);

void csv2Bin(FILE *fileIn, FILE *file);

void saveField(Registro *reg, FILE *file);

void saveReg(Registro *reg, FILE *file);
//-----------------------------------------------------//


//***************************************//
//* FUNÇÕES PARA LISTAGEM DOS REGISTROS *//
//***************************************//
void listBin(FILE *file);

void listBin_I(FILE *file);

void listBin_D(FILE *file);

void listBin_N(FILE *file);
//---------------------------------------//


//********************//
//* FUNÇÕES DE BUSCA *//
//********************//
Registro* buscaCampo(FILE *file, int fieldBusca, char *strBusca);

Registro* buscaCampo_I(FILE *file, int fieldBusca, char *strBusca);

Registro* buscaCampo_D(FILE *file, int fieldBusca, char *strBusca);

Registro* buscaCampo_N(FILE *file, int fieldBusca, char *strBusca);

Registro* buscaReg(FILE *file, int regBusca);

Registro* buscaReg_I(FILE *file, int regBusca);

Registro* buscaReg_D(FILE *file, int regBusca);

Registro* buscaReg_N(FILE *file, int regBusca);
//--------------------//

//****************************//
//* SUB FUNÇÕES - UTILIDADES *//
//****************************//
void addCharField(Registro *reg, char c, int field, int iField);

void addStringFieldCNPJ(Registro *reg, char c[18], int field);

void addStringFieldDate(Registro *reg,char c[18],int field);

void nullFields(Registro *reg);

void printReg(Registro *reg);

int compareField(Registro *reg, int field, char *strBusca);

char* buscaRegCampo(FILE *file, int regBusca, int fieldBusca);

Registro* getReg(FILE *file);

char* getField(FILE *file);
//----------------------------//
