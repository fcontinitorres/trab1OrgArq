// Armazena qual a organização de arquivo o programa irá trabalhar
// Possível valores:
// 	I = Indicador de tamanho
// 	D = Delimitador entre registros
// 	N = Número fixo de campos
char org;

// delimitador do registro no arquivo de saída
#define DEL_REG '#'

// delimitador do campo arquivo de saída
#define DEL_FIELD ';'

// caminho do arquivo de entrada
#define FILE_IN "data/SCC0215012017projeto01turmaBdadosCompanhias.csv"

// caminho do arquivo de saida
#define FILE_OUT "out/saida.bin"