/*
GRUPO G5
Felipe Scrochio Custodio, 9442688
Felipe Contini Torres, 9253670
Júlia Diniz Ferreira, 9364865
Bruno Henrique Rasteiro, 9292910
*/

#include "../src/funcoes.h"
#include "../src/parametros.h"

int main() {
	int flag;
	int opcao;
	FILE *arquivoEntrada, *arquivoSaida;

	// verifica se os arquivos foram abertos com sucesso
	if (!validaArquivos(&arquivoEntrada, &arquivoSaida)) {
		return(0);
	}

	flag = 1;
	while (flag) {
		// seleciona o tipo da organização do arquivo
		printf("\n++++++++++++++++++++++++++++++++++++++++\n");
		printf("+ TRABALHO 1 - ORGANIZAÇÃO DE ARQUIVOS +\n");
		printf("++++++++++++++++++++++++++++++++++++++++\n");
		printf("1 - Indicador de tamanho\n");
		printf("2 - Delimitador entre registros\n");
		printf("3 - Numero fixo de campos\n");
		printf("Informe a organização de arquivo desejada: ");
		scanf("%d", &opcao);
		getchar();

		if (opcao < 1 || opcao > 3) {
			printf("\nOpcao invalida.\n");
			continue;
		}

		if (opcao == 1)
			org = 'I';
		else if (opcao == 2)
			org = 'D';
		else
			org = 'N';

		flag = 0;
	}

	// gera arquivo de saida (binario)
	gerarBinarioCSV(arquivoEntrada, arquivoSaida);

	// fechando arquivo
	fclose(arquivoEntrada);

	// reseta ponteiro do arquivo binario
	fseek(arquivoSaida, 0, SEEK_SET);

	flag = 1;
	while(flag) {

    	printf("\n++++++++\n");
    	printf("+ MENU +\n");
    	printf("++++++++\n");
    	printf("1 - Listar todos os registros\n");
		printf("2 - Buscar um registro pelo campo\n");
		printf("3 - Buscar um registro pela posição\n");
		printf("4 - Buscar um campo pela posição do registro\n");
		printf("5 - Sair\n");
		printf("Escolha uma opção: ");
		scanf("%d", &opcao);
		getchar();

		switch(opcao) {
			case 1:
				opcao1(arquivoSaida);
			break;

			case 2:
				opcao2(arquivoSaida);
			break;

			case 3:
				opcao3(arquivoSaida);
			break;

			case 4:
				opcao4(arquivoSaida);
			break;

			case 5:
				flag = 0;
			break;

			default:
				printf("\nOpcao invalida.\n");
		}
	}

	// fechando arquivo
	fclose(arquivoSaida);
}
