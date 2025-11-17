#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
    Struct Territorio:
    Representa um território no jogo.
*/
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

/* Protótipos das funções */
Territorio* cadastrarTerritorios(int total);
void exibirTerritorios(Territorio* mapa, int total);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* mapa);


/* ========================= MAIN ========================= */
int main() {
    srand(time(NULL));  // Inicializa gerador de números aleatórios

    int total;
    printf("Digite o numero de territorios que deseja cadastrar: ");
    scanf("%d", &total);

    // Alocação dinâmica do vetor de territórios
    Territorio* mapa = cadastrarTerritorios(total);

    int opcao;

    do {
        printf("\n=========== MENU ===========\n");
        printf("1 - Exibir territorios\n");
        printf("2 - Realizar ataque\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            exibirTerritorios(mapa, total);
        }
        else if (opcao == 2) {
            int at, df;

            exibirTerritorios(mapa, total);

            printf("\nDigite o indice do territorio atacante (1 a %d): ", total);
            scanf("%d", &at);

            printf("Digite o indice do territorio defensor (1 a %d): ", total);
            scanf("%d", &df);

            // Ajuste do número digitado (para começar em 1 ao invés de 0)
            at--;
            df--;

            // Validação dos índices
            if (at < 0 || at >= total || df < 0 || df >= total) {
                printf("Indices invalidos!\n");
                continue;
            }

            // Validação de cor
            if (strcmp(mapa[at].cor, mapa[df].cor) == 0) {
                printf("Um territorio nao pode atacar outro da mesma cor!\n");
                continue;
            }

            atacar(&mapa[at], &mapa[df]);
        }

    } while (opcao != 0);

    liberarMemoria(mapa);

    return 0;
}


/* ========================= FUNÇÕES ========================= */

/* Cadastro dinâmico dos territórios */
Territorio* cadastrarTerritorios(int total) {

    Territorio* mapa = (Territorio*) calloc(total, sizeof(Territorio));

    printf("\n=== Cadastro de Territorios ===\n");

    for (int i = 0; i < total; i++) {
        printf("\nTerritorio %d:\n", i + 1);

        printf("Nome: ");
        scanf("%s", mapa[i].nome);

        printf("Cor do exercito: ");
        scanf("%s", mapa[i].cor);

        printf("Tropas: ");
        scanf("%d", &mapa[i].tropas);
    }

    return mapa;
}


/* Exibe todos os territórios cadastrados */
void exibirTerritorios(Territorio* mapa, int total) {

    printf("\n======= Territorios Cadastrados =======\n");

    for (int i = 0; i < total; i++) {
        printf("\nID %d:\n", i + 1);
        printf("Nome: %s\n", mapa[i].nome);
        printf("Cor: %s\n", mapa[i].cor);
        printf("Tropas: %d\n", mapa[i].tropas);
    }
}


/* Função de ataque entre territórios */
void atacar(Territorio* atacante, Territorio* defensor) {

    printf("\n=== Ataque iniciado ===\n");
    printf("%s (%s) ATACANDO %s (%s)\n",
           atacante->nome, atacante->cor,
           defensor->nome, defensor->cor);

    if (atacante->tropas <= 1) {
        printf("O atacante nao tem tropas suficientes para atacar!\n");
        return;
    }

    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\nDado do atacante: %d\n", dadoAtacante);
    printf("Dado do defensor: %d\n\n", dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("Resultado: O atacante venceu!\n");

        int tropasOcupantes = atacante->tropas / 2;

        defensor->tropas = tropasOcupantes;
        strcpy(defensor->cor, atacante->cor);

        atacante->tropas -= tropasOcupantes;

        printf("O territorio defensor foi conquistado!\n");

    } else {
        printf("Resultado: O defensor resistiu!\n");
        atacante->tropas -= 1;
        printf("O atacante perdeu 1 tropa.\n");
    }
}


/* Libera a memória alocada dinamicamente */
void liberarMemoria(Territorio* mapa) {
    free(mapa);
    printf("\nMemoria liberada com sucesso!\n");
}
