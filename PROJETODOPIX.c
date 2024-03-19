#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_CHAVES 10
#define MAX_USUARIOS 5

struct usuario {
  int senha;
  int cpf;
  char email[50];
};
struct transacao_pix {
  int chave_origem;
  int chave_destino;
  double valor;
  double saldo_disponivel;
  char status[20];
};

void ler_chaves_pix(struct usuario usuarios[], int *num_usuarios) {
  printf("Quantos usu?rios deseja cadastrar? (até %d): ", MAX_USUARIOS);
  scanf("%d", num_usuarios);

  printf("Digite os dados dos usu?rios:\n");
  for (int i = 0; i < *num_usuarios; i++) {
    printf("Usu?rio %d:\n", i + 1);
    printf("Senha: ");
    scanf("%d", &usuarios[i].senha);
    printf("CPF: ");
    scanf("%d", &usuarios[i].cpf);
    printf("Email: ");
    scanf("%s", usuarios[i].email);
  }
}

void selecionar_chave(int *chave_selecionada, struct usuario usuarios[], int num_usuarios) {
  int opcao;

  do {
    printf("Selecione a chave PIX (CPF):\n");
    for (int i = 0; i < num_usuarios; i++) {
      printf("%d. CPF: %d\n", i + 1, usuarios[i].cpf);
    }
    printf("Op??o: ");
    scanf("%d", &opcao);

    if (opcao >= 1 && opcao <= num_usuarios) {
      *chave_selecionada = usuarios[opcao - 1].cpf;
    } else {
      printf("Op??o inv?lida. Por favor, escolha uma chave PIX v?lida.\n");
    }
  } while (opcao < 1 || opcao > num_usuarios);
}

struct transacao_pix *gerar_transacao(int chave_origem, int chave_destino, double valor, double saldo_disponivel) {
  struct transacao_pix *transacao = (struct transacao_pix *)malloc(sizeof(struct transacao_pix));

  if (transacao == NULL) {
    printf("Erro ao alocar mem?ria para a transa??o.\n");
    exit(EXIT_FAILURE);
  }

  transacao->chave_origem = chave_origem;
  transacao->chave_destino = chave_destino;
  transacao->valor = valor;

  if (valor > saldo_disponivel) {
    strcpy(transacao->status, "Saldo insuficiente");
  } else {
    saldo_disponivel -= valor;
    transacao->saldo_disponivel = saldo_disponivel;
    strcpy(transacao->status, "Conclu?da");
  }

  return transacao;
}

void salvar_transacao_arquivo(const struct transacao_pix *transacao) {
  FILE *arquivo = fopen("comprovante_transacao.txt", "a"); // Modo de abertura alterado para "a" para anexar ao arquivo

  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo de comprovante.\n");
    exit(EXIT_FAILURE);
  }

  fprintf(arquivo, "Chave do remetente (CPF): %d\n", transacao->chave_origem);
  fprintf(arquivo, "Chave do destinat?rio (CPF): %d\n", transacao->chave_destino);
  fprintf(arquivo, "Valor da transa??o: %.2f\n", transacao->valor);
  fprintf(arquivo, "Saldo dispon?vel ap?s a transa??o: %.2f\n", transacao->saldo_disponivel);
  fprintf(arquivo, "Status da transa??o: %s\n\n", transacao->status);

  fclose(arquivo);
  printf("Comprovante da transa??o salvo em comprovante_transacao.txt\n\n");
}

void limpar_historico_transacoes() {
  // Abre o arquivo de hist?rico de transa??es em modo de escrita para sobrescrever o conte?do
  FILE *arquivo = fopen("comprovante_transacao.txt", "w");
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo de hist?rico de transa??es para limpeza.\n");
    exit(EXIT_FAILURE);
  }
  fclose(arquivo);
  printf("Hist?rico de transa??es limpo.\n");
}

int main() {
  setlocale(LC_ALL, "portuguese_Brazil");

  struct usuario usuarios[MAX_USUARIOS];
  int num_usuarios = 0;

  ler_chaves_pix(usuarios, &num_usuarios);

  int continuar = 1; // Vari?vel para controlar se o usu?rio deseja continuar fazendo transa??es

  while (continuar) {
    int chave_origem, chave_destino;
    double valor, saldo_disponivel = 1000.0; // Valor inicial do saldo

    selecionar_chave(&chave_origem, usuarios, num_usuarios);
    selecionar_chave(&chave_destino, usuarios, num_usuarios);

    printf("Informe o valor da transa??o: ");
    scanf("%lf", &valor);

    struct transacao_pix *transacao = gerar_transacao(chave_origem, chave_destino, valor, saldo_disponivel);

    salvar_transacao_arquivo(transacao);

    free(transacao);

    // Perguntar ao usu?rio se deseja fazer outra transa??o
    printf("Deseja fazer outra transa??o? (1 - Sim / 0 - N?o): ");
    scanf("%d", &continuar);
    printf("\n");
  }

  // Se o usu?rio optou por sair, limpar o hist?rico de transa??es
  limpar_historico_transacoes();

  return 0;
}
//miiuiuiujgggfhgfghohlkdshakdlhaskdhaksldhasljk